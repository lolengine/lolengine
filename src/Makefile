
all: test-map

test-map: test-map.cpp
	g++ -Wall -O3 $^ -o $@ `pkg-config --cflags --libs sdl gl SDL_image`

clean:
	rm -f test-map

