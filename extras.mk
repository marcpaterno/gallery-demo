all: deps.pdf gallery.dot gallery.pdf

deps.pdf: deps.dot
	dot -Tpdf $< | pdfcrop - $@

gallery.pdf: gallery.dot
	dot -Tpdf $< | pdfcrop - $@

gallery.dot:
	ups-d2dot gallery v1_02_02 -q e10:nu:prof | tred > $@

