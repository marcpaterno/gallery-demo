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
#for x in ['GALLERY_INC', 'CANVAS_INC', 'FHICLCPP_INC', 'CETLIB_INC', 'BOOST_INC' ]:
#        ROOT.gInterpreter.AddIncludePath(os.environ.get(x))

print "Reading headers..."
read_header('gallery/ValidHandle.h')

print "Instantiating member templates..."
provide_get_valid_handle('std::vector<simb::MCTruth>')
provide_get_valid_handle('std::vector<recob::Vertex>')

print "Preparing before event loop..."
mctruths_tag = ROOT.art.InputTag("generator");
vertex_tag   = ROOT.art.InputTag("linecluster");

# Make histograms before we open the art/ROOT file, or the file ends
# up owning the histograms.
histfile = ROOT.TFile("hist.root", "RECREATE")
npart_hist = ROOT.TH1F("npart", "Number of particles per MCTruth", 51, -0.5, 50.5)
nvert_hist = ROOT.TH1F("nvert", "Number of vertices per event", 51, -0.5, 50.5)

ROOT.gStyle.SetOptStat("nemo") # Print name, entries, and overflows.

filenames = ROOT.vector(ROOT.string)(1, "dune.root")

print "Creating event object ..."
ev = ROOT.gallery.Event(filenames)

# Capture the functions that will get ValidHandles
get_mctruths = ev.getValidHandle(ROOT.vector(ROOT.simb.MCTruth))
get_vertices = ev.getValidHandle(ROOT.vector(ROOT.recob.Vertex))

print "Entering event loop..."
while (not ev.atEnd()) :
        mctruths = get_mctruths(mctruths_tag)
        nparts = mctruths.product()[0].NParticles()
        npart_hist.Fill(nparts)

        vertices = get_vertices(vertex_tag);
        #print 'Number of showers = {}'.format(showers.product().size())
        # Create FindMany object from the showers handle.
        # Loop over all showers; for each, count how many clusters.
        # fill the nclus_hist histogram for each value of number of clusters per shower.
        
        ev.next()

print "Writing histograms..."
histfile.Write()




