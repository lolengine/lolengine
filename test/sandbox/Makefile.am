
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

all-local: $(noinst_PROGRAMS)
	test x$(MAKE_FSELF) = xno || make_fself sample$(EXEEXT) sample.self

CLEANFILES = $(noinst_PROGRAMS:%$(EXEEXT)=%.self) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.elf) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.exe)

noinst_PROGRAMS = sample

sample_SOURCES = sample.cpp
sample_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
sample_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
sample_DEPENDENCIES = $(top_builddir)/src/liblol.a

