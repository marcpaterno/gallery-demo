#ifndef NTUPLE_TYPES_HH
#define NTUPLE_TYPES_HH

#include "hep_hpc/hdf5/Ntuple.hpp"

using mctruth_nt_t = hep_hpc::hdf5::Ntuple<unsigned int, int>;
using vertex_nt_t =
  hep_hpc::hdf5::Ntuple<unsigned int, int, double, double, double>;
using cluster_nt_t = hep_hpc::hdf5::Ntuple<unsigned int, int, int, float>;
using hit_nt_t =
  hep_hpc::hdf5::Ntuple<unsigned int, int, int, float, float, short>;

#endif
