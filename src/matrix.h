//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Matrix classes
// ------------------
//

#if !defined __DH_MATRIX_H__
#define __DH_MATRIX_H__

template <typename T> struct Vector2
{
    Vector2() { x = y = 0; }
    Vector2(T _x, T _y) { x = _x; y = _y; }
    T& operator[](int n) { return *(&x + n); }

    union { T x; T a; T i; };
    union { T y; T b; T j; };
};

typedef Vector2<float> Float2;
typedef Vector2<int> Int2;

template <typename T> struct Vector3
{
    Vector3() { x = y = z = 0; }
    Vector3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
    T& operator[](int n) { return *(&x + n); }

    union { T x; T a; T i; };
    union { T y; T b; T j; };
    union { T z; T c; T k; };
};

typedef Vector3<float> Float3;
typedef Vector3<int> Int3;

#endif // __DH_MATRIX_H__

