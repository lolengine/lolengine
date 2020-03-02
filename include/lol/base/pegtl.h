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

//
// The PEGTL classes
// —————————————————
//

// Ensure CreateFile2() is available on mingw
#if defined _WIN32 && !defined _MSC_VER && \
              (!defined _WIN32_WINNT || _WIN32_WINNT < 0x0602)
#   undef _WIN32_WINNT
#   define _WIN32_WINNT 0x0602
#endif

//namespace lol
//{

#include "../3rdparty/pegtl/include/tao/pegtl.hpp"

//} // namespace lol

