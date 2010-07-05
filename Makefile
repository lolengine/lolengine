
all:
	cd art && $(MAKE) all
	cd src && $(MAKE) all

clean:
	cd art && $(MAKE) clean
	cd src && $(MAKE) clean

