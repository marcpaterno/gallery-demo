#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TH1F.h"

#include "analyze.hh"

// Since this is a main program, not a header, the using-directives
// below aren't bad practice; they allow us to make the code a bit
// more brief. In this demonstration code we specifically do not
// include a using-directive for the gallery namespace, so that names
// that are provided by gallery can be more easily identified.
using namespace art;
using namespace std;

int main(int argc, char** argv) {
  if (argc == 1) {
    cout << "Please specify the name of one or more art/ROOT input file(s) to "
            "read.\n";
    return 1;
  }

  // We'll read all files specified on the command line. We don't
  // bother to check that they are actually art/ROOT input files.
  vector<string> const filenames(argv + 1, argv + argc);

  // These are the InputTags that identify the products we'll read
  // from the gallery::Event. This is the same InputTag as is used in
  // art.
  InputTag const mctruths_tag{"generator"};
  InputTag const vertex_tag{"linecluster"};
  InputTag const assns_tag{"linecluster"};

  // ROOT indicates a problem with construction by setting the 'zombie
  // bit', and so we'll be careful to test that -- so as not to cause
  // a confusing problem later on if some contructor fails.
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
  // Use gallery::Event::atEnd() to check if you've reached the end of the
  // stream.
  // Use gallery::Event::next() to go to the next event.

  auto start_time = chrono::system_clock::now();
  vector<chrono::microseconds> times;  // we'll record the time for each event.

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    auto const t0 = chrono::system_clock::now();

    analyze(ev, mctruths_tag, vertex_tag, assns_tag,
            npart_hist, nclust_hist, nhits_hist);

    times.push_back(chrono::duration_cast<chrono::microseconds>(
        chrono::system_clock::now() - t0));
  }

  auto const elapsed_time = chrono::duration_cast<chrono::milliseconds>(
      chrono::system_clock::now() - start_time);
  auto const sum_times =
      accumulate(begin(times), end(times), chrono::microseconds(0));

  cout << "Processed " << times.size() << " events in an average of "
       << sum_times.count() / times.size() << " microseconds/event\n";
  cout << "Total processing time (including file opening) was "
       << elapsed_time.count() << " milliseconds\n";

  histfile.Write();
  histfile.Close();
}
