#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "hdfstudy/hdf5/File.hpp"
#include "hdfstudy/hdf5/Ntuple.hpp"

#include "analyze.hh"

// Since this is a main program, not a header, the using-directives
// below aren't bad practice; they allow us to make the code a bit
// more brief. In this demonstration code we specifically do not
// include a using-directive for the gallery namespace, so that names
// that are provided by gallery can be more easily identified.
using namespace art;
using namespace std;
using namespace std::chrono;
using namespace std::string_literals;
using namespace hdfstudy::hdf5;

// We use a function try block to catch and report on all exceptions.
int
main(int argc, char** argv) try {
  if (argc == 1) {
    cout << "Please specify the name of one or more art/ROOT input file(s) to "
            "read.\n";
    return 1;
  }

  // We'll read all files specified on the command line. We don't
  // bother to check that they are actually art/ROOT input files;
  // gallery::Event will let us know if they are not.
  vector<string> const filenames(argv + 1, argv + argc);

  // The Tags struct contains the InputTags that identify the products
  // we'll read from the gallery::Event. They are the same InputTags
  // as are used in art modules. The string specified is the label of
  // the module the produced the product in question.

  InputTag const mctruths_tag("generator");
  InputTag const vertices_tag("linecluster");
  InputTag const clusters_tag("linecluster");
  InputTag const vertex_cluster_assns("linecluster");
  InputTag const cluster_hit_assns("linecluster");

  // Make some ntuples to store our tabular data.
  File hdffile("demo.h5", H5F_ACC_TRUNC);
  Ntuple<int> mctruths(hdffile, "mctruths"s, { "nparticles"s });
  Ntuple<double, double, double> vertices(hdffile, "vertices"s, {"x"s,"y"s,"z"s});
  Ntuple<unsigned int, size_t, float> clusters(hdffile, "clusters"s, {{"eid"s, 3}, "vtx"s, "sumadc"s});
  Ntuple<unsigned int, size_t, float> hits(hdffile, "hits"s, {{"eid"s, 3}, "clus"s,"integral"s});

  // The gallery::Event object acts as a cursor into the stream of
  // events.  A newly-constructed gallery::Event is at the start if
  // its stream.  Use gallery::Event::atEnd() to check if you've
  // reached the end of the stream. Use gallery::Event::next() to go
  // to the next event.

  auto const start_time = system_clock::now();
  vector<microseconds> times; // we'll record the time for each event.

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    auto const t0 = system_clock::now();
    analyze_mctruths(ev, mctruths_tag, mctruths);
    analyze_vertices(ev, vertices_tag, vertices);
    analyze_vertex_cluster_correlations(ev, vertices_tag, vertex_cluster_assns, clusters);
    analyze_cluster_hit_correlations(ev, clusters_tag, cluster_hit_assns, hits);
    times.push_back(duration_cast<microseconds>(system_clock::now() - t0));
  }

  auto const elapsed_time =
    duration_cast<milliseconds>(system_clock::now() - start_time);
  auto const sum_times =
    std::accumulate(begin(times), end(times), microseconds(0));

  cout << "Processed " << times.size() << " events in an average of "
       << sum_times.count() / times.size() << " microseconds/event\n";
  cout << "Total processing time (including file opening) was "
       << elapsed_time.count() << " milliseconds\n";
} catch (std::exception const& ex) {
  std::cerr << ex.what() << '\n';
  return 1;
} catch (...) {
  std::cerr << "Unidentifiable exception caught\n";
  return 1;
}
