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

int Forge::Register(std::string const &path)
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

