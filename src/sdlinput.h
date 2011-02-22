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
// The SdlInput class
// ------------------
//

#if !defined __DH_SDLINPUT_H__
#define __DH_SDLINPUT_H__

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
    virtual void TickGame(float deltams);

private:
    SdlInputData *data;
};

} /* namespace lol */

#endif // __DH_SDLINPUT_H__

