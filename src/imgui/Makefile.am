
include $(top_srcdir)/build/autotools/common.am

noinst_LIBRARIES = libimgui.a

libimgui_a_SOURCES = $(imgui_sources)
libimgui_a_CPPFLAGS = $(AM_CPPFLAGS)

imgui_sources = \
    stb_rect_pack.h stb_textedit.h stb_truetype.h \
    imconfig.h imgui.cpp imgui.h \
    lolimgui.cpp lolimgui.h

