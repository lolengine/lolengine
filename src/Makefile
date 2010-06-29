
SRC = test-map.cpp video.cpp

all: test-map

test-map: $(SRC:%.cpp=%.o)
	g++ -Wall -O3 $^ -o $@ `pkg-config --libs sdl gl SDL_image`

%.o: %.cpp
	g++ -Wall -O3 -c $^ -o $@ `pkg-config --cflags sdl gl SDL_image`

clean:
	rm -f *.o test-map

