# gallery-demo

This is the demonstration repository for `gallery`, a package for the reading of `art/ROOT` data files. Please see http://art.fnal.gov/gallery for an overview of `gallery`.


## When to use `gallery`

`gallery` provides library (as opposed to framework) facilities for reading (not writing) event data from _art_/ROOT data files. As such, it is suitable for making programs that perform tasks that do not require the full set of features of the _art_ framework. Such tasks include things like reading event data to create histograms or ntuples.

Because it allows use of your experiment's event data product classes, you can use `gallery` to write code that is easy to move into your experiment's code base. The access to event data in `gallery` is very similar to that in _art_. The `gallery::Event` interface provides the same `getByLabel`, _etc_., as is present in the `art::Event` class.

However, `gallery` is not a full-featured framework. In fact, it is not a framework at all. Thus is it not appropriate for tasks that require framework facilities, including:
* access to non-event data,
* writing of _art_/ROOT data files, and
* use of "services" that are framework-dependent.

## Running the demonstrations

There are three different demonstration codes:

1. `demo.cc` is a C++ main program which processes an input file and creates a ROOT histogram file. The provided `makefile` can be used to build the executable `demo` from `demo.cc`. `demo` expects one or more arguments, the names of the _art_/ROOT data files to be read.
2. `demo.C` is a ROOT macro intended to be invoked within an interactive ROOT session, and which creates a few histograms in memory. The macro function `demo` takes a single argument, the name of the _art_/ROOT data file to be processed. Assuming one has a local _art_/ROOT file named "sample.root", a sample session might be:
```bash
$ root -l
root [0] .x demo.C("sample.root")
Successfully opened file sample.root
Closing file, read 1232928 bytes in 119 transactions
Processed 100 events in an average of 396 microseconds/event
Total processing time (including file opening) was 326 milliseconds
root [1] nhits->Draw();
Info in <TCanvas::MakeDefCanvas>:  created default TCanvas with name c1
```
3. `demo.py` is a PyROOT program that processes an input file and creates a ROOT histogram file. It can be run with `python demo.py`

Both `demo.cc` and `demo.C` make use of the function `analyze`, declared in `analyze.hh` and defined in `analyze.cc`. The compiled program writes ROOT histograms to an output file named "histfile.root"; the ROOT macro creates ROOT histograms in memory.

The PyROOT program does as much of the same work as is supported using PyROOT. Note that the quality of PyROOT support varies by operating system, and over time.

