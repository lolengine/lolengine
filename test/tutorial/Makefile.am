
AM_CPPFLAGS = -I$(top_srcdir)/src

all-local: $(noinst_PROGRAMS)
	test x$(MAKE_FSELF) = xno || make_fself tut01$(EXEEXT) tut01.self
	test x$(MAKE_FSELF) = xno || make_fself tut02$(EXEEXT) tut02.self
	test x$(MAKE_FSELF) = xno || make_fself tut03$(EXEEXT) tut03.self

CLEANFILES = $(noinst_PROGRAMS:%$(EXEEXT)=%.self) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.elf) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.exe)

noinst_PROGRAMS = tut01 tut02 tut03

tut01_SOURCES = tut01.cpp
tut01_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
tut01_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
tut01_DEPENDENCIES = $(top_builddir)/src/liblol.a

tut02_SOURCES = tut02.cpp
tut02_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
tut02_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
tut02_DEPENDENCIES = $(top_builddir)/src/liblol.a

tut03_SOURCES = tut03.cpp
tut03_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
tut03_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
tut03_DEPENDENCIES = $(top_builddir)/src/liblol.a

