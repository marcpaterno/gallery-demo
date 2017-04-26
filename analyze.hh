#ifndef ANALYZE_HH
#define ANALYZE_HH

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

// We need to that TH1F names a class, but we don't need to know more
//  about that class here.
class TH1F;
class TH2F;

void
analyze_mctruths(gallery::Event const& ev,
                 art::InputTag const& mctruth_tag,
                 TH1F& hist);

void
analyze_vertices(gallery::Event const& ev,
                 art::InputTag const& vertices_tag,
                 TH1F& xhist,
                 TH1F& yhist,
                 TH1F& zhist,
                 TH2F& xyhist);

void
analyze_vertex_cluster_correlations(gallery::Event const& ev,
                                    art::InputTag const& vertices_tag,
                                    art::InputTag const& assns_tag,
                                    TH2F& hist);

void
analyze_cluster_hit_correlations(gallery::Event const& ev,
                                 art::InputTag const& clusters_tag,
                                 art::InputTag const& assns_tag,
                                 TH2F& hist);
#endif
