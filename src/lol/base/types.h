//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_BASE_TYPES_H__
#define __LOL_BASE_TYPES_H__

namespace lol
{

/* There are many reasons for wanting single-word type names, the most
 * important one being compilation speedups in our vector.h: we can hide
 * some global methods in namespaces that contain the name of the type,
 * but namespaces cannot have spaces in their names. */
typedef long double ldouble;

/* The “real” type used for real numbers. It’s a specialisation of the
 * “Real” template class. */
template<int N> class Real;
typedef Real<16> real;

/* The “half” type used for 16-bit floating point numbers. */
class half;

/* Forward declaration of vec and matrix */
template<int N, typename T, int MASK> struct vec;
template<int COLS, int ROWS, typename T> struct matrix;

} /* namespace lol */

#endif // __LOL_BASE_TYPES_H__

