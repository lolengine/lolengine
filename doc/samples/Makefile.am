
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = math sandbox

bench: benchsuite$(EXEEXT)
	./benchsuite$(EXEEXT)

noinst_PROGRAMS = benchsuite btphystest nacl_phystest meshviewer \
    simplex

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
    shinymvtexture.lolfx shinyfur.lolfx \
    scenesetup.cpp scenesetup.h \
    scenesetup-compiler.cpp scenesetup-compiler.h \
    generated/scenesetup-scanner.cpp generated/scenesetup-scanner.h \
    generated/scenesetup-parser.cpp
meshviewer_CPPFLAGS = $(AM_CPPFLAGS)
meshviewer_DEPENDENCIES = @LOL_DEPS@
meshviewer_LDFLAGS = $(AM_LDFLAGS)
if USE_EMSCRIPTEN
meshviewer_LDFLAGS += -s EXPORTED_FUNCTIONS="['_main', '_C_Send']"
endif

simplex_SOURCES = simplex.cpp
simplex_CPPFLAGS = $(AM_CPPFLAGS)
simplex_DEPENDENCIES = @LOL_DEPS@

