
noinst_LIBRARIES = liblol.a

liblol_a_SOURCES = \
    core.h matrix.cpp matrix.h tiler.cpp tiler.h dict.cpp dict.h \
    audio.cpp audio.h scene.cpp scene.h font.cpp font.h layer.cpp layer.h \
    map.cpp map.h entity.cpp entity.h ticker.cpp ticker.h lolgl.h \
    tileset.cpp tileset.h forge.cpp forge.h video.cpp video.h log.cpp log.h \
    timer.cpp timer.h bitfield.h profiler.cpp profiler.h input.h input.cpp \
    world.cpp world.h sample.cpp sample.h sampler.cpp sampler.h \
    text.cpp text.h emitter.cpp emitter.h numeric.h hash.cpp hash.h \
    worldentity.cpp worldentity.h gradient.cpp gradient.h half.cpp half.h \
    platform.cpp platform.h sprite.cpp sprite.h trig.cpp trig.h \
    real.cpp real.h emcee.cpp emcee.h \
    \
    lol/unit.h \
    \
    application/application.cpp application/application.h \
    eglapp.cpp eglapp.h \
    \
    $(ps3_sources) \
    $(nacl_sources) \
    $(sdl_sources) \
    \
    shader/shader.cpp shader/shader.h \
    thread/threadbase.h thread/thread.h \
    gpu/vbo.cpp gpu/vbo.h \
    \
    image/image.cpp image/image.h image/image-private.h \
    image/codec/android-image.cpp \
    image/codec/gdiplus-image.cpp \
    image/codec/ios-image.cpp \
    image/codec/sdl-image.cpp \
    image/codec/ps3-image.cpp \
    image/codec/dummy-image.cpp \
    \
    loldebug.h \
    debug/fps.cpp debug/fps.h debug/sphere.cpp debug/sphere.h \
    debug/record.cpp debug/record.h debug/stats.cpp debug/stats.h \
    debug/quad.cpp debug/quad.h
liblol_a_CPPFLAGS = @LOL_CFLAGS@

sdl_sources = \
    platform/sdl/sdlapp.cpp platform/sdl/sdlapp.h \
    platform/sdl/sdlinput.cpp platform/sdl/sdlinput.h

if USE_NACL
nacl_sources = \
    platform/nacl/naclapp.cpp platform/nacl/naclapp.h \
    platform/nacl/nacl_instance.cpp platform/nacl/nacl_instance.h \
    platform/nacl/nacl_module.cpp \
    platform/nacl/opengl_context.cpp platform/nacl/opengl_context.h \
    platform/nacl/opengl_context_ptrs.h
endif

if HAVE_PS3
ps3_sources =  \
    platform/ps3/threadbase.h \
    platform/ps3/ps3app.cpp platform/ps3/ps3app.h \
    platform/ps3/ps3input.cpp platform/ps3/ps3input.h
endif

