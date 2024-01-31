//
//  Lol Engine
//
//  Copyright © 2010–2024 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lol/format> // std::format

namespace lol
{

/*
 * Text implementation class
 */

class TextData
{
    friend class Text;

private:
    Font *m_font;
    TextAlign m_align;
    std::string m_text;
    vec3 m_pos;
    vec2 m_scale;
    float m_spacing;
};

/*
 * Public Text class
 */

Text::Text(std::string const &text, char const *font)
  : data(new TextData())
{
    data->m_font = Font::create(font);
    data->m_align = TextAlign::Left;
    data->m_text = text;
    data->m_pos = vec3(0, 0, 0);
    data->m_scale = vec2(1.f);
    data->m_spacing = 0.f;

    m_drawgroup = tickable::group::draw::entity;
}

void Text::SetText(std::string const &text)
{
    data->m_text = text;
}

void Text::SetInt(int val)
{
    data->m_text = std::format("{}", val);
}

void Text::SetPos(vec3 pos)
{
    data->m_pos = pos;
}

void Text::SetScale(vec2 scale)
{
    data->m_scale = scale;
}

void Text::SetSpacing(float spacing)
{
    data->m_spacing = spacing;
}

void Text::SetAlign(TextAlign align)
{
    data->m_align = align;
}

vec3 Text::GetPos()
{
    return (vec3)data->m_pos;
}

ivec2 Text::GetFontSize()
{
    return data->m_font->GetSize();
}

void Text::tick_draw(float seconds, Scene &scene)
{
    entity::tick_draw(seconds, scene);

    if (auto length = data->m_text.length())
    {
        vec3 delta(0.0f);
        float text_width = ((length - 0.5f) + (length - 1) * data->m_spacing)
                         * data->m_font->GetSize().x;

        if (data->m_align == TextAlign::Right)
            delta.x -= text_width * data->m_scale.x;
        else if (data->m_align == TextAlign::Center)
            delta.x -= 0.5f * text_width * data->m_scale.x;

        data->m_font->Print(scene, data->m_pos + delta, data->m_text,
                            data->m_scale, data->m_spacing);
    }
}

Text::~Text()
{
    Font::destroy(data->m_font);
    delete data;
}

} /* namespace lol */

