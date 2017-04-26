#include "analyze.hh"

#include <vector>

// Interactive ROOT is very sensitive to the order of these
// headers. Take care with any reordering.
#include "gallery/ValidHandle.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "nusimdata/SimulationBase/MCTruth.h"

#include "canvas/Persistency/Common/FindMany.h"

using namespace art;
using namespace std;

void analyze(gallery::Event const& ev, Tags const& tags, Histos& hists) {

  // getValidHandle() is preferred to getByLabel(), for both art and
  // gallery use. It does not require in-your-face error handling.
  auto const& mctruths =
      *ev.getValidHandle<vector<simb::MCTruth> >(tags.mctruths);
  if (!mctruths.empty())
    hists.nparticles.Fill(mctruths[0].NParticles());

  using vertices_t = vector<recob::Vertex>;
  auto const& vertices_h = ev.getValidHandle<vertices_t>(tags.vertices);
  FindMany<recob::Cluster, unsigned short> clusters_for_vertex(
      vertices_h, ev, tags.vertex_cluster_assns);

  for (size_t i = 0, sz = vertices_h->size(); i != sz; ++i) {
    vector<recob::Cluster const*> clusters;
    clusters_for_vertex.get(i, clusters);
    hists.nclusters.Fill(clusters.size());

    for (auto pclus : clusters) {
      if (!pclus)
        continue; // skip null pointers
      hists.nhits.Fill(pclus->NHits());
    }
  }
}
