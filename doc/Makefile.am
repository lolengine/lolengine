include $(top_srcdir)/build/autotools/common.am

SUBDIRS =
SUBDIRS += tutorial
SUBDIRS += examples
SUBDIRS += legacy

EXTRA_DIST += doxygen.cfg.in doxygen.css footer.html header.html \
              $(man_MANS) $(doxygen_DOX)
CLEANFILES += doxygen.log stamp-latex stamp-doxygen

doxygen_DOX = lolengine.dox
if BUILD_DOCUMENTATION
man_MANS = lolengine.1
endif

if BUILD_DOCUMENTATION
htmldoc_DATA = html/doxygen.css
htmldocdir = $(datadir)/doc/lolengine-dev/html
if LOL_USE_LATEX
pdfdoc_DATA = latex/lolengine.pdf
pdfdocdir = $(datadir)/doc/lolengine-dev/pdf
endif
endif

html/doxygen.css html/doxygen.png: stamp-doxygen
latex/lolengine.pdf: stamp-latex

stamp-doxygen: $(doxygen_DOX)
if BUILD_DOCUMENTATION
	doxygen doxygen.cfg 2>&1 | tee doxygen.log
	touch stamp-doxygen
endif

stamp-latex: stamp-doxygen
if BUILD_DOCUMENTATION
if LOL_USE_LATEX
	rm -f latex/lolengine.tex latex/lolengine.pdf
	$(SED) -i -e 's/setlength{/renewcommand{/' latex/refman.tex
	$(SED) -i -e 's/.*usepackage.*times.*//' latex/refman.tex
	$(SED) -i -e 's/latex refman/latex -file-line-error -halt-on-error refman/' latex/Makefile
	cd latex && $(MAKE) $(AM_CFLAGS) refman.pdf || (cat refman.log; exit 1)
	mv latex/refman.pdf latex/lolengine.pdf
	touch stamp-latex
endif
endif

clean-local:
	-rm -Rf html latex man

install-data-local:
if BUILD_DOCUMENTATION
	mkdir -p $(DESTDIR)$(datadir)/doc
	$(mkinstalldirs) $(DESTDIR)$(datadir)/doc/lolengine-dev/html
	cp `find html -name '*.html' -o -name '*.gif' -o -name '*.png'` \
	  $(DESTDIR)$(datadir)/doc/lolengine-dev/html
	$(mkinstalldirs) $(DESTDIR)$(mandir)/man3
	for man in $$(find man -name '*.3lol'); do \
	  sed -e 's/man3lol/man3/g' -e "s/fC'\([a-zA-Z0-9]*\)'/fC\"\1\"/g" \
	    $$man >$(DESTDIR)$(mandir)/man3/$${man##*/}; \
	done
	$(mkinstalldirs) $(DESTDIR)$(mandir)/man1
endif

uninstall-local:
	rmdir $(DESTDIR)$(datadir)/doc/lolengine-dev/html 2>/dev/null || true
	rmdir $(DESTDIR)$(datadir)/doc/lolengine-dev 2>/dev/null || true
	rmdir $(DESTDIR)$(datadir)/doc 2>/dev/null || true

