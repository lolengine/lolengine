//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cstdlib> /* free() */
#include <cstring> /* strdup() */

#include "core.h"

/*
 * Text implementation class
 */

class TextData
{
    friend class Text;

private:
    int font;
    char *text;
    int3 pos;
};

/*
 * Public Text class
 */

Text::Text(char const *text, char const *font)
  : data(new TextData())
{
    data->font = Forge::Register(font);
    data->text = text ? strdup(text) : NULL;
    data->pos = int3(0, 0, 0);

    drawgroup = DRAWGROUP_HUD;
}

void Text::SetText(char const *text)
{
    if (data->text)
        free(data->text);
    data->text = text ? strdup(text) : NULL;
}

void Text::SetInt(int val)
{
    if (data->text)
        free(data->text);
    char buf[128];
    sprintf(buf, "%i", val);
    data->text = strdup(buf);
}

void Text::SetPos(int3 pos)
{
    data->pos = pos;
}

void Text::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (data->text)
    {
        Font *font = Forge::GetFont(data->font);
        font->Print(data->pos, data->text);
    }
}

Text::~Text()
{
    if (data->text)
        free(data->text);
    Forge::Deregister(data->font);
    delete data;
}

