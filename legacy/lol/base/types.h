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

#include <stdint.h>

namespace lol
{

// It’s sometimes nice to have single-word type names, so define this.
typedef long double ldouble;

template<typename T> class real_t;
typedef real_t<uint32_t> real;

class half;

} /* namespace lol */

