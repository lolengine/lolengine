
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

all-local: $(noinst_PROGRAMS)
	test x$(MAKE_FSELF) = xno || make_fself xolotl$(EXEEXT) xolotl.self

CLEANFILES = $(noinst_PROGRAMS:%$(EXEEXT)=%.self) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.elf) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.exe)

noinst_PROGRAMS = xolotl

xolotl_SOURCES = xolotl.cpp xolotl.h
xolotl_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
xolotl_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
xolotl_DEPENDENCIES = $(top_builddir)/src/liblol.a

