include $(top_srcdir)/build/autotools/common.am

EXTRA_DIST += doxygen.cfg.in doxygen.css footer.html header.html \
              $(man_MANS) $(doxygen_DOX)
CLEANFILES += doxygen.log stamp-latex stamp-doxygen

doxygen_DOX = lolengine.dox
man_MANS = lolengine.1

if BUILD_DOCUMENTATION
htmldoc_DATA = html/doxygen.css
htmldocdir = $(datadir)/doc/lolengine-dev/html
if USE_LATEX
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
if USE_LATEX
	rm -f latex/lolengine.tex latex/lolengine.pdf
	mv latex/refman.tex latex/lolengine.tex
	sed 's/setlength{/renewcommand{/' latex/lolengine.tex \
	  | sed 's/.*usepackage.*times.*//' > latex/refman.tex
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
endif
	$(mkinstalldirs) $(DESTDIR)$(mandir)/man1

uninstall-local:
	rm -f $(DESTDIR)$(datadir)/doc/libcucul-dev
	rmdir $(DESTDIR)$(datadir)/doc 2>/dev/null || true

