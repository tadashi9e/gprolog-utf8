PREFIX=gprolog
MAIN=$(PREFIX).tex
DVINAME=$(PREFIX).dvi
PSNAME=$(PREFIX).ps
PDFNAME=$(PREFIX).pdf
HTMLNAME=$(PREFIX).html
CHMNAME=$(PREFIX).chm


# Images:
#    .eps are needed for latex (and hevea)
#    .pdf are needed for pdflatex
#    .png are needed for hevea
#
# There are rules to convert eps -> pdf and eps->png
# NB: convert (ImageMagick) is a superb tool for conversion
#
# For the logo: it is created from the .ico using convert
#  NB: gprolog.ico[5] means the 5th image of the .ico file (256x256)

IMG_EPS=logo.eps compil-scheme.eps debug-box.eps
IMG_PDF=$(IMG_EPS:.eps=.pdf)
IMG_PNG=$(IMG_EPS:.eps=.png)

TEX_SRC=$(MAIN) packages.tex macros.tex body.tex copyright.tex \
        cover.tex acknow.tex intro.tex use.tex debugger.tex \
        format-defs.tex direct-cc.tex pl-bips.tex fd-cstr.tex \
        c-interface.tex references.tex tbl-contents.tex the-index.tex \
        version_no.tex

TRACE=
#TRACE=-trace


.SUFFIXES:
.SUFFIXES: .html .hva .pdf .gif .png .eps .ps .dvi .aux .toc .idx .tex $(SUFFIXES)

help:
	@echo
	@echo '*** Please read the README file ***'
	@echo
	@echo 'make all        make DVI, PDF, PostScript, HTML and CHM versions'
	@echo 'make dvi        make DVI version        (needs LaTeX2e)'
	@echo 'make pdf        make PDF version        (needs pdflatex)'
	@echo 'make ps         make PostScript version (needs dvips)'
	@echo 'make html       make HTML versions      (needs HeVeA)'
	@echo 'make chm        make HTMLHelp version   (needs hhc - Win32 only)'
	@echo
	@echo 'make clean      remove all temporary files'
	@echo 'make distclean  remove all versions and auxiliary files'
	@echo 'make clean-all  remove all versions'
	@echo
	@echo 'make help       this help'
	@echo


.eps.pdf:
	epstopdf $<
#	convert $< $@
#	ps2pdf $< $@


#.eps.gif:
#	convert $< $@

.eps.png:
	convert $< $@



all: dvi pdf ps html chm


logo.eps: ../gprolog.ico
	convert -scale 150x150 $<[5] $@

# epstopdf does not handle transparency (needed for HeVeA in case the background is colored)
logo.png: ../gprolog.ico
	convert -scale 150x150 $<[5] $@

logo.pdf: ../gprolog.ico
	convert -scale 150x150 $<[5] $@



dvi: $(DVINAME)

$(DVINAME): $(TEX_SRC) $(IMG_EPS)
	./do_latex -dvi $(TRACE) $(MAIN)



pdf: $(PDFNAME)

$(PDFNAME): $(TEX_SRC)  $(IMG_PDF)
	./do_latex -pdf $(TRACE) $(MAIN)


ps: $(PSNAME)

$(PSNAME): $(DVINAME)
	dvips -o $(PSNAME) -D 300 $(DVINAME)



html: $(HTMLNAME) html_node/index.html

$(HTMLNAME): custom.hva $(TEX_SRC) $(IMG_PNG)
	hevea -O -s -exec xxdate.exe -fix custom.hva $(MAIN) -o $(HTMLNAME)

html_node/index.html: $(HTMLNAME)
	rm -f html_node/index.html html_node/$(PREFIX)*.html
	cp $(IMG_PNG) html_node/.
	(cd html_node && hacha -hrf -tocbis ../$(HTMLNAME) && ./hh_do_hhc_hhk $(PREFIX))

chm: $(CHMNAME)

