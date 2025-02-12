//
//  Lol Engine
//
//  Copyright © 2010–2025 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine/sys> // lol::sys::resource
#include <string> // std::string
#include <unordered_map> // std::unordered_map

namespace lol::sys
{

static std::unordered_map<std::string, resource> &catalog()
{
    // We don’t make this a global variable because of static initialisation order
    static std::unordered_map<std::string, resource> ret;
    return ret;
}

resource::resource(void const *data, size_t size, uint32_t flags)
  : m_data(static_cast<uint8_t const *>(data), size),
    m_flags(flags)
{
}

resource resource::add(std::string const &path, void const *data, size_t size, uint32_t flags)
{
    return catalog()[path] = resource(data, size, flags);
}

resource resource::get(std::string const &path)
{
    if (auto it = catalog().find(path); it != catalog().end())
        return it->second;
    return resource(nullptr, 0, error);
}

} // namespace lol::sys