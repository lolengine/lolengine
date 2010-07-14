
all:
	cd src && $(MAKE) all
	cd tools && $(MAKE) all
	cd art && $(MAKE) all
	cd gfx && $(MAKE) all

clean:
	cd gfx && $(MAKE) clean
	cd art && $(MAKE) clean
	cd tools && $(MAKE) clean
	cd src && $(MAKE) clean

