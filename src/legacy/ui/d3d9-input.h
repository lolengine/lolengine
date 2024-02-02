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
// The D3d9Input class
// -------------------
//

#include "engine/entity.h"

namespace lol
{

class D3d9InputData;

class D3d9Input : public entity
{
public:
    D3d9Input();
    virtual ~D3d9Input();

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

private:
    D3d9InputData *m_data;
};

} /* namespace lol */

