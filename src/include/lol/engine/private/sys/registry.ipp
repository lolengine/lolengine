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

#include <span>
#include <string>
#include <unordered_map>

#if !defined LOL_EMBED_STATIC_INITIALIZER
#   error "LOL_EMBED_STATIC_INITIALIZER is not defined; please check your build system"
#endif

#define LOL_EMBED_REGISTER(name) \
    extern "C" uint8_t *name; \
    extern "C" size_t name##_size; \
    extern "C" char const *name##_path; \
    static auto name##_token = lol::registry::registry[name##_path] = lol::registry::resource(name, name##_size)

#if defined LOL_EMBED_REGISTRY_IMPL
extern "C" { int LOL_EMBED_STATIC_INITIALIZER = 0xdeadbeef; }
#else
extern "C" { extern int LOL_EMBED_STATIC_INITIALIZER; }
#endif

namespace lol::registry
{

static std::unordered_map<std::string, struct resource> registry;

struct resource
{
    resource() = default;

    resource(uint8_t const *data, size_t size)
      : data(data, size)
    {}

    std::span<uint8_t const> data;
};

};
