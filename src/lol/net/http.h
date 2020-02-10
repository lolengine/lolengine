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

class client
{
public:
    client();
    ~client();

    void get(std::string const &url);
    bool is_ready() const;
    std::tuple<int, std::string> result();

private:
    std::unique_ptr<client_impl> impl;
};

} // namespace http

} // namespace net

} // namespace lol

