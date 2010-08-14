
noinst_PROGRAMS = test-map editor

noinst_LIBRARIES = libcommon.a

libcommon_a_SOURCES = \
    game.cpp game.h tiler.cpp tiler.h tileset.cpp tileset.h \
    scene.cpp scene.h font.cpp font.h layer.cpp layer.h map.cpp map.h \
    joystick.cpp joystick.h asset.cpp asset.h ticker.cpp ticker.h \
    video.cpp video.h
libcommon_a_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image`

test_map_SOURCES = test-map.cpp sdlinput.cpp sdlinput.h
test_map_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image`
test_map_LDADD = libcommon.a
test_map_LDFLAGS = `pkg-config --libs sdl gl SDL_image`

editor_SOURCES = gtk/editor.cpp gtk/gtkvideo.cpp gtk/gtkvideo.h
editor_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image gtk+-2.0 gtkgl-2.0`
editor_LDADD = libcommon.a
editor_LDFLAGS = `pkg-config --libs sdl gl gtk+-2.0 gtkgl-2.0 SDL_image`

