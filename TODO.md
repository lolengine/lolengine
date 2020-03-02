
## `lol` headers to keep

    src/lol/base/types.h (vec_t shortcuts eg. vec2, ivec2, u8vec3 etc.)
    src/lol/math/bigint.h
    src/lol/math/constants.h
    src/lol/math/functions.h
    src/lol/math/noise/*


## other headers to keep

    tools/lolunit/lolunit.h

## headers we could clean up and keep

    src/lol/algorithm/aabb_tree.h
    src/lol/base/assert.h (could be useful)
    src/lol/base/avl_tree.h
    src/lol/base/string.h (string utilities)
    src/lol/image/color.h
    src/lol/image/pixel.h (but is it useful?)
    src/lol/math/arraynd.h (but do we really want to keep it?)
    src/lol/math/geometry.h
    src/lol/math/interp.h (but what is it?)

    src/numeric.h


## headers to keep in the engine

    src/lol/algorithm/portal.h
    src/lol/audio/*
    src/lol/base/features.h (a bit dangerous to ship)
    src/lol/base/log.h
    src/lol/debug/*
    src/lol/engine.h
    src/lol/engine/*
    src/lol/engine-internal.h
    src/lol/extras.h
    src/lol/gpu/*
    src/lol/image/image.h
    src/lol/image/movie.h
    src/lol/image/resource.h
    src/lol/legacy.h
    src/lol/lua.h
    src/lol/net/http.h
    src/lol/public.h
    src/lol/sys/file.h
    src/lol/sys/getopt.h
    src/lol/sys/init.h


## headers to remove one day

    src/lol/algorithm/sort.h (there’s plenty of stuff in libc++)
    src/lol/base/array.h (use std::vector)
    src/lol/base/enum.h (can’t see the point)
    src/lol/base/tuple.h (use std::tuple)

