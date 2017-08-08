#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TGraph.h"

using namespace art;
using namespace std;

// make a poor-man's event display of recob::hits -- peaktime vs. channel, for the ievcount'th event in the file

void
demo3(std::string const& filename, size_t ievcount)
{

  size_t evcounter=0;

  InputTag recobhit_tag{ "gaushit" };
  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    if (evcounter == ievcount)
      {
	auto const& recobhits = *ev.getValidHandle<vector<recob::Hit>>(recobhit_tag);
	if (!recobhits.empty())
	  {
	    vector<double> xvals;
	    vector<double> yvals;

	    for (size_t ihit=0;ihit<recobhits.size(); ++ihit)
	      {
		xvals.push_back(recobhits[ihit].Channel());
		yvals.push_back(recobhits[ihit].PeakTime());
	      }
	    TGraph *gr = new TGraph(xvals.size(),xvals.data(),yvals.data());
	    gr->SetMarkerColor(1);
	    gr->SetMarkerStyle(1);
	    gr->SetTitle("TGraph Event Display");
	    gr->GetXaxis()->SetTitle("Channel");
	    gr->GetYaxis()->SetTitle("Tick");
	    gr->Draw("AP");
	  }
      }
    ++evcounter;
  }
}
