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

// make a poor-man's event display of recob::SpacePoints -- for the ievcount'th event in the file
// spacepoint tag string can be "pandora" or "pmtrack"

void
demo4(std::string const& filename, size_t ievcount, std::string spacepointtagstring="pandora")
{

  size_t evcounter=0;

  InputTag recobspacepoint_tag{ spacepointtagstring };
  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    if (evcounter == ievcount)
      {
	auto const& recobspacepoints = *ev.getValidHandle<vector<recob::SpacePoint>>(recobspacepoint_tag);
	if (!recobspacepoints.empty())
	  {
	    TGraph2D *gr = new TGraph2D();
            TCanvas *c = new TCanvas("c","TGraph2D Event Display",0,0,800,800);
	    for (size_t isp=0;isp<recobspacepoints.size(); ++isp)
	      {
		gr->SetPoint(isp,recobspacepoints[isp].XYZ()[2],recobspacepoints[isp].XYZ()[0],recobspacepoints[isp].XYZ()[1]);
	      }

	    gr->SetMarkerColor(1);
	    gr->SetMarkerStyle(1);
	    std::string titlestring=spacepointtagstring;
	    titlestring += " SpacePoint Display";
	    gr->SetTitle(titlestring.c_str());
	    gr->Draw("P");
	    gr->GetXaxis()->SetTitle("Z");
	    gr->GetXaxis()->SetTitleColor(4);
	    gr->GetYaxis()->SetTitle("X");
	    gr->GetYaxis()->SetTitleColor(4);
	    gr->GetZaxis()->SetTitle("Y");
	    gr->GetZaxis()->SetTitleColor(4);
	    c->Update();
	  }
      }
    ++evcounter;
  }
}
