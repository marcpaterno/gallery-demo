#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TROOT.h"

using namespace art;
using namespace std;
using namespace std::chrono;

void
demo(std::string const& filename)
{
  InputTag mctruths_tag{ "generator" };
  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  // Don't do the following in compiled C++. This code relies on the
  // interactive ROOT system to (implicitly) own the histograms we
  // create on the heap. In a C++ program, this object would be leaked
  // (the memory unrecoverable by the program).
  TH1F* npart_hist =
    new TH1F("npart", "Number of particles per MCTruth", 51, -0.5, 50.5);

  auto start_time = system_clock::now();
  // We'll record the time it takes to process each gallery::Event.
  vector<microseconds> times;

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    auto const t0 = system_clock::now();
    auto const& mctruths =
      *ev.getValidHandle<vector<simb::MCTruth>>(mctruths_tag);
    if (!mctruths.empty())
      npart_hist->Fill(mctruths[0].NParticles());

    times.push_back(duration_cast<microseconds>(system_clock::now() - t0));
  }
  auto const elapsed_time =
    duration_cast<milliseconds>(system_clock::now() - start_time);
  auto const sum_times = accumulate(begin(times), end(times), microseconds(0));

  cout << "Processed " << times.size() << " events in an average of "
       << sum_times.count() / times.size() << " microseconds/event\n";
  cout << "Total processing time (including file opening, but not macro "
          "compilation) was "
       << elapsed_time.count() << " milliseconds\n";
}
