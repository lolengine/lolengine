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
// The Gradient class
// ------------------
//

#include "engine/worldentity.h"

#include <memory>

namespace lol
{

class GradientData;

class Gradient : public WorldEntity
{
public:
    Gradient(vec3 aa, vec3 bb);
    virtual ~Gradient();

    std::string GetName() const { return "<gradient>"; }

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

private:
    std::unique_ptr<GradientData> data;
};

} /* namespace lol */

