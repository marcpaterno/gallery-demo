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
using namespace hdfstudy::hdf5;

void
analyze_mctruths(gallery::Event const& ev,
                 InputTag const& mctruths_tag,
                 Ntuple<int>& nt)
{
  // getValidHandle() is preferred to getByLabel(), for both art and
  // gallery use. It does not require in-your-face error handling. We
  // immediately dereference the ValidHandle to obtain a (const)
  // reference to the underlying data product.
  auto const& mctruths =
    *ev.getValidHandle<vector<simb::MCTruth>>(mctruths_tag);
  for (auto const& mctruth : mctruths) {
    auto n = mctruth.NParticles();
    nt.insert(&n);
  }
}

void
analyze_vertices(gallery::Event const& ev,
                 InputTag const& vertices_tag,
                 Ntuple<double, double, double>& nt)
{
  auto const& vertices = *ev.getValidHandle<vector<Vertex>>(vertices_tag);

  for (auto const& vertex : vertices) {
    // The interface to Vertex forces us to use C-style code.
    // here.
    double pos[3];
    vertex.XYZ(pos);
    nt.insert(pos, pos+1, pos+2);
  }
}

void
analyze_vertex_cluster_correlations(gallery::Event const& ev,
                                    InputTag const& vertices_tag,
                                    InputTag const& assns_tag,
                                    Ntuple<unsigned int, size_t, float>& nt)
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

  auto const& aux = ev.eventAuxiliary();
  std::array<unsigned int, 3> const event_id { aux.run(), aux.subRun(), aux.event() };

  for (size_t i = 0, sz = vertices_h->size(); i != sz; ++i) {
    // We will fill this histogram once for each vertex.
    vector<Cluster const*> clusters;
    clusters_for_vertex.get(i, clusters);
        for (auto pcluster : clusters) {
      auto sadc = pcluster->SummedADC();
      nt.insert(event_id.data(), &i, &sadc);
    }
  }
}

void
analyze_cluster_hit_correlations(gallery::Event const& ev,
                                 InputTag const& clusters_tag,
                                 InputTag const& assns_tag,
                                 Ntuple<unsigned int, size_t, float>& nt)
{
  auto const clusters_h = ev.getValidHandle<vector<Cluster>>(clusters_tag);
  FindMany<Hit> hits_for_cluster(clusters_h, ev, assns_tag);

  auto const& aux = ev.eventAuxiliary();
  std::array<unsigned int, 3> const event_id { aux.run(), aux.subRun(), aux.event() };
  auto const& clusters = *clusters_h;

  for (size_t i = 0, sz = clusters_h->size(); i != sz; ++i) {
    auto const& cluster = clusters[i];
    // We will fill this histogram once for each cluster.
    vector<Hit const*> hits;
    hits_for_cluster.get(i, hits);
    for (auto phit : hits) {
      auto integral = phit->Integral();
      nt.insert(event_id.data(), &i, &integral);
    }
  }
}
