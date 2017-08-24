
include $(top_srcdir)/build/autotools/common.am

if BUILD_SAMPLES
if LOL_USE_GL
noinst_PROGRAMS = meshviewer
endif
endif

meshviewer_SOURCES = meshviewer.cpp meshviewer.h \
    scenesetup.cpp scenesetup.h \
    shinymvtexture.lolfx shinyfur.lolfx
meshviewer_CPPFLAGS = $(AM_CPPFLAGS)
meshviewer_DEPENDENCIES = @LOL_DEPS@
meshviewer_LDFLAGS = $(AM_LDFLAGS)
if LOL_USE_EMSCRIPTEN
meshviewer_LDFLAGS += -s EXPORTED_FUNCTIONS="['_main', '_C_Send']"
endif

