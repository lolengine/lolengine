//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

bool has_threads()
{
    static char const *var = getenv("LOL_NOTHREADS");
    static bool const disable_threads = var && var[0];
    return !disable_threads && std::thread::hardware_concurrency() > 1;
}

} // namespace lol

