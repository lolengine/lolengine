//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The XboxInput class
// -------------------
//

#if !defined __LOL_XBOXINPUT_H__
#define __LOL_XBOXINPUT_H__

#include "entity.h"

namespace lol
{

class XboxInputData;

class XboxInput : public Entity
{
public:
    XboxInput();
    virtual ~XboxInput();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    XboxInputData *m_data;
};

} /* namespace lol */

#endif // __LOL_XBOXINPUT_H__

