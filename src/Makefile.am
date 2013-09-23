
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = bullet lua data

noinst_LIBRARIES = liblolcore.a

liblolcore_a_SOURCES = \
    core.h tiler.cpp tiler.h dict.cpp dict.h \
    audio.cpp audio.h scene.cpp scene.h font.cpp font.h layer.cpp layer.h \
    map.cpp map.h entity.cpp entity.h ticker.cpp ticker.h lolgl.h \
    tileset.cpp tileset.h forge.cpp forge.h video.cpp video.h \
    world.cpp world.h sample.cpp sample.h sampler.cpp sampler.h \
    profiler.cpp profiler.h text.cpp text.h emitter.cpp emitter.h numeric.h utils.h \
    messageservice.cpp messageservice.h \
    worldentity.cpp worldentity.h gradient.cpp gradient.h gradient.lolfx \
    platform.cpp platform.h sprite.cpp sprite.h camera.cpp camera.h \
    light.cpp light.h \
    \
    $(liblolcore_headers) \
    $(liblolcore_sources) \
    \
    $(ps3_sources) \
    $(xbox_sources) \
    $(nacl_sources) \
    $(sdl_sources) \
    $(d3d9_sources) \
    $(android_sources)
liblolcore_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/bullet -I$(srcdir)/lua

EXTRA_DIST += easymesh/easymesh-scanner.l easymesh/easymesh-parser.y \
              gpu/lolfx-scanner.l gpu/lolfx-parser.y

liblolcore_headers = \
    lol/base/all.h \
    lol/base/log.h lol/base/array.h lol/base/types.h lol/base/array.h \
    lol/base/assert.h lol/base/string.h lol/base/hash.h lol/base/map.h \
    \
    lol/math/all.h \
    lol/math/functions.h lol/math/vector.h lol/math/half.h lol/math/real.h \
    lol/math/remez.h lol/math/geometry.h lol/math/interp.h lol/math/rand.h \
    lol/math/constants.h \
    \
    lol/sys/all.h \
    lol/sys/init.h lol/sys/file.h lol/sys/thread.h lol/sys/atomic.h \
    lol/sys/timer.h \
    \
    lol/image/all.h \
    lol/image/color.h lol/image/image.h \
    \
    lol/gpu/all.h \
    lol/gpu/shader.h lol/gpu/indexbuffer.h lol/gpu/vertexbuffer.h \
    lol/gpu/framebuffer.h lol/gpu/texture.h lol/gpu/lolfx.h \
    lol/gpu/renderer.h lol/gpu/rendercontext.h \
    \
    lol/debug/all.h \
    lol/debug/lines.h \
    \
    lol/unit.h

liblolcore_sources = \
    generated/location.hh generated/position.hh generated/stack.hh \
    \
    application/application.cpp application/application.h \
    eglapp.cpp eglapp.h \
    \
    easymesh/easymesh.cpp easymesh/easymesh.h \
    easymesh/csgbsp.cpp easymesh/csgbsp.h \
    easymesh/shiny.lolfx easymesh/shinydebugwireframe.lolfx \
    easymesh/shinydebuglighting.lolfx easymesh/shinydebugnormal.lolfx \
    easymesh/shinydebugUV.lolfx easymesh/shiny_SK.lolfx \
    easymesh/easymesh-compiler.cpp easymesh/easymesh-compiler.h \
    generated/easymesh-parser.cpp generated/easymesh-parser.h \
    generated/easymesh-scanner.cpp \
    \
    base/assert.cpp base/hash.cpp base/log.cpp base/string.cpp \
    \
    math/vector.cpp math/real.cpp math/half.cpp math/trig.cpp \
    math/constants.cpp math/geometry.cpp \
    \
    gpu/shader.cpp gpu/indexbuffer.cpp gpu/vertexbuffer.cpp \
    gpu/framebuffer.cpp gpu/texture.cpp gpu/renderer.cpp \
    gpu/rendercontext.cpp \
    \
    input/input.cpp input/input.h input/input_internal.h input/keys.h \
    input/controller.cpp input/controller.h \
    \
    gpu/defaultmaterial.lolfx \
    gpu/tile.lolfx gpu/line.lolfx \
    gpu/emptymaterial.lolfx \
    gpu/testmaterial.lolfx \
    \
    gpu/lolfx.cpp \
    gpu/lolfx-compiler.cpp gpu/lolfx-compiler.h \
    generated/lolfx-parser.cpp generated/lolfx-parser.h \
    generated/lolfx-scanner.cpp \
    \
    mesh/mesh.cpp mesh/mesh.h \
    \
    sys/init.cpp sys/timer.cpp sys/file.cpp \
    sys/threadbase.h \
    \
    image/image.cpp image/image-private.h \
    image/codec/gdiplus-image.cpp \
    image/codec/ios-image.cpp \
    image/codec/dummy-image.cpp \
    image/color/cie1931.cpp \
    \
    loldebug.h \
    debug/fps.cpp debug/fps.h debug/lines.cpp \
    debug/record.cpp debug/record.h debug/stats.cpp debug/stats.h

sdl_sources = \
    image/codec/sdl-image.cpp \
    platform/sdl/sdlapp.cpp platform/sdl/sdlapp.h \
    platform/sdl/sdlinput.cpp platform/sdl/sdlinput.h

d3d9_sources = \
    platform/d3d9/d3d9input.cpp platform/d3d9/d3d9input.h

if USE_NACL
nacl_sources = \
    platform/nacl/nacl-app.cpp platform/nacl/nacl-app.h \
    platform/nacl/nacl-instance.cpp platform/nacl/nacl-instance.h \
    platform/nacl/nacl-module.cpp \
    platform/nacl/opengl_context.cpp platform/nacl/opengl_context.h \
    platform/nacl/opengl_context_ptrs.h
endif

if USE_PS3
ps3_sources = \
    image/codec/ps3-image.cpp \
    platform/ps3/threadbase.h \
    platform/ps3/ps3app.cpp platform/ps3/ps3app.h \
    platform/ps3/ps3input.cpp platform/ps3/ps3input.h
endif

if USE_X360
xbox_sources = \
    platform/xbox/xboxapp.cpp platform/xbox/xboxapp.h \
    platform/xbox/xboxinput.cpp platform/xbox/xboxinput.h
endif

android_sources = \
    image/codec/android-image.cpp \
    platform/android/androidapp.cpp platform/android/androidapp.h

