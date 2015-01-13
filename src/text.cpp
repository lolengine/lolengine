//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstdio>
#include <cstdlib> /* free() */
#include <cstring> /* strdup() */

namespace lol
{

/*
 * Text implementation class
 */

class TextData
{
    friend class Text;

private:
    int font, align;
    String m_text;
    vec3 pos;
};

/*
 * Public Text class
 */

Text::Text(String const &text, char const *font)
  : data(new TextData())
{
    data->font = Forge::Register(font);
    data->m_text = text;
    data->pos = vec3(0, 0, 0);

    m_drawgroup = DRAWGROUP_HUD;
}

void Text::SetText(String const &text)
{
    data->m_text = text;
}

void Text::SetInt(int val)
{
    data->m_text = String::Printf("%i", val);
}

void Text::SetPos(vec3 pos)
{
    data->pos = pos;
}

vec3 Text::GetPos()
{
    return (vec3)data->pos;
}

void Text::SetAlign(int align)
{
    data->align = align;
}

ivec2 Text::GetFontSize()
{
    Font *font = Forge::GetFont(data->font);
    return font->GetSize();
}

void Text::TickDraw(float seconds, Scene &scene)
{
    Entity::TickDraw(seconds, scene);

    int length = data->m_text.count();
    if (length)
    {
        Font *font = Forge::GetFont(data->font);
        vec3 delta(0.0f);
        if (data->align == ALIGN_RIGHT)
            delta.x -= length * font->GetSize().x;
        else if (data->align == ALIGN_CENTER)
            delta.x -= length * font->GetSize().x / 2;
        font->Print(data->pos + delta, data->m_text.C());
    }
}

Text::~Text()
{
    Forge::Deregister(data->font);
    delete data;
}

} /* namespace lol */

