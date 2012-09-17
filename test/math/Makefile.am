
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

all-local: $(PROGRAMS) $(noinst_PROGRAMS)
	for x in $(^:%$(EXEEXT)=%); do \
	  test x$(MAKE_FSELF) = xno || make_fself $${x}$(EXEEXT) $${x}.self; \
	done

EXTRA_DIST = NEWS.txt lolremez.sln remez.vcxproj remez.vcxproj.filters

CLEANFILES = $(noinst_PROGRAMS:%$(EXEEXT)=%.self) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.elf) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.exe)

noinst_PROGRAMS = pi poly remez

pi_SOURCES = pi.cpp
pi_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
pi_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
pi_DEPENDENCIES = $(top_builddir)/src/liblol.a

poly_SOURCES = poly.cpp
poly_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
poly_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
poly_DEPENDENCIES = $(top_builddir)/src/liblol.a

remez_SOURCES = remez.cpp
remez_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
remez_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
remez_DEPENDENCIES = $(top_builddir)/src/liblol.a

