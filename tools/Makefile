
all: make-font

make-font: make-font.o
	g++ -g -Wall -O3 $^ -o $@ `pkg-config --libs caca`

%.o: %.cpp
	g++ -g -Wall -O3 -c $^ -o $@ `pkg-config --cflags caca`

clean:
	rm -f *.o make-font

