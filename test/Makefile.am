
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = math sandbox xolotl

bench: benchsuite$(EXEEXT)
	./benchsuite$(EXEEXT)

noinst_PROGRAMS = benchsuite $(testsuite) btphystest meshviewer

TESTS = $(testsuite)

# Conditionally built for now because of STLport issues
if !USE_ANDROID
testsuite = testsuite
endif

testsuite_SOURCES = testsuite.cpp \
    unit/vector.cpp unit/matrix.cpp unit/half.cpp unit/trig.cpp \
    unit/build.cpp unit/real.cpp unit/image.cpp unit/quat.cpp unit/cmplx.cpp \
    unit/array.cpp unit/rotation.cpp unit/string.cpp unit/map.cpp \
    unit/color.cpp unit/atomic.cpp unit/interp.cpp unit/box.cpp \
    unit/rand.cpp
testsuite_CPPFLAGS = $(AM_CPPFLAGS)
testsuite_DEPENDENCIES = @LOL_DEPS@
noinst_DATA = data/gradient.png

benchsuite_SOURCES = benchsuite.cpp \
    benchmark/vector.cpp benchmark/half.cpp benchmark/trig.cpp \
    benchmark/real.cpp
benchsuite_CPPFLAGS = $(AM_CPPFLAGS)
benchsuite_DEPENDENCIES = @LOL_DEPS@

btphystest_SOURCES = \
    btphystest.cpp btphystest.h physicobject.h \
    physics/easyphysics.cpp physics/easyphysics.h \
    physics/lolbtphysicsintegration.h physics/lolphysics.h \
    physics/easycharactercontroller.cpp physics/easycharactercontroller.h \
    physics/easyconstraint.cpp physics/easyconstraint.h \
    physics/bulletcharactercontroller.cpp physics/bulletcharactercontroller.h
btphystest_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/bullet \
    -DHAVE_PHYS_USE_BULLET
btphystest_DEPENDENCIES = @LOL_DEPS@

meshviewer_SOURCES = meshviewer.cpp \
    shinymvtexture.lolfx shinyfur.lolfx
meshviewer_CPPFLAGS = $(AM_CPPFLAGS)
meshviewer_DEPENDENCIES = @LOL_DEPS@

