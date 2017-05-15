#include "analyze.hh"

#include "canvas/Persistency/Common/FindMany.h"
#include "gallery/ValidHandle.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "nusimdata/SimulationBase/MCTruth.h"

#include <vector>

using namespace art;
using namespace recob;
using namespace std;
using namespace hep_hpc::hdf5;

// Return an array of unsigned int, carrying the elements of the
// EventID for the given gallery::Event.
inline std::array<unsigned int, 3>
get_eid(gallery::Event const& ev)
{
  auto const& aux = ev.eventAuxiliary();
  return { aux.run(), aux.subRun(), aux.event() };
};

void
analyze_mctruths(gallery::Event const& ev,
                 InputTag const& mctruths_tag,
                 mctruth_nt_t& nt)
{
  auto event_id = get_eid(ev);
  // getValidHandle() is preferred to getByLabel(), for both art and
  // gallery use. It does not require in-your-face error handling. We
  // immediately dereference the ValidHandle to obtain a (const)
  // reference to the underlying data product.
  auto const& mctruths =
    *ev.getValidHandle<vector<simb::MCTruth>>(mctruths_tag);
  for (auto const& mctruth : mctruths) {
    nt.insert(event_id.data(), static_cast<int>(mctruth.NParticles()));
  }
}

void
analyze_vertices(gallery::Event const& ev,
                 InputTag const& vertices_tag,
                 vertex_nt_t& nt)
{
  auto event_id = get_eid(ev);
  auto const& vertices = *ev.getValidHandle<vector<Vertex>>(vertices_tag);

  for (size_t i = 0, sz = vertices.size(); i != sz; ++i) {
    // The interface to Vertex forces us to use C-style code.
    // here.
    double pos[3];
    vertices[i].XYZ(pos);
    nt.insert(event_id.data(), static_cast<int>(i), pos[0], pos[1], pos[2]);
  }
}

void
analyze_vertex_cluster_correlations(gallery::Event const& ev,
                                    InputTag const& vertices_tag,
                                    InputTag const& assns_tag,
                                    cluster_nt_t& nt)
{
  // Note that we do not dereference the ValidHandle -- vertices_h is
  // a ValidHandle<vector<Vertex>>. We will need the handle to
  // form the smart query object below.
  auto const vertices_h = ev.getValidHandle<vector<Vertex>>(vertices_tag);

  // We loop over each vertex, and for each vertex loop over all
  // associated clusters. We have to use an indexec for loop so that
  // we have the index 'i' to use within the loop, as required for use
  // with the FindMany smart query object. The second template
  // parameter of the FindMany indicates that each association between
  // a Vertex and Cluster has an associated datum of type 'unsigned
  // short'. Consult the documentation of the LArSoft data model to
  // determine what that datum means!
  FindMany<Cluster, unsigned short> clusters_for_vertex(
    vertices_h, ev, assns_tag);
  auto event_id = get_eid(ev);

  for (size_t iv = 0, szv = vertices_h->size(); iv != szv; ++iv) {
    vector<Cluster const*> clusters;
    clusters_for_vertex.get(iv, clusters);
    for (size_t ic = 0, szc = clusters.size(); ic != szc; ++ic) {
      nt.insert(event_id.data(),
                static_cast<int>(ic),
                static_cast<int>(iv),
                clusters[ic]->SummedADC());
    }
  }
}

void
analyze_cluster_hit_correlations(gallery::Event const& ev,
                                 InputTag const& clusters_tag,
                                 InputTag const& assns_tag,
                                 hit_nt_t& nt)
{
  auto const clusters_h = ev.getValidHandle<vector<Cluster>>(clusters_tag);
  FindMany<Hit> hits_for_cluster(clusters_h, ev, assns_tag);

  auto event_id = get_eid(ev);

  auto const& clusters = *clusters_h;

  for (size_t i = 0, sz = clusters_h->size(); i != sz; ++i) {
    int n = static_cast<int>(i);
    auto const& cluster = clusters[i];
    vector<Hit const*> hits;
    hits_for_cluster.get(i, hits);
    for (size_t j = 0, hsz = hits.size(); j != hsz; ++j) {
      auto const& hit = *hits[j];
      nt.insert(event_id.data(),
                static_cast<int>(j),
                n,
                hit.Integral(),
                hit.GoodnessOfFit(),
                hit.DegreesOfFreedom());
    }
  }
}
