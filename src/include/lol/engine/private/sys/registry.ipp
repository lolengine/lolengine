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

namespace lol::registry
{

static std::unordered_map<std::string, struct resource> registry;

struct resource
{
    resource() = default;

    resource(uint8_t const *data, size_t size)
      : data(data, size)
    {}

    std::span<uint8_t const, std::dynamic_extent> data;
};

};

#define LOL_EMBED_REGISTER(name) \
    extern "C" uint8_t *name; \
    extern "C" size_t name##_size; \
    extern "C" char const *name##_path; \
    static auto name##_token = lol::registry::registry[name##_path] = lol::registry::resource(name, name##_size)
