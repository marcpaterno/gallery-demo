#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
//#include "gallery/ValidHandle.h"
//#include "lardataobj/RecoBase/Cluster.h"
//#include "lardataobj/RecoBase/Vertex.h"
//#include "nusimdata/SimulationBase/MCTruth.h"

#include "TH1F.h"
#include "TFile.h"
#include "TInterpreter.h"
#include "TROOT.h"

#include "analyze.cc"

using namespace art;
using namespace std;

void demo(std::string const& filename) {

  InputTag mctruths_tag { "generator" };
  InputTag vertex_tag { "linecluster" };
  InputTag assns_tag  { "linecluster" };
  vector<string> filenames(1, filename);

  // Don't do this in compiled C++. We're relying on the interactive
  // ROOT system to own the histograms we create on the heap.
  auto npart_hist = new TH1F("npart", "Number of particles per MCTruth", 51, -0.5, 50.5);
  auto nclust_hist = new TH1F("nclus", "Number of clusters per vertex", 51, -0.5, 50.5);
  auto nhits_hist = new TH1F("nhits", "Number of hits per cluster", 101, -0.5, 100.5);

  auto  start_time = chrono::system_clock::now();
  vector<chrono::microseconds> times; // we'll record the time for each event.

  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
    auto const t0 = chrono::system_clock::now();

    analyze(ev, mctruths_tag, vertex_tag, assns_tag,
            *npart_hist, *nclust_hist, *nhits_hist);
    
    times.push_back(chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now()-t0));
  }
  auto const elapsed_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start_time);
  auto const sum_times = accumulate(begin(times), end(times), chrono::microseconds(0));
  
  cout << "Processed " << times.size()
       << " events in an average of "
       <<  sum_times.count()/times.size()
       << " microseconds/event\n";
  cout << "Total processing time (including file opening) was "
       << elapsed_time.count()
       << " milliseconds\n";
}
