
include $(top_srcdir)/build/autotools/common.am

noinst_PROGRAMS = meshviewer

meshviewer_SOURCES = meshviewer.cpp meshviewer.h \
    scenesetup.cpp scenesetup.h \
    shinymvtexture.lolfx shinyfur.lolfx
meshviewer_CPPFLAGS = $(AM_CPPFLAGS)
meshviewer_DEPENDENCIES = @LOL_DEPS@
meshviewer_LDFLAGS = $(AM_LDFLAGS)
if USE_EMSCRIPTEN
meshviewer_LDFLAGS += -s EXPORTED_FUNCTIONS="['_main', '_C_Send']"
endif

