#ifndef ANALYZE_HH
#define ANALYZE_HH

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "ntuple_types.hh"

void
analyze_mctruths(gallery::Event const& ev,
                 art::InputTag const& mctruth_tag,
                 mctruth_nt_t& nt);

void
analyze_vertices(gallery::Event const& ev,
                 art::InputTag const& vertices_tag,
                 vertex_nt_t& nt);

void
analyze_vertex_cluster_correlations(gallery::Event const& ev,
                                    art::InputTag const& vertices_tag,
                                    art::InputTag const& assns_tag,
                                    cluster_nt_t& nt);

void
analyze_cluster_hit_correlations(gallery::Event const& ev,
                                 art::InputTag const& clusters_tag,
                                 art::InputTag const& assns_tag,
                                 hit_nt_t& nt);
#endif
