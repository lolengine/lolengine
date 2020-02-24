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
// The build-time features
// ———————————————————————
//

// C++ compiler features detected through __has_cpp_attribute

#if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
#   define LOL_ATTR_NODISCARD [[nodiscard]]
#else
#   define LOL_ATTR_NODISCARD /* */
#endif

#if defined(__has_cpp_attribute) && __has_cpp_attribute(fallthrough)
#   define LOL_ATTR_FALLTHROUGH [[fallthrough]];
#else
#   define LOL_ATTR_FALLTHROUGH /* */
#endif

// Define some attribute macros.

#ifdef __GNUC__
#   define LOL_ATTR_FORMAT(n, p) __attribute__((format(printf, n, p)))
#else
#   define LOL_ATTR_FORMAT(n, p)
#endif

#if defined(_WIN32)
#   define LOL_ATTR_STDCALL __stdcall
#else
#   define LOL_ATTR_STDCALL /* */
#endif

