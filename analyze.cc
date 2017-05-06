#include "analyze.hh"

#include "canvas/Persistency/Common/FindMany.h"
#include "gallery/ValidHandle.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "nusimdata/SimulationBase/MCTruth.h"

#include "range/v3/all.hpp"

#include <vector>

using namespace art;
using namespace recob;
using namespace std;
using namespace ranges::v3;

#include "TH1F.h"
#include "TH2F.h"

void
analyze_mctruths(gallery::Event const& ev,
                 InputTag const& mctruths_tag,
                 TH1F& hist)
{
  // getValidHandle() is preferred to getByLabel(), for both art and
  // gallery use. It does not require in-your-face error handling. We
  // immediately dereference the ValidHandle to obtain a (const)
  // reference to the underlying data product.
  ranges::for_each(*ev.getValidHandle<vector<simb::MCTruth>>(mctruths_tag) |
                     view::transform(&simb::MCTruth::NParticles),
                   [&hist](int n) { hist.Fill(n); });
}

void
analyze_vertices(gallery::Event const& ev,
                 InputTag const& vertices_tag,
                 TH1F& xhist,
                 TH1F& yhist,
                 TH1F& zhist,
                 TH2F& xyhist)
{
  auto vertex_pos = [](auto const& vtx) {
    std::array<double, 3> pos;
    vtx.XYZ(pos.data());
    return pos;
  };

  ranges::for_each(*ev.getValidHandle<vector<Vertex>>(vertices_tag) |
                     view::transform(vertex_pos),
                   [&](auto const& pos) {
                     xhist.Fill(pos[0]);
                     yhist.Fill(pos[1]);
                     zhist.Fill(pos[2]);
                     xyhist.Fill(pos[0], pos[1]);
                   });
}

void
analyze_vertex_cluster_correlations(gallery::Event const& ev,
                                    InputTag const& vertices_tag,
                                    InputTag const& assns_tag,
                                    TH2F& hist)
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

  for (size_t i = 0, sz = vertices_h->size(); i != sz; ++i) {
    // We will fill this histogram once for each vertex.
    vector<Cluster const*> clusters;
    clusters_for_vertex.get(i, clusters);
    float adc_sum = ranges::accumulate(
      clusters | view::transform(&recob::Cluster::SummedADC), 0.0f);
    hist.Fill(clusters.size(), adc_sum);
  }
}

void
analyze_cluster_hit_correlations(gallery::Event const& ev,
                                 InputTag const& clusters_tag,
                                 InputTag const& assns_tag,
                                 TH2F& hist)
{
  auto const clusters_h = ev.getValidHandle<vector<Cluster>>(clusters_tag);
  FindMany<Hit> hits_for_cluster(clusters_h, ev, assns_tag);

  for (size_t i = 0, sz = clusters_h->size(); i != sz; ++i) {
    auto const& cluster = (*clusters_h)[i];
    // We will fill this histogram once for each cluster.
    vector<Hit const*> hits;
    hits_for_cluster.get(i, hits);
    float adc = cluster.SummedADC();
    float summed_integrals =
      ranges::accumulate(hits | view::transform(&recob::Hit::Integral), 0.0f);
    hist.Fill(adc, summed_integrals);
  }
}
