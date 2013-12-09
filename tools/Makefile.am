
include $(top_srcdir)/build/autotools/common.am

SUBDIRS =
SUBDIRS += lolremez
SUBDIRS += neercs
SUBDIRS += vimlol
SUBDIRS += vslol

noinst_PROGRAMS = $(make_font)

make_font_SOURCES = make-font.cpp
make_font_CPPFLAGS = @CACA_CFLAGS@
make_font_LDFLAGS = @CACA_LIBS@

if USE_CACA
make_font = make-font
endif

