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
// The Font class
// --------------
//

#include "engine/entity.h"

namespace lol
{

class FontData;

class Font : public Entity
{
public:
    Font(char const *path);
    ~Font();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual void TickDraw(float seconds, Scene &scene);

public:
    /* New methods */
    void Print(Scene &scene, vec3 pos, char const *str, vec2 scale = vec2(1.0f), float spacing = 0.0f);
    ivec2 GetSize() const;

private:
    FontData *data;
};

} /* namespace lol */

