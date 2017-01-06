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

void analyze(gallery::Event const& ev,
             InputTag const& mctruths_tag,
             InputTag const& vertex_tag,
             InputTag const& assns_tag,
             TH1F& npart_hist,
             TH1F& nclust_hist,
             TH1F& nhits_hist) {

    // getValidHandle() is preferred to getByLabel(), for both art and
    // gallery use. It does not require in-your-face error handling.
    auto const& mctruths =
        *ev.getValidHandle<vector<simb::MCTruth> >(mctruths_tag);
    if (!mctruths.empty()) npart_hist.Fill(mctruths[0].NParticles());

    using vertices_t = vector<recob::Vertex>;
    auto const& vertices_h = ev.getValidHandle<vertices_t>(vertex_tag);
    FindMany<recob::Cluster, unsigned short> clusters_for_vertex(vertices_h, ev,
                                                                 assns_tag);

    for (size_t i = 0, sz = vertices_h->size(); i != sz; ++i) {
      vector<recob::Cluster const*> clusters;
      clusters_for_vertex.get(i, clusters);
      nclust_hist.Fill(clusters.size());

      for (auto pclus : clusters) {
        if (!pclus) continue; // skip null pointers
        nhits_hist.Fill(pclus->NHits());
      }
      
    }
}
