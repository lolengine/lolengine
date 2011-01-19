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
// The Input static class
// ----------------------
//

#if !defined __DH_INPUT_H__
#define __DH_INPUT_H__

#include "matrix.h"

class Input
{
public:
    static Float2 GetAxis(int axis);
    static void SetMousePos(Int2 coord);
    static Int2 GetMousePos();
};

#endif // __DH_INPUT_H__

