//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The DebugRecord class
// ---------------------
//

#include "engine/entity.h"

namespace lol
{

class DebugRecordData;

class DebugRecord : public entity
{
public:
    DebugRecord(std::string const &path, float fps);
    virtual ~DebugRecord();

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

private:
    DebugRecordData *m_data;
};

} /* namespace lol */

