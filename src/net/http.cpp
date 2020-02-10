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

#include <lol/engine-internal.h>

namespace lol
{

namespace net
{

namespace http
{

#if __EMSCRIPTEN__
class client_impl
{
    // FIXME
};
#else
class client_impl
{
public:
    void get(std::string const& url)
    {

    }
};
#endif

client::client()
  : impl(std::make_unique<client_impl>())
{
}

client::~client()
{
}

void client::get(std::string const &url)
{
}

bool client::is_ready() const
{
    return false;
}

std::tuple<int, std::string> client::result()
{
    return std::make_tuple(404, std::string());
}

} // namespace http

} // namespace net

} // namespace lol

