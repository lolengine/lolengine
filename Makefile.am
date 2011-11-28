
SUBDIRS = src test deushax monsterz tools gfx
DIST_SUBDIRS = $(SUBDIRS)

EXTRA_DIST = bootstrap build-linux build-mingw build-mingw64 build-ps3 \
             build-nacl32 build-nacl64 build/lol-build
AUTOMAKE_OPTIONS = dist-bzip2

bench:
	cd test && $(MAKE) $(AM_MAKEFLAGS) $@
.PHONY: bench

dist: lolunit-$(LOLUNIT_VERSION).tar.gz

lolunit-$(LOLUNIT_VERSION).tar.gz:
	rm -rf lolunit-$(LOLUNIT_VERSION)
	mkdir lolunit-$(LOLUNIT_VERSION)
	mkdir lolunit-$(LOLUNIT_VERSION)/lol
	cp $(top_srcdir)/src/lol/unit.h lolunit-$(LOLUNIT_VERSION)/lol
	tar cvzf lolunit-$(LOLUNIT_VERSION).tar.gz lolunit-$(LOLUNIT_VERSION)
	rm -rf lolunit-$(LOLUNIT_VERSION)

