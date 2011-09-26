
AM_CPPFLAGS = -I$(top_srcdir)/src

all-local: $(noinst_PROGRAMS)
	test x$(MAKE_FSELF) = xno || make_fself pi$(EXEEXT) pi.self
	test x$(MAKE_FSELF) = xno || make_fself remez$(EXEEXT) remez.self

CLEANFILES = $(noinst_PROGRAMS:%$(EXEEXT)=%.self) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.elf) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.exe)

noinst_PROGRAMS = pi remez

pi_SOURCES = pi.cpp
pi_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
pi_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
pi_DEPENDENCIES = $(top_builddir)/src/liblol.a

remez_SOURCES = remez.cpp
remez_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
remez_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
remez_DEPENDENCIES = $(top_builddir)/src/liblol.a

