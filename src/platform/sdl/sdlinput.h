//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The SdlInput class
// ------------------
//

#include "entity.h"

namespace lol
{

class SdlInputData;

class SdlInput : public Entity
{
public:
    /** passing the screen resolution (note: not the windowed app resolution!) allows to make the mouse axis resolution-independent */
    SdlInput(int app_w, int app_h, int screen_w, int screen_h);

    virtual ~SdlInput();
    void SetScreenResolution();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, Scene &scene);

private:
    SdlInputData *m_data;
};

} /* namespace lol */

