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

#pragma once

//
// The resource classes
// ————————————————————
//

#include <lol/std/span> // std::span
#include <string>
#include <unordered_map> // std::unordered_map

namespace lol::sys
{

struct resource
{
    resource() = default;

    resource(std::string const &path, void const *data, size_t size, uint32_t flags)
      : m_data(static_cast<uint8_t const *>(data), size),
        m_flags(flags)
    {
        catalog()[path] = *this;
    }

    static bool get(std::string const &path, std::string &data)
    {
        auto ret = catalog().find(path);
        if (ret == catalog().end())
            return false;
        data.assign((char *)ret->second.m_data.data(), ret->second.m_data.size());
        return true;
    }

protected:
    std::span<uint8_t const> m_data;

    uint32_t m_flags;

    static std::unordered_map<std::string, struct resource> &catalog();
};

} // namespace lol::sys
