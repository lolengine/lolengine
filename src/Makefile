
COMMON = test-map.cpp \
      game.cpp tiler.cpp tileset.cpp scene.cpp \
      font.cpp layer.cpp map.cpp joystick.cpp
TEST_SRC = test-map.cpp sdlvideo.cpp $(COMMON)
EDITOR_SRC = editor.cpp gtkvideo.cpp $(COMMON)

all: test-map

test-map: $(TEST_SRC:%.cpp=%.o)
	g++ -g -Wall -O3 $^ -o $@ `pkg-config --libs sdl gl SDL_image`

editor-map: $(EDITOR_SRC:%.cpp=%.o)
	g++ -g -Wall -O3 $^ -o $@ `pkg-config --libs sdl gl SDL_image`

%.o: %.cpp
	g++ -g -Wall -O3 -c $^ -o $@ `pkg-config --cflags sdl gl SDL_image`

clean:
	rm -f *.o test-map

