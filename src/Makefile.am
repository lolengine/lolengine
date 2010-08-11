
noinst_PROGRAMS = test-map editor

noinst_LIBRARIES = libcommon.a

libcommon_a_SOURCES = \
    game.cpp tiler.cpp tileset.cpp scene.cpp \
    font.cpp layer.cpp map.cpp joystick.cpp \
    asset.h asset.cpp ticker.h ticker.cpp
libcommon_a_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image`

test_map_SOURCES = test-map.cpp sdlvideo.cpp
test_map_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image`
test_map_LDADD = libcommon.a
test_map_LDFLAGS = `pkg-config --libs sdl gl SDL_image`

editor_SOURCES = editor.cpp gtkvideo.cpp
editor_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image gtk+-2.0 gtkgl-2.0`
editor_LDADD = libcommon.a
editor_LDFLAGS = `pkg-config --libs sdl gl gtk+-2.0 gtkgl-2.0 SDL_image`

