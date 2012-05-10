//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The D3d9Input class
// -------------------
//

#if !defined __LOL_D3D9INPUT_H__
#define __LOL_D3D9INPUT_H__

#include "entity.h"

namespace lol
{

class D3d9InputData;

class D3d9Input : public Entity
{
public:
    D3d9Input();
    virtual ~D3d9Input();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    D3d9InputData *m_data;
};

} /* namespace lol */

#endif // __LOL_D3D9INPUT_H__

