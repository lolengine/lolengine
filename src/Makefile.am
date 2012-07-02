
SUBDIRS = data

noinst_LIBRARIES = liblol.a

liblol_a_SOURCES = \
    core.h tiler.cpp tiler.h dict.cpp dict.h array.h \
    audio.cpp audio.h scene.cpp scene.h font.cpp font.h layer.cpp layer.h \
    map.cpp map.h entity.cpp entity.h ticker.cpp ticker.h lolgl.h \
    tileset.cpp tileset.h forge.cpp forge.h video.cpp video.h log.cpp log.h \
    timer.cpp timer.h bitfield.h profiler.cpp profiler.h \
    world.cpp world.h sample.cpp sample.h sampler.cpp sampler.h \
    text.cpp text.h emitter.cpp emitter.h numeric.h hash.cpp hash.h \
    worldentity.cpp worldentity.h gradient.cpp gradient.h \
    platform.cpp platform.h sprite.cpp sprite.h camera.cpp camera.h \
    \
    lol/unit.h lol/debug.h \
    lol/math/vector.h lol/math/half.h lol/math/real.h lol/math/remez.h \
    lol/math/math.h \
    \
    generated/location.hh generated/position.hh generated/stack.hh \
    \
    application/application.cpp application/application.h \
    eglapp.cpp eglapp.h \
    \
    easymesh/easymesh.cpp easymesh/easymesh.h \
    easymesh/shiny.lolfx \
    easymesh/easymesh-compiler.cpp easymesh/easymesh-compiler.h \
    generated/easymesh-parser.cpp generated/easymesh-parser.h \
    generated/easymesh-scanner.cpp \
    \
    $(ps3_sources) \
    $(xbox_sources) \
    $(nacl_sources) \
    $(sdl_sources) \
    $(d3d9_sources) \
    $(android_sources) \
    \
    thread/threadbase.h thread/thread.h \
    \
    math/vector.cpp math/real.cpp math/half.cpp math/trig.cpp \
    \
    input/input.cpp input/input.h \
    input/stick.cpp input/stick.h \
    \
    gpu/shader.cpp gpu/shader.h \
    gpu/indexbuffer.cpp gpu/indexbuffer.h \
    gpu/vertexbuffer.cpp gpu/vertexbuffer.h \
    gpu/framebuffer.cpp gpu/framebuffer.h \
    gpu/defaultmaterial.lolfx \
    gpu/emptymaterial.lolfx \
    gpu/testmaterial.lolfx \
    \
    image/image.cpp image/image.h image/image-private.h \
    image/codec/gdiplus-image.cpp \
    image/codec/ios-image.cpp \
    image/codec/dummy-image.cpp \
    \
    loldebug.h \
    debug/fps.cpp debug/fps.h debug/sphere.cpp debug/sphere.h \
    debug/record.cpp debug/record.h debug/stats.cpp debug/stats.h \
    debug/quad.cpp debug/quad.h
liblol_a_CPPFLAGS = @LOL_CFLAGS@

SUFFIXES = .lolfx
.lolfx.o:
	$(LOLFX_BUILD)

if TRUE
generated: .FORCE
	$(MKDIR_P) generated
	rm -f generated/[a-zA-Z]*
	flex -o generated/easymesh-scanner.cpp easymesh/easymesh-scanner.l
	bison -o generated/easymesh-parser.cpp --defines=generated/easymesh-parser.h \
	      -d -b generated/easymesh easymesh/easymesh-parser.y
.FORCE:
endif

EXTRA_DIST = easymesh/easymesh-scanner.l easymesh/easymesh-parser.y

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

