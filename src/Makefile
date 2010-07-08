
SRC = test-map.cpp \
      game.cpp video.cpp tiler.cpp tileset.cpp scene.cpp layer.cpp map.cpp

all: test-map

test-map: $(SRC:%.cpp=%.o)
	g++ -g -Wall -O3 $^ -o $@ `pkg-config --libs sdl gl SDL_image`

%.o: %.cpp
	g++ -g -Wall -O3 -c $^ -o $@ `pkg-config --cflags sdl gl SDL_image`

clean:
	rm -f *.o test-map

