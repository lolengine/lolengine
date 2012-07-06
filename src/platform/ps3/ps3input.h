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
// The Ps3Input class
// ------------------
//

#if !defined __LOL_PS3INPUT_H__
#define __LOL_PS3INPUT_H__

#include "entity.h"

namespace lol
{

class Ps3InputData;

class Ps3Input : public Entity
{
public:
    Ps3Input();
    virtual ~Ps3Input();

protected:
    virtual void TickGame(float seconds);

private:
    Ps3InputData *m_data;
};

} /* namespace lol */

#endif // __LOL_PS3INPUT_H__

