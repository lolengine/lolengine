//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

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

