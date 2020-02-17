//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <memory>

namespace lol
{

namespace net
{

namespace http
{

class client_impl;

enum class status : uint8_t
{
    ready   = 0,
    pending = 1,
    success = 2,
    error   = 3,
};

class client
{
public:
    client();
    ~client();

    // Enqueue a query
    void get(std::string const &url);

    // Reset state
    void reset();

    // Get status (may be pending) and result
    status get_status() const;
    std::string const & get_result() const;

private:
    std::unique_ptr<client_impl> impl;
};

} // namespace http

} // namespace net

} // namespace lol

