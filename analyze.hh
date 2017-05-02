#ifndef ANALYZE_HH
#define ANALYZE_HH

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "hdfstudy/hdf5/Ntuple.hpp"

void
analyze_mctruths(gallery::Event const& ev,
                 art::InputTag const& mctruth_tag,
                 hdfstudy::hdf5::Ntuple<int>& nt);

void
analyze_vertices(gallery::Event const& ev,
                 art::InputTag const& vertices_tag,
                 hdfstudy::hdf5::Ntuple<double,double,double>& nt);

void
analyze_vertex_cluster_correlations(gallery::Event const& ev,
                                    art::InputTag const& vertices_tag,
                                    art::InputTag const& assns_tag,
                                    hdfstudy::hdf5::Ntuple<unsigned int, size_t,float>& nt);

void
analyze_cluster_hit_correlations(gallery::Event const& ev,
                                 art::InputTag const& clusters_tag,
                                 art::InputTag const& assns_tag,
                                 hdfstudy::hdf5::Ntuple<unsigned int, size_t,float>& nt);
#endif
