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

enum class TextAlign
{
    Left,
    Right,
    Center,
};

class Text : public Entity
{
public:
    Text(String const &text, char const *font);
    virtual ~Text();

    /** Set the text that will be displayed */
    void SetText(String const &text);
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

