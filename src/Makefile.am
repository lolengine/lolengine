
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

liblol_core_headers =

liblol_core_sources = \
    base/assert.cpp base/engine.cpp base/log.cpp \
    \
    audio/audio.cpp \
    \
    sys/init.cpp \
    \
    image/resource.cpp image/resource-private.h \
    image/image.cpp image/image-private.h image/kernel.cpp image/pixel.cpp \
    image/crop.cpp image/resample.cpp image/noise.cpp image/combine.cpp \
    image/codec/gdiplus-image.cpp image/codec/imlib2-image.cpp \
    image/codec/sdl-image.cpp image/codec/ios-image.cpp \
    image/codec/oric-image.cpp image/codec/dummy-image.cpp \
    image/dither/random.cpp image/dither/ediff.cpp image/dither/dbs.cpp \
    image/dither/ostromoukhov.cpp image/dither/ordered.cpp \
    image/filter/convolution.cpp image/filter/colors.cpp \
    image/filter/dilate.cpp image/filter/median.cpp image/filter/yuv.cpp \
    image/movie.cpp \
    \
    net/http.cpp

include 3rdparty/lol-imgui.am
