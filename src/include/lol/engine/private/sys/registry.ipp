//
//  Lol Engine
//
//  Copyright © 2010–2024 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine/sys> // lol::sys::resource

#if !defined LOL_EMBED_STATIC_INITIALIZER
#   error "LOL_EMBED_STATIC_INITIALIZER is not defined; please check the build system"
#endif

#define LOL_EMBED_REGISTER(name) \
    extern "C" uint8_t *name; \
    extern "C" size_t name##_size; \
    extern "C" char const *name##_path; \
    static auto name##_token = lol::sys::resource(name##_path, name, name##_size)

// Applications must reference this variable so that embedded resources
// get linked into the final binary.
extern "C" { int LOL_EMBED_STATIC_INITIALIZER = 0xdeadbeef; }
