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

#if defined(__has_cpp_attribute)

#   if !__has_cpp_attribute(nodiscard)
#       error C++ compiler support for [[nodiscard]] is required (use -std=c++17?)
#   endif

#   if !__has_cpp_attribute(fallthrough)
#       error C++ compiler support for [[fallthrough]] is required (use -std=c++17?)
#   endif

#endif


// C++ compiler features detected through __has_feature

#if defined(__has_feature)

#   if !__has_feature(cxx_constexpr)
#       error C++ compiler for constexpr is required (use -std=c++11?)
#   endif

#endif


// Define some attribute macros

#ifdef __GNUC__
#   define lol_attr_printf_format(n, p) __attribute__((format(printf, n, p)))
#else
#   define lol_attr_printf_format(n, p)
#endif

