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
// The Font class
// --------------
//

#include "engine/entity.h"

namespace lol
{

class FontData;

class Font : public entity
{
public:
    static Font *create(std::string const &path);
    static void destroy(Font *);

protected:
    Font(std::string const &path);
    ~Font();

    /* Inherited from entity */
    virtual std::string GetName() const;
    virtual void tick_draw(float seconds, Scene &scene);

public:
    /* New methods */
    void Print(Scene &scene, vec3 pos, std::string const &str, vec2 scale = vec2(1.0f), float spacing = 0.0f);
    ivec2 GetSize() const;

private:
    FontData *data;
};

} /* namespace lol */

