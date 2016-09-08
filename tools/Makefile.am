
include $(top_srcdir)/build/autotools/common.am

SUBDIRS =
SUBDIRS += lolremez
SUBDIRS += lolunit
SUBDIRS += vimlol
SUBDIRS += vslol

if BUILD_TOOLS
noinst_PROGRAMS = $(make_font)
endif

make_font_SOURCES = make-font.cpp
make_font_CPPFLAGS = @CACA_CFLAGS@
make_font_LDFLAGS = @CACA_LIBS@

if LOL_USE_CACA
make_font = make-font
endif

