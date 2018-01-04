//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Text class
// --------------
//

#include "engine/entity.h"

#include <string>

namespace lol
{

class TextData;

enum class TextAlign
{
    Left,
    Right,
    Center,
};

class Text : public Entity
{
public:
    Text(std::string const &text, char const *font);
    virtual ~Text();

    /** Set the text that will be displayed */
    void SetText(std::string const &text);
    void SetInt(int val);

    /** Set the position of the text object, in the 3D world. */
    void SetPos(vec3 pos);

    /** Set the text scaling */
    void SetScale(vec2 scale);

    /** Set the spacing between characters, as a fraction of character width */
    void SetSpacing(float spacing);

    /** Set the alignment method */
    void SetAlign(TextAlign align);

    vec3 GetPos();
    ivec2 GetFontSize();

protected:
    virtual void TickDraw(float seconds, Scene &scene);

private:
    TextData *data;
};

} /* namespace lol */

