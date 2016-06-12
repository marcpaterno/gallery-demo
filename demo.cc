#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "nusimdata/SimulationBase/MCTruth.h"

#include "TFile.h"
#include "TH1F.h"

#include "for_each_entry.hh"
#include "for_each_associated.hh"

using namespace art;
using namespace std;

int main() {

  InputTag mctruths_tag { "generator" };
  InputTag vertex_tag { "linecluster" };
  InputTag assns_tag  { "linecluster" };
  vector<string> filenames { "dune.root" }; // multiple files are allowed.

  TFile histfile("histfile.root", "RECREATE");
  if (histfile.IsZombie()) {
    cerr << "Unable to open 'histfile.root, exiting...'\n";
    return 1;
  }

  TH1F npart_hist("npart", "Number of particles per MCTruth", 51, -0.5, 50.5);
  if (npart_hist.IsZombie()) {
    cerr << "Unable to create npart_hist, exiting...\n";
    return 1;
  }  

  TH1F nclust_hist("nclust", "Number of clusters per vertex", 51, -0.5, 50.5);
  if (nclust_hist.IsZombie()) {
    cerr << "Unable to create nclust_hist, exiting...\n";
    return 1;
  }

  TH1F nhits_hist("nhits", "Number of hits per cluster", 101, -0.5, 100.5);
  if (nhits_hist.IsZombie()) {
    cerr << "Unable to create nhits_hits, exiting...\n";
  }

  // The gallery::Event object acts as a cursor into the stream of events.
  // A newly-constructed gallery::Event is at the start if its stream.
  // Use gallery::Event::atEnd() to check if you've reached the end of the stream.
  // Use gallery::Event::next() to go to the next event.
  
  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    // getValidHandle() is preferred to getByLabel(), for both art and
    // gallery use. It does not require in-your-face error handling.
    auto const& mctruths = *ev.getValidHandle<vector<simb::MCTruth>>(mctruths_tag);
    if (!mctruths.empty()) npart_hist.Fill(mctruths[0].NParticles());

    using vertices_t = vector<recob::Vertex>;
    auto const& vertices_h = ev.getValidHandle<vertices_t>(vertex_tag);
    FindMany<recob::Cluster, unsigned short> clusters_for_vertex(vertices_h, ev, assns_tag);

    // for_each_entry applies the given function to each entry (here, vector<recob::Cluster>)
    auto fill_nclust_hist = [&h=nclust_hist](auto const& clusters) { h.Fill(clusters.size()); };
    for_each_entry(clusters_for_vertex, fill_nclust_hist);

    // for_each_associated applies the given function to each
    // associated item (here, recob::Cluster).
    auto fill_nhits_hist = [&h=nhits_hist](auto pcluster) { h.Fill(pcluster->NHits()); };
    for_each_associated(clusters_for_vertex, fill_nhits_hist);
  }
  histfile.Write();
  histfile.Close();
}
