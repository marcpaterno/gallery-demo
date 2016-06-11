#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TFile.h"

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCNeutrino.h"

#include "canvas/Persistency/Common/FindMany.h"

using namespace art;
using namespace std;

void demo() {

  InputTag mctruths_tag { "generator" };
  InputTag vertex_tag { "linecluster" };
  InputTag assns_tag  { "linecluster" };
  vector<string> filenames { "dune.root" }; // multiple files are allowed.

  // Don't do this in compiled C++. We're relying on the interactive
  // ROOT system to own the histograms we create on the heap.
  auto npart_hist = new TH1F("npart", "Number of particles per MCTruth", 51, -0.5, 50.5);
  auto nclust_hist = new TH1F("nclus", "Number of clusters per vertex", 51, -0.5, 50.5);

  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
    auto const& mctruths = *ev.getValidHandle<vector<simb::MCTruth>>(mctruths_tag);
    if (!mctruths.empty()) npart_hist->Fill(mctruths[0].NParticles());

    using vertices_t = vector<recob::Vertex>;
    auto const& vertices_h = ev.getValidHandle<vertices_t>(vertex_tag);

    FindMany<recob::Cluster, unsigned short> clus_for_vtx(vertices_h, ev, assns_tag);
    for (size_t i = 0, sz = vertices_h->size(); i != sz; ++i) {
      vector<recob::Cluster const*> clusters;
      clus_for_vtx.get(i, clusters);
      nclust_hist->Fill(clusters.size());
    }
  }
}
