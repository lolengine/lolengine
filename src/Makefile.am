
noinst_PROGRAMS = test-map editor

noinst_LIBRARIES = libcommon.a

libcommon_a_SOURCES = \
    core.h matrix.h game.cpp game.h tiler.cpp tiler.h dict.cpp dict.h \
    scene.cpp scene.h font.cpp font.h layer.cpp layer.h map.cpp map.h \
    entity.cpp entity.h ticker.cpp ticker.h tileset.cpp tileset.h \
    forge.cpp forge.h video.cpp video.h timer.cpp timer.h bitfield.h \
    profiler.cpp profiler.h input.h input.cpp world.cpp world.h \
    debugfps.cpp debugfps.h debugsprite.cpp debugsprite.h \
    debugrecord.cpp debugrecord.h
libcommon_a_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image`

test_map_SOURCES = test-map.cpp sdlinput.cpp sdlinput.h
test_map_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image` @PIPI_CFLAGS@
test_map_LDADD = libcommon.a
test_map_LDFLAGS = `pkg-config --libs sdl gl SDL_image` @PIPI_LIBS@

editor_SOURCES = gtk/editor.cpp gtk/glmapview.cpp gtk/glmapview.h \
                 mapviewer.cpp mapviewer.h
editor_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image gtk+-2.0 gtkgl-2.0`
editor_LDADD = libcommon.a
editor_LDFLAGS = `pkg-config --libs sdl gl gtk+-2.0 gtkgl-2.0 SDL_image`
EXTRA_DIST = gtk/editor.xml

