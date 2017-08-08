#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TROOT.h"

using namespace art;
using namespace std;

// make a poor-man's event display of recob::hits -- peaktime vs. channel, for the ievcount'th event in the file

void
demo2(std::string const& filename, size_t ievcount)
{

  size_t evcounter=0;

  InputTag recobhit_tag{ "gaushit" };
  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  TH2F *evh = new TH2F("evh","TH2F event display;Channel Number;Hit Time (ticks)",500,-0.5,15100,500,0,8000);

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    if (evcounter == ievcount)
      {
	auto const& recobhits = *ev.getValidHandle<vector<recob::Hit>>(recobhit_tag);
	if (!recobhits.empty())
	  {
	    for (size_t ihit=0;ihit<recobhits.size(); ++ihit)
	      {
		evh->Fill(recobhits[ihit].Channel(),recobhits[ihit].PeakTime());
	      }
	    evh->SetStats(0);
	    evh->Draw("colz");
	  }
      }
    ++evcounter;
  }
}
