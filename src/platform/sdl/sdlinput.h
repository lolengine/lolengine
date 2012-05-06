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
// The SdlInput class
// ------------------
//

#if !defined __LOL_SDLINPUT_H__
#define __LOL_SDLINPUT_H__

#include "entity.h"

namespace lol
{

class SdlInputData;

class SdlInput : public Entity
{
public:
    SdlInput();
    virtual ~SdlInput();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    SdlInputData *m_data;
};

} /* namespace lol */

#endif // __LOL_SDLINPUT_H__

