#ifndef ANALYZE_HH
#define ANALYZE_HH

#include "gallery/Event.h"
#include "canvas/Utilities/InputTag.h"

#include "TH1F.h"

// We introduce the Tags struct to group together all the InputTags in
// which we are interested. We use a struct (rather than a
// vector<InputTag> or array<InputTag> or a tuple<...>) so that we can
// give meaningful names to the individual InputTag
// objects. Everything is declared const, because there is no need to
// modify an InputTag after it is constructed.
struct Tags {
  art::InputTag const mctruths;
  art::InputTag const vertices;
  art::InputTag const vertex_cluster_assns;
  art::InputTag const cluster_hit_assns;
};

// We introduce the Histos struct to group together all the histogram
// objects we'll be filling. They are not declared const, because
// we'll be modifying (filling!) them.
struct Histos {
  TH1F nparticles;
  TH1F nclusters;
  TH1F nhits;
};

void analyze(gallery::Event const& ev, Tags const& tags, Histos& hists);

#endif
