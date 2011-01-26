
noinst_LIBRARIES = liblol.a

liblol_a_SOURCES = \
    core.h matrix.h tiler.cpp tiler.h dict.cpp dict.h audio.cpp audio.h \
    scene.cpp scene.h font.cpp font.h layer.cpp layer.h map.cpp map.h \
    entity.cpp entity.h ticker.cpp ticker.h tileset.cpp tileset.h \
    forge.cpp forge.h video.cpp video.h timer.cpp timer.h bitfield.h \
    profiler.cpp profiler.h input.h input.cpp world.cpp world.h \
    sample.cpp sample.h sampler.cpp sampler.h text.cpp text.h \
    emitter.cpp emitter.h numeric.h \
    \
    sdlinput.cpp sdlinput.h \
    \
    debugfps.cpp debugfps.h debugsphere.cpp debugsphere.h \
    debugrecord.cpp debugrecord.h debugstats.cpp debugstats.h
liblol_a_CXXFLAGS = `pkg-config --cflags sdl gl SDL_image`

