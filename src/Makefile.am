
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = 3rdparty data . t

noinst_LIBRARIES = liblol-core.a

EXTRA_DIST += lol-core.vcxproj lol-core.vcxproj.filters

liblol_core_a_SOURCES = \
    lolgl.h scene.cpp scene.h font.cpp font.h \
    textureimage.cpp textureimage.h textureimage-private.h \
    tileset.cpp tileset.h video.cpp video.h \
    profiler.cpp profiler.h text.cpp text.h emitter.cpp emitter.h \
    numeric.h utils.h messageservice.cpp messageservice.h \
    gradient.cpp gradient.h gradient.lolfx \
    platform.cpp platform.h sprite.cpp sprite.h camera.cpp camera.h \
    light.cpp light.h \
    \
    $(liblol_core_headers) \
    $(liblol_core_sources) \
    \
    $(imgui_sources) \
    $(sdl_sources) \
    $(d3d9_sources) \
    $(android_sources)
liblol_core_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/3rdparty/lua/src

liblol_core_headers = \
    lol/engine-internal.h lol/engine.h lol/public.h lol/extras.h \
    lol/lua.h \
    \
    lol/base/all.h \
    lol/base/array.h lol/base/map.h lol/base/enum.h \
    lol/base/log.h \
    \
    lol/audio/all.h \
    lol/audio/audio.h lol/audio/sample.h \
    \
    lol/engine/all.h \
    lol/engine/tickable.h \
    \
    lol/sys/all.h \
    lol/sys/init.h lol/sys/file.h \
    \
    lol/image/all.h \
    lol/image/image.h \
    lol/image/resource.h lol/image/movie.h \
    \
    lol/net/all.h \
    lol/net/http.h \
    \
    lol/gpu/all.h \
    lol/gpu/shader.h lol/gpu/indexbuffer.h lol/gpu/vertexbuffer.h \
    lol/gpu/framebuffer.h lol/gpu/texture.h lol/gpu/lolfx.h \
    lol/gpu/renderer.h lol/gpu/rendercontext.h lol/gpu/debug.h \
    \
    lol/debug/all.h \
    lol/debug/lines.h

liblol_core_sources = \
    application/application.cpp application/application.h \
    application/egl-app.cpp application/egl-app.h \
    \
    lolua/baselua.cpp lolua/baselua.h \
    \
    commandstack.h \
    easymesh/easymeshbuild.cpp easymesh/easymeshbuild.h \
    easymesh/easymeshrender.cpp easymesh/easymeshrender.h \
    easymesh/easymesh.cpp \
    easymesh/easymeshinternal.cpp easymesh/easymeshcsg.cpp \
    easymesh/easymeshprimitive.cpp easymesh/easymeshtransform.cpp \
    easymesh/easymeshcursor.cpp easymesh/easymesh.h \
    easymesh/easymeshlua.cpp easymesh/easymeshlua.h \
    easymesh/csgbsp.cpp easymesh/csgbsp.h \
    easymesh/shiny.lolfx easymesh/shinyflat.lolfx \
    easymesh/shinydebugwireframe.lolfx \
    easymesh/shinydebuglighting.lolfx easymesh/shinydebugnormal.lolfx \
    easymesh/shinydebugUV.lolfx easymesh/shiny_SK.lolfx \
    \
    base/assert.cpp base/log.cpp \
    \
    math/geometry.cpp \
    \
    gpu/shader.cpp gpu/indexbuffer.cpp gpu/vertexbuffer.cpp \
    gpu/framebuffer.cpp gpu/texture.cpp gpu/renderer.cpp \
    gpu/rendercontext.cpp gpu/debug.cpp \
    \
    audio/audio.cpp audio/sample.cpp \
    \
    ui/input.cpp ui/input.h ui/keys.inc ui/buttons.inc \
    ui/gui.cpp ui/gui.h \
    \
    gpu/default-material.lolfx \
    gpu/empty-material.lolfx \
    gpu/test-material.lolfx \
    gpu/tile.lolfx gpu/palette.lolfx gpu/line.lolfx \
    gpu/blit.lolfx \
    gpu/postprocess.lolfx \
    \
    gpu/lolfx.cpp \
    \
    mesh/mesh.cpp mesh/mesh.h \
    mesh/primitivemesh.cpp mesh/primitivemesh.h \
    \
    sys/init.cpp sys/file.cpp \
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
    net/http.cpp \
    \
    engine/tickable.cpp engine/ticker.cpp engine/ticker.h \
    engine/entity.cpp engine/entity.h \
    engine/world.cpp engine/world.h \
    engine/worldentity.cpp engine/worldentity.h \
    \
    loldebug.h \
    debug/fps.cpp debug/fps.h debug/lines.cpp \
    debug/record.cpp debug/record.h debug/stats.cpp debug/stats.h

if LOL_USE_SDL
sdl_sources = \
    application/sdl-app.cpp application/sdl-app.h \
    ui/sdl-input.cpp ui/sdl-input.h
endif

d3d9_sources = \
    ui/d3d9-input.cpp ui/d3d9-input.h

include 3rdparty/lol-imgui.am

android_sources = \
    image/codec/android-image.cpp \
    application/android-app.cpp application/android-app.h

