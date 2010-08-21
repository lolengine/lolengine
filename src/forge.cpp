//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "core.h"

#if defined WIN32
#   define strcasecmp _stricmp
#endif

/*
 * Forge implementation class
 */

static class ForgeData
{
    friend class Forge;

public:
    ForgeData() :
        fonts(0),
        nfonts(0)
    {
        /* Nothing to do */
    }

    ~ForgeData()
    {
        if (nfonts)
            fprintf(stderr, "ERROR: still %i fonts in forge\n", nfonts);
        free(fonts);
    }

private:
    Font **fonts;
    int nfonts;
}
forgedata;

static ForgeData * const data = &forgedata;

/*
 * Public Forge class
 */

Font *Forge::GetFont(char const *path)
{
    int id, empty = -1;

    /* If the font is already registered, remember its ID. Look for an
     * empty slot at the same time. */
    for (id = 0; id < data->nfonts; id++)
    {
        Font *t = data->fonts[id];
        if (!t)
            empty = id;
        else if (!strcasecmp(path, t->GetName()))
            break;
    }

    /* If this is a new font, create a new one. */
    if (id == data->nfonts)
    {
        if (empty == -1)
        {
            empty = data->nfonts++;
            data->fonts = (Font **)realloc(data->fonts,
                                           data->nfonts * sizeof(Font *));
        }

        data->fonts[empty] = new Font(path);
        id = empty;
    }

    data->fonts[id]->Ref();
    return data->fonts[id];
}

void Forge::ReleaseFont(Font *font)
{
    if (font->Unref() == 0)
        for (int id = 0; id < data->nfonts; id++)
            if (font == data->fonts[id])
            {
                data->fonts[id] = NULL;
                break;
            }
}

