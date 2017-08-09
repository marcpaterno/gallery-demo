#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TGraph2D.h"

using namespace art;
using namespace std;

// make a poor-man's event display of recob::SpacePoints -- peaktime vs. channel, for the ievcount'th event in the file

void
demo4(std::string const& filename, size_t ievcount)
{

  size_t evcounter=0;

  InputTag recobspacepoint_tag{ "pandora" };
  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    if (evcounter == ievcount)
      {
	auto const& recobspacepoints = *ev.getValidHandle<vector<recob::SpacePoint>>(recobspacepoint_tag);
	if (!recobspacepoints.empty())
	  {
	    int ipc=0;
	    TGraph2D *gr = new TGraph2D();
            TCanvas *c = new TCanvas("c","TGraph2D Event Display",0,0,800,800);
	    for (size_t isp=0;isp<recobspacepoints.size(); ++isp)
	      {
		gr->SetPoint(ipc,recobspacepoints[isp].XYZ()[0],recobspacepoints[isp].XYZ()[1],recobspacepoints[isp].XYZ()[2]);
		ipc ++;
	      }

	    gr->SetMarkerColor(1);
	    gr->SetMarkerStyle(1);
	    gr->SetTitle("TGraph2D SpacePoint Display");
	    gr->Draw("AP");
	    c->Update();
	    gr->GetXaxis()->SetTitle("X");
	    gr->GetYaxis()->SetTitle("Y");
	    gr->GetYaxis()->SetTitle("Z");
	  }
      }
    ++evcounter;
  }
}
