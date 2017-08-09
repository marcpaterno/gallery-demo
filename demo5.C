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
#include "TMath.h"

using namespace art;
using namespace std;

// make a poor-man's event display of recob::SpacePoints -- for the ievcount'th event in the file
// spacepoint tag string can be "pandora" or "pmtrack"

void
demo5(std::string const& filename, size_t ievcount, std::string spacepointtagstring="pandora")
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
	    int inroad=0;
	    int outroad=0;

	    double xpt[3] = {-15,400,0};
	    double dcos[3] = {-0.2,-0.15,0.9899};

	    TGraph2D *gri = new TGraph2D();
	    TGraph2D *gro = new TGraph2D();
            TCanvas *c = new TCanvas("c","TGraph2D Event Display",0,0,800,800);
	    for (size_t isp=0;isp<recobspacepoints.size(); ++isp)
	      {
		double x = recobspacepoints[isp].XYZ()[0];
		double y = recobspacepoints[isp].XYZ()[1];
		double z = recobspacepoints[isp].XYZ()[2];

		double xd = (x-xpt[0]);
		double yd = (y-xpt[1]);
		double zd = (z-xpt[2]);

		double xc = (yd * dcos[2] - dcos[1]*zd); // signs might be wrong here but we'll square it anyway
		double yc = (xd * dcos[2] - dcos[0]*zd);
		double zc = (xd * dcos[1] - dcos[0]*yd);
		double sdiff = TMath::Sqrt(xc*xc+yc*yc+zc*zc);

		if (sdiff < 50.0) // 50 cm cut from the road
		  {
		    gri->SetPoint(inroad,z,x,y);
		    ++inroad;
		  }
		else
		  {
		    gro->SetPoint(outroad,z,x,y);
		    ++outroad;
		  }
	      }

	    gro->SetMarkerColor(1);
	    gro->SetMarkerStyle(1);
	    std::string titlestring=spacepointtagstring;
	    titlestring += " SpacePoint Display";
	    gro->SetTitle(titlestring.c_str());
	    gro->Draw("P");
	    gro->GetXaxis()->SetTitle("Z");
	    gro->GetXaxis()->SetTitleColor(4);
	    gro->GetYaxis()->SetTitle("X");
	    gro->GetYaxis()->SetTitleColor(4);
	    gro->GetZaxis()->SetTitle("Y");
	    gro->GetZaxis()->SetTitleColor(4);

	    
	    gri->SetMarkerColor(2);
	    gri->SetMarkerStyle(1);
	    gri->Draw("PSAME");
	    c->Update();
	  }
      }
    ++evcounter;
  }
}
