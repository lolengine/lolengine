
SUBDIRS = math

AM_CPPFLAGS = -I$(top_srcdir)/src

bench: benchsuite$(EXEEXT)
	./benchsuite$(EXEEXT)

all-local: $(noinst_PROGRAMS)
	test x$(MAKE_FSELF) = xno || make_fself quad$(EXEEXT) quad.self
	test x$(MAKE_FSELF) = xno || make_fself sandbox$(EXEEXT) sandbox.self
	test x$(MAKE_FSELF) = xno || make_fself benchsuite$(EXEEXT) benchsuite.self
	test x$(MAKE_FSELF) = xno || make_fself testsuite$(EXEEXT) testsuite.self

CLEANFILES = $(noinst_PROGRAMS:%$(EXEEXT)=%.self) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.elf) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.exe)

EXTRA_DIST = data/gradient.png

noinst_PROGRAMS = quad sandbox benchsuite testsuite

TESTS = testsuite

testsuite_SOURCES = testsuite.cpp \
    unit/matrix.cpp unit/half.cpp unit/trig.cpp unit/build.cpp \
    unit/real.cpp unit/image.cpp
testsuite_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
testsuite_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
testsuite_DEPENDENCIES = $(top_builddir)/src/liblol.a

benchsuite_SOURCES = benchsuite.cpp \
    benchmark/matrix.cpp benchmark/half.cpp benchmark/trig.cpp \
    benchmark/real.cpp
benchsuite_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
benchsuite_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
benchsuite_DEPENDENCIES = $(top_builddir)/src/liblol.a

quad_SOURCES = debug/quad.cpp
quad_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
quad_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
quad_DEPENDENCIES = $(top_builddir)/src/liblol.a

sandbox_SOURCES = debug/sandbox.cpp
sandbox_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
sandbox_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
sandbox_DEPENDENCIES = $(top_builddir)/src/liblol.a

