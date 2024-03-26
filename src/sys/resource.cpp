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

#include <lol/engine/sys> // lol::sys::resource

namespace lol::sys
{

std::unordered_map<std::string, resource> &resource::catalog()
{
    // This is not a global variable because of static initialisation order
    static std::unordered_map<std::string, resource> ret;
    return ret;
}

} // namespace lol::sys