
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = math sandbox xolotl

bench: benchsuite$(EXEEXT)
	./benchsuite$(EXEEXT)

noinst_PROGRAMS = benchsuite $(testsuite) btphystest nacl_phystest meshviewer

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
    unit/rand.cpp unit/thread.cpp unit/camera.cpp
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
    physics/bulletcharactercontroller.cpp physics/bulletcharactercontroller.h \
    \
    front_camera_sprite.lolfx
btphystest_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/bullet \
    -DHAVE_PHYS_USE_BULLET
btphystest_DEPENDENCIES = @LOL_DEPS@
btphystest_LDFLAGS = $(AM_LDFLAGS)
if USE_EMSCRIPTEN
btphystest_LDFLAGS += \
    --preload-file data/CatsSheet.png \
    -s TOTAL_MEMORY=$(shell expr 32 '*' 1024 '*' 1024)
endif

nacl_phystest_SOURCES = \
    nacl_phystest.cpp nacl_phystest.h
nacl_phystest_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/bullet \
    -DHAVE_PHYS_USE_BULLET
nacl_phystest_DEPENDENCIES = @LOL_DEPS@
nacl_phystest_LDFLAGS = $(AM_LDFLAGS)

meshviewer_SOURCES = meshviewer.cpp \
    shinymvtexture.lolfx shinyfur.lolfx
meshviewer_CPPFLAGS = $(AM_CPPFLAGS)
meshviewer_DEPENDENCIES = @LOL_DEPS@
meshviewer_LDFLAGS = $(AM_LDFLAGS) -s
if USE_EMSCRIPTEN
meshviewer_LDFLAGS += -s EXPORTED_FUNCTIONS="['_main', '_C_Send']"
endif

