import os, sys
import ROOT

print "Starting demo..."

# Some functions that I find useful to reduce error-prone typing.
def read_header(h):
        """Make the ROOT C++ jit compiler read the specified header."""
        ROOT.gROOT.ProcessLine('#include "%s"' % h)

def provide_get_valid_handle(klass):
        """Make the ROOT C++ jit compiler instantiate the
           Event::getValidHandle member template for template
           parameter klass."""
        ROOT.gROOT.ProcessLine('template gallery::ValidHandle<%(name)s> gallery::Event::getValidHandle<%(name)s>(art::InputTag const&) const;' % {'name' : klass})


# Now for the script...

print "Reading headers..."
read_header('gallery/ValidHandle.h')

print "Instantiating member templates..."
provide_get_valid_handle('std::vector<simb::MCTruth>')
provide_get_valid_handle('std::vector<recob::Vertex>')

print "Preparing before event loop..."
mctruths_tag = ROOT.art.InputTag("generator");
vertex_tag   = ROOT.art.InputTag("linecluster");
filenames = ROOT.vector(ROOT.string)(1, "dune.root")

# Make histograms before we open the art/ROOT file, or the file ends
# up owning the histograms.
histfile = ROOT.TFile("hist.root", "RECREATE")
npart_hist = ROOT.TH1F("npart", "Number of particles per MCTruth", 51, -0.5, 50.5)
nvert_hist = ROOT.TH1F("nclus", "Number of clusters per vertex", 51, -0.5, 50.5)

print "Creating event object ..."
ev = ROOT.gallery.Event(filenames)

# Capture the functions that will get ValidHandles. This avoids some
# inefficiency in constructing the function objects many times.
get_mctruths = ev.getValidHandle(ROOT.vector(ROOT.simb.MCTruth))
get_vertices = ev.getValidHandle(ROOT.vector(ROOT.recob.Vertex))
findMaker = ROOT.gallery.FindMaker()

print "Entering event loop..."
while (not ev.atEnd()) :
        mctruths = get_mctruths(mctruths_tag)
        if (not mctruths.empty()):
                npart_hist.Fill(mctruths.product()[0].NParticles())

        # The Assns<> involved in demo.cc appears to be inaccessible
        # from PyROOT at this time, because of PyROOT's incomplete
        # object model.
        ev.next()

print "Writing histograms..."
histfile.Write()
