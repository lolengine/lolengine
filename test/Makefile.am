
SUBDIRS = math sandbox xolotl

AM_CPPFLAGS = -I$(top_srcdir)/src

bench: benchsuite$(EXEEXT)
	./benchsuite$(EXEEXT)

all-local: $(noinst_PROGRAMS)
	test x$(MAKE_FSELF) = xno || make_fself quad$(EXEEXT) quad.self
	test x$(MAKE_FSELF) = xno || make_fself benchsuite$(EXEEXT) benchsuite.self
	test x$(MAKE_FSELF) = xno || make_fself testsuite$(EXEEXT) testsuite.self

CLEANFILES = $(noinst_PROGRAMS:%$(EXEEXT)=%.self) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.elf) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.exe)

EXTRA_DIST = data/gradient.png

noinst_PROGRAMS = quad benchsuite $(testsuite) btphystest

TESTS = $(testsuite)

# Conditionally built for now because of STLport issues
if !USE_ANDROID
testsuite = testsuite
endif

testsuite_SOURCES = testsuite.cpp \
    unit/vector.cpp unit/matrix.cpp unit/half.cpp unit/trig.cpp \
    unit/build.cpp unit/real.cpp unit/image.cpp unit/quat.cpp unit/cmplx.cpp \
    unit/array.cpp unit/rotation.cpp
testsuite_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
testsuite_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
testsuite_DEPENDENCIES = $(top_builddir)/src/liblol.a

benchsuite_SOURCES = benchsuite.cpp \
    benchmark/vector.cpp benchmark/half.cpp benchmark/trig.cpp \
    benchmark/real.cpp
benchsuite_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
benchsuite_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
benchsuite_DEPENDENCIES = $(top_builddir)/src/liblol.a

quad_SOURCES = debug/quad.cpp
quad_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
quad_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
quad_DEPENDENCIES = $(top_builddir)/src/liblol.a

btphystest_SOURCES = BtPhysTest.cpp BtPhysTest.h \
    Physics/EasyPhysics.cpp Physics/EasyPhysics.h \
    Physics/LolBtPhysicsIntegration.h Physics/LolPhysics.h
btphystest_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@ -I$(top_builddir)/src/bullet \
    -DHAVE_PHYS_USE_BULLET
btphystest_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
btphystest_DEPENDENCIES = $(top_builddir)/src/liblol.a

