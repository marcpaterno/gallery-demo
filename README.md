# gallery-demo

This is the demonstration repository for `gallery`, a package for the reading of `art/ROOT` data files. Please see http://art.fnal.gov/gallery for an overview of `gallery`.


## When to use `gallery`

`gallery` provides library (as opposed to framework) facilities for reading (not writing) event data from _art_/ROOT data files. As such, it is suitable for making programs that perform tasks that do not require the full set of features of the _art_ framework. Such tasks include things like reading event data to create histograms or ntuples.

Because it allows use of your experiment's event data product classes, you can use `gallery` to write code that is easy to move into your experiment's code base. The access to event data in `gallery` is very similar to that in _art_. The `gallery::Event` interface provides the same `getByLabel`, _etc_., as is present in the `art::Event` class.

However, `gallery` is not a full-featured framework. In fact, it is not a framework at all. Thus is it not appropriate for tasks that require framework facilities, including:
* access to non-event data,
* reconstruction, i.e. adding new data products to the `gallery::Event`,
* writing of _art_/ROOT data files, and
* use of "services" that are framework-dependent.

## Building and Running the demonstrations

There are three different demonstration codes, listed below. Building and use of each requires the right environment be established. Specifically, an appropriate version of the UPS product `gallery` must be available, and ROOT dictionaries for the classes corresponding to objects in the file to be processed must be available. All this is usually established by "setting up" your experiment's software environment, using whatever setup scripts are provided by your experiment. The file `demo-setup` in this repository is an example setup script, which assumes that one has available a specific version of the `larsoftobj` distribution bundle. Please consult the instructions within that file for additional instructions.

The main source code for the three demonstration programs are `demo.cc`, `demo.C`, and `demo.py`. The compiled C++ program is the most complete.

### `demo.cc`

`demo.cc` is a C++ main program which processes an input file and creates a ROOT histogram file. It is built with CMake, which is usually availble (along with your experiment software) through setup using UPS. Any "new enough" CMake can be used. This does not use the facilities provided by `cetbuildtools`; it is "bare" CMake. To compile and link the `demo` executable, use the following commands, from the directory containing the `CMakeLists.txt` file:
```
$ cmake .
$ make
```

The executable program `demo` expects one or more arguments, the names of the _art_/ROOT data files to be read.  `demo.cc` makes use of the function and types declared in  `analyze.hh`, with some implementation in `analyze.cc`. some , declared in `analyze.hh` and defined in `analyze.cc`. The compiled program writes ROOT histograms to an output file named "histfile.root".

### `demo.C`

`demo.C` is a ROOT macro intended to be invoked within an interactive ROOT session, and which creates a histogram in memory. The macro function `demo` takes a single argument, the name of the _art_/ROOT data file to be processed. Assuming one has a local _art_/ROOT file named "sample.root", a sample session might be:
```
$ root -l
root [0] .x demo.C("sample.root")
Successfully opened file sample.root
Closing file, read 1232928 bytes in 119 transactions
Processed 100 events in an average of 396 microseconds/event
Total processing time (including file opening) was 326 milliseconds
root [1] npart->Draw();
Info in <TCanvas::MakeDefCanvas>:  created default TCanvas with name c1
```

### `demo.py`

`demo.py` is a PyROOT program that processes an input file and creates a ROOT histogram file. It can be run with `python demo.py`

The PyROOT demonstrates some use of PyROOT. Note that the quality of PyROOT support varies by operating system, and over time. In particular, PyROOT has often failed on Apple operating systems. In particular, there is a `dynamic_cast` involved in the reading of data products which works correctly in C++ on all platforms, and which works in PyROOT on Linux platforms, but which fails on Apple systems. The result it that it appears that the data product is not available in the input file, even when it is present in the file.

A typical output for the `demo.py` program in this case is:
```
$ python demo.py dune.root
Starting demo...
Reading headers...
Instantiating member templates...
Preparing before event loop...
Creating event object ...
Successfully opened file dune.root
Entering event loop...
terminate called after throwing an instance of 'cet::coded_exception<art::errors::ErrorCodes, &art::ExceptionDetail::translate>'
  what():  ---- ProductNotFound BEGIN
  Failed to find product for
    type = 'std::vector<simb::MCTruth>'
    module = 'generator'
    productInstance = ''
    process=''
---- ProductNotFound END
```

This failure is, unfortunately, in the interals of ROOT and PyROOT, and is not fixable in `gallery`.

## Suggested exercises

1. The main program `demo.cc` can be extended to do the creation of whatever histograms, using whatever data products, you find interesting. Try extending it with new functionality.
2. The ROOT macro `demo.C` does only part of what `demo.cc` does. If you prefer using interactive ROOT, try updating `demo.C` to do the other things that `demo.cc` does. Pay attention to modularity! Try following the lead of `demo.cc` in that regard.
3. The PyROOT program `demo.py` does only a part of what `demo.cc` does. If you prefer using PyROOT, and are using a system on which it works sufficiently well, try extending the example to do what `demo.cc` does.
