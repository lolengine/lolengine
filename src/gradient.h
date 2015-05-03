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
// The Gradient class
// ------------------
//

#include "engine/worldentity.h"

namespace lol
{

class GradientData;

class Gradient : public WorldEntity
{
public:
    Gradient(vec3 aa, vec3 bb);
    virtual ~Gradient();

    char const *GetName() { return "<gradient>"; }

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, Scene &scene);

private:
    GradientData *data;
};

} /* namespace lol */

