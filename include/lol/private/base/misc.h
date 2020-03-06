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

namespace lol
{

namespace sys
{

// A handy endianness test function
static inline bool is_big_endian()
{
    union { int i; char c; } u;
    u.i = 1;
    return u.c == 0;
}

} // namespace os

} // namespace lol
