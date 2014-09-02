//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

/*
 * Forge implementation class
 */

static class ForgeData
{
    friend class Forge;

public:
    Dict fonts;
}
forgedata;

static ForgeData * const data = &forgedata;

/*
 * Public Forge class
 */

int Forge::Register(char const *path)
{
    int id = data->fonts.MakeSlot(path);

    if (!data->fonts.GetEntity(id))
    {
        Font *font = new Font(path);
        data->fonts.SetEntity(id, font);
    }

    return id;
}

void Forge::Deregister(int id)
{
    data->fonts.RemoveSlot(id);
}

Font *Forge::GetFont(int id)
{
    return (Font *)data->fonts.GetEntity(id);
}

} /* namespace lol */

