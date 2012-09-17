
include $(top_srcdir)/build/autotools/common.am

if USE_CACA
all: ascii.png

ascii.png:
	../../../tools/make-font | convert tga:- $@

#clean:
#	rm -f ascii.png
endif

