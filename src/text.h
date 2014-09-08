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
// The Text class
// --------------
//

#include "entity.h"

namespace lol
{

class TextData;

class Text : public Entity
{
public:
    Text(String const &text, char const *font);
    virtual ~Text();

    void SetText(String const &text);
    void SetInt(int val);
    void SetPos(vec3 pos);
    vec3 GetPos();
    void SetAlign(int align);

    enum
    {
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_CENTER,
    };

protected:
    virtual void TickDraw(float seconds, Scene &scene);

private:
    TextData *data;
};

} /* namespace lol */

