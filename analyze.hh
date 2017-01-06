#ifndef ANALYZE_HH
#define ANALYZE_HH

#include "gallery/Event.h"
#include "canvas/Utilities/InputTag.h"

#include "TH1F.h"

void analyze(gallery::Event const& ev,
             art::InputTag const& mctruths_tag,
             art::InputTag const& vertex_tag,
             art::InputTag const& assns_tag,
             TH1F& npart_hist,
             TH1F& nclust_hist,
             TH1F& nhits_hist);

#endif
