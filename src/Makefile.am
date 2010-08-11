
noinst_PROGRAMS = test-map editor

noinst_LIBRARIES = libcommon.a

libcommon_a_SOURCES = \
    game.cpp tiler.cpp tileset.cpp scene.cpp \
    font.cpp layer.cpp map.cpp joystick.cpp
libcommon_a_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image`

test_map_SOURCES = test-map.cpp sdlvideo.cpp
test_map_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image`
test_map_LDADD = libcommon.a
test_map_LDFLAGS = `pkg-config --libs sdl gl SDL_image`

editor_SOURCES = editor.cpp gtkvideo.cpp
editor_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image gtk+-2.0 gtkgl-2.0`
editor_LDADD = libcommon.a
editor_LDFLAGS = `pkg-config --libs sdl gl gtk+-2.0 gtkgl-2.0 SDL_image`

#COMMON = $(libcommon_a_SOURCES)
#
#TEST_SRC = test-map.cpp sdlvideo.cpp $(COMMON)
#EDITOR_SRC = editor.cpp gtkvideo.cpp $(COMMON)
#
#CPPFLAGS = `pkg-config --cflags sdl gl SDL_image gtk+-2.0 gtkgl-2.0`
#CXXFLAGS = -g -O3 -Wall \
#           -Wpointer-arith -Wcast-align -Wcast-qual -Wshadow -Wsign-compare
#
#test-map: $(TEST_SRC:%.cpp=%.o)
#	g++ $(CXXFLAGS) $^ -o $@ `pkg-config --libs sdl gl SDL_image`
#
#editor: $(EDITOR_SRC:%.cpp=%.o)
#	g++ $(CXXFLAGS) $^ -o $@ `pkg-config --libs sdl gl gtk+-2.0 gtkgl-2.0 SDL_image`
#
#%.o: %.cpp
#	g++ -c $^ -o $@ $(CXXFLAGS) $(CPPFLAGS)
#
#clean:
#	rm -f *.o test-map editor

