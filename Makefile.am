
SUBDIRS = src test deushax monsterz tools gfx
DIST_SUBDIRS = $(SUBDIRS)

EXTRA_DIST = bootstrap build-linux build-mingw build-mingw64 build-ps3 \
             build-nacl32 build-nacl64 build/lol-build
AUTOMAKE_OPTIONS = dist-bzip2

bench:
	cd test && $(MAKE) $(AM_MAKEFLAGS) $@
.PHONY: bench

dist: lolunit-$(LOLUNIT_VERSION).tar.gz
dist: lolremez-$(LOLREMEZ_VERSION).tar.gz

.PHONY: lolunit-$(LOLUNIT_VERSION).tar.gz
lolunit-$(LOLUNIT_VERSION).tar.gz:
	rm -rf lolunit-$(LOLUNIT_VERSION)
	mkdir lolunit-$(LOLUNIT_VERSION)
	mkdir lolunit-$(LOLUNIT_VERSION)/lol
	cp $(top_srcdir)/src/lol/unit.h lolunit-$(LOLUNIT_VERSION)/lol
	tar cvzf lolunit-$(LOLUNIT_VERSION).tar.gz lolunit-$(LOLUNIT_VERSION)
	rm -rf lolunit-$(LOLUNIT_VERSION)

.PHONY: lolremez-$(LOLREMEZ_VERSION).tar.gz
lolremez-$(LOLREMEZ_VERSION).tar.gz:
	rm -rf lolremez-$(LOLREMEZ_VERSION)
	mkdir lolremez-$(LOLREMEZ_VERSION)
	mkdir lolremez-$(LOLREMEZ_VERSION)/lol
	mkdir lolremez-$(LOLREMEZ_VERSION)/lol/math
	cat $(top_srcdir)/test/math/remez.cpp | sed '/#if/,/^$$/d' > \
	  lolremez-$(LOLREMEZ_VERSION)/remez.cpp
	cat $(top_srcdir)/src/real.cpp | sed 's@core.h@lol/math/real.h@' | sed '/#if/,/^$$/d' > \
	  lolremez-$(LOLREMEZ_VERSION)/real.cpp
	cp $(top_srcdir)/test/math/NEWS.txt \
	   $(top_srcdir)/test/math/lolremez.sln \
	   $(top_srcdir)/test/math/remez.vcxproj \
	   $(top_srcdir)/test/math/remez.vcxproj.filters \
	  lolremez-$(LOLREMEZ_VERSION)/
	cp $(top_srcdir)/src/lol/math/real.h \
	   $(top_srcdir)/src/lol/math/matrix.h \
	   $(top_srcdir)/src/lol/math/remez.h \
	  lolremez-$(LOLREMEZ_VERSION)/lol/math/
	printf 'remez: real.cpp remez.cpp\n' > lolremez-$(LOLREMEZ_VERSION)/Makefile
	printf '\t$$(CXX) -I. -O2 -g -ggdb $$^ -o $$@\n' >> lolremez-$(LOLREMEZ_VERSION)/Makefile
	printf 'clean:\n' >> lolremez-$(LOLREMEZ_VERSION)/Makefile
	printf '\trm -f remez\n' >> lolremez-$(LOLREMEZ_VERSION)/Makefile
	tar cvzf lolremez-$(LOLREMEZ_VERSION).tar.gz lolremez-$(LOLREMEZ_VERSION)
	rm -rf lolremez-$(LOLREMEZ_VERSION)

