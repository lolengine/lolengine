
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = . 3rdparty data t

# Convenience library; needs to be marked noinst
noinst_LTLIBRARIES = liblol-core.la

EXTRA_DIST += lol-core.vcxproj lol-core.vcxproj.filters

liblol_core_la_SOURCES = \
    $(liblol_core_headers) \
    $(liblol_core_sources) \
    \
    $(imgui_sources) \
    $(sdl_sources) \
    $(d3d9_sources) \
    $(android_sources)
liblol_core_la_CPPFLAGS = $(AM_CPPFLAGS)
liblol_core_la_LDFLAGS = $(AM_LDFLAGS)

liblol_core_headers = \
    src/include/lol/engine/net \
    src/include/lol/engine/sys \
    \
    src/include/lol/engine/private/net/http.h \
    src/include/lol/engine/private/sys/init.h

liblol_core_sources = \
    net/http.cpp \
    \
    sys/init.cpp \
    sys/main.cpp

include 3rdparty/lol-imgui.am
