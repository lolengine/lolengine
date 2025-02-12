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

#include <algorithm> // std::min
#include <lol/std/span> // std::span
#include <string> // std::string

namespace lol::sys
{

struct resource
{
    resource() = default;

    // Add an entry to the resource registry
    static resource add(std::string const &path, void const *data, size_t size, uint32_t flags);

    // Get an entry from the registry
    static resource get(std::string const &path);

    bool exists() const { return (m_flags & error) == 0; }

    size_t size() const { return m_data.size(); }

    size_t read(void *data, size_t pos, size_t len) const
    {
        if (pos >= m_data.size())
            return 0;
        len = std::min(len, m_data.size() - pos);
        memcpy(data, m_data.data() + pos, len);
        return len;
    }

    void read(std::string &s) const
    {
        s.assign((char *)m_data.data(), m_data.size());
    }

protected:
    resource(void const *data, size_t size, uint32_t flags);

    std::span<uint8_t const> m_data;
    uint32_t m_flags = 0x0000;

    // FIXME: this requires synchronisation with msbuild/embed/task.props and
    // is thus a bit fragile; find something else, maybe macros?
    static uint32_t const error = 0x0001;
    static uint32_t const gzip  = 0x0002;
};

} // namespace lol::sys