$(CHMNAME): $(HTMLNAME) html_node/hh-$(PREFIX).hhp html_node/index.html
	-(cd html_node; hhc hh-$(PREFIX).hhp; mv -f $(CHMNAME) ..) 2>/dev/null || exit 0

clean:
	rm -f $(PREFIX).aux $(PREFIX).toc $(PREFIX).ind $(PREFIX).idx $(PREFIX).log  $(PREFIX).out $(PREFIX).ilg
	rm -f $(PREFIX).pdf.aux $(PREFIX).pdf.toc $(PREFIX).pdf.ind $(PREFIX).pdf.idx
	rm -f $(PREFIX).dvi.aux $(PREFIX).dvi.toc $(PREFIX).dvi.ind $(PREFIX).dvi.idx
	rm -rf $(PREFIX)*.htoc $(PREFIX)*.haux $(PREFIX)*.hind $(PREFIX)*.hrf


distclean: clean-aux


clean-all:
	rm -f $(DVINAME) $(PDFNAME) $(PSNAME) $(CHMNAME)
	rm -f $(HTMLNAME)
	(cd html_node ; rm -f *.hhc *.hhk *.css *.hrf *_motif.gif *.png *.html)

clean-aux:
	rm -f *.aux *.toc *.ind *.idx *.log *.haux *.htoc *.hidx *.hind *.out *.ilg *.hrf *.image.tex 

clean-full: clean-all clean-aux

clean-test:
	rm -f ?.dvi ?.pdf ?.html ?.ps ?.info ?.txt
	rm -f ?.aux ?.toc ?.idx ?.ind ?.log
	rm -f ?.haux ?.htoc ?.hidx ?.hind ?.out ?.ilg ?.hrf


# create compressed formats for DVI and HTML in 1 page and HTML 1 page / node

COPY_PATH=/tmp
DVITGZNAME=$(DVINAME).tar.gz
HTMLTGZNAME=$(HTMLNAME).tar.gz
HTMLNODETGZNAME=$(PREFIX).html_node.tar.gz

copy: $(DVINAME) $(PSNAME) $(PDFNAME) $(HTMLNAME) $(CHMNAME) names
	cp $(DVINAME) $(IMG_EPS) $(PSNAME) $(PDFNAME) $(HTMLNAME) $(IMG_PNG) $(CHMNAME) $(COPY_PATH)/.
	tar cf - $(DVINAME) $(IMG_EPS) | gzip > $(COPY_PATH)/$(DVITGZNAME)
	tar cf - $(HTMLNAME) $(IMG_PNG) | gzip > $(COPY_PATH)/$(HTMLTGZNAME)
	( cd html_node ; tar cf - index.html $(PREFIX)*.html $(PREFIX)*.css *.gif *.png| gzip > $(COPY_PATH)/$(HTMLNODETGZNAME) )

NAMES_FILE=$(COPY_PATH)/doc_names

names:
	@echo '# file generated by doc/Makefile'    >$(NAMES_FILE)
	@echo "PREFIX=$(PREFIX)"                   >>$(NAMES_FILE)
	@echo "DVINAME=$(DVINAME)"                 >>$(NAMES_FILE)
	@echo "PDFNAME=$(PDFNAME)"                 >>$(NAMES_FILE)
	@echo "PSNAME=$(PSNAME)"                   >>$(NAMES_FILE)
	@echo "HTMLNAME=$(HTMLNAME)"               >>$(NAMES_FILE)
	@echo "CHMNAME=$(CHMNAME)"                 >>$(NAMES_FILE)
	@echo "IMG_EPS='$(IMG_EPS)'"               >>$(NAMES_FILE)
	@echo "IMG_PNG='$(IMG_PNG)'"               >>$(NAMES_FILE)
	@echo "DVITGZNAME=$(DVITGZNAME)"           >>$(NAMES_FILE)
	@echo "HTMLTGZNAME=$(HTMLTGZNAME)"         >>$(NAMES_FILE)
	@echo "HTMLNODETGZNAME=$(HTMLNODETGZNAME)" >>$(NAMES_FILE)

for-release: all dist-dvi dist-html names
