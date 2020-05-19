
## `lol` headers to keep

    src/lol/base/types.h (vec_t shortcuts eg. vec2, ivec2, u8vec3 etc.)
    src/lol/math/constants.h


## headers we could clean up and keep

    src/lol/algorithm/aabb_tree.h
    src/lol/algorithm/portal.h
    src/lol/base/avl_tree.h
    src/lol/image/pixel.h (but is it useful?)
    src/lol/math/arraynd.h (but do we really want to keep it?)
    src/lol/math/geometry.h
    src/lol/math/interp.h (but what is it?)


## headers to keep in the engine

    src/lol/algorithm/portal.h
    src/lol/audio/*
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
    src/lol/sys/init.h


## headers to remove one day

    src/lol/base/enum.h (can’t see the point)

