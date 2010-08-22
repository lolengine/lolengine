//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Matrix classes
// ------------------
//

#if !defined __DH_MATRIX_H__
#define __DH_MATRIX_H__

struct Float2
{
    Float2() { x = y = 0.0f; }
    Float2(float _x, float _y) { x = _x; y = _y; }

    float x, y;
};

struct Float3
{
    Float3() { x = y = z = 0.0f; }
    Float3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

    float x, y, z;
};

#endif // __DH_MATRIX_H__

