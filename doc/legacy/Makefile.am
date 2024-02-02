
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = math sandbox

bench: benchsuite$(EXEEXT)
	./benchsuite$(EXEEXT)

if BUILD_LEGACY
noinst_PROGRAMS = bluenoise benchsuite simplex
if LOL_USE_GL
if LOL_USE_BULLET
noinst_PROGRAMS += btphystest
endif
endif
endif

bluenoise_SOURCES = bluenoise.cpp
bluenoise_CPPFLAGS = $(AM_CPPFLAGS)
bluenoise_LDFLAGS = @LOL_DEPS@

benchsuite_SOURCES = benchsuite.cpp \
    benchmark/vector.cpp benchmark/half.cpp benchmark/real.cpp
benchsuite_CPPFLAGS = $(AM_CPPFLAGS)
benchsuite_LDFLAGS = @LOL_DEPS@

btphystest_SOURCES = \
    btphystest.cpp btphystest.h physicobject.h \
    physics/easyphysics.cpp physics/easyphysics.h \
    physics/lolbtphysicsintegration.h physics/lolphysics.h \
    physics/easycharactercontroller.cpp physics/easycharactercontroller.h \
    physics/easyconstraint.cpp physics/easyconstraint.h \
    physics/bulletcharactercontroller.cpp physics/bulletcharactercontroller.h \
    \
    front_camera_sprite.lolfx
btphystest_CPPFLAGS = $(AM_CPPFLAGS)
btphystest_LDFLAGS = $(AM_LDFLAGS) @LOL_DEPS@
if LOL_USE_EMSCRIPTEN
btphystest_LDFLAGS += \
    --preload-file data/CatsSheet.png \
    -s TOTAL_MEMORY=$(shell expr 32 '*' 1024 '*' 1024)
endif

simplex_SOURCES = simplex.cpp
simplex_CPPFLAGS = $(AM_CPPFLAGS)
simplex_LDFLAGS = @LOL_DEPS@

