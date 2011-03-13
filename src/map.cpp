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
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctype.h>

#include "core.h"

namespace lol
{

/*
 * Map implementation class
 */

class MapData
{
    friend class Map;

    static int const MAX_TILERS = 128;

private:
    int tilers[MAX_TILERS];
    int ntilers;

    Layer **layers;
    int nlayers;

    int width, height;
};

/*
 * Public Map class
 */

Map::Map(char const *path)
  : data(new MapData())
{
    data->ntilers = 0;
    data->layers = NULL;
    data->nlayers = 0;
    data->width = 0;
    data->height = 0;

    char tmp[BUFSIZ];
    int gids[MapData::MAX_TILERS];
    uint32_t *tiles = NULL;
    int level = 0, orientation = 0, ntiles = 0;

    FILE *fp = fopen(path, "r");

    if (!fp)
        return;

    while (!feof(fp))
    {
        char str[1024];
        int i, j, k;
        char a, b;

        /* Read a line, then decide what to do with it. */
        fgets(tmp, BUFSIZ, fp);

        if (tiles && !strchr(tmp, '<'))
        {
            /* We are in the process of reading layer data. Only stop
             * when we have read the expected number of tiles. */
            char const *parser = tmp;
            while (ntiles < data->width * data->height)
            {
                uint32_t code = 0;
                int id = atoi(parser);
                if (id)
                {
                    for (int n = 0; n < data->ntilers; n++)
                    {
                        if (id < gids[n])
                            continue;
                        if (n == data->ntilers - 1
                             || id < gids[n + 1])
                        {
                            code = (data->tilers[n] << 16) | (id - gids[n]);
                            break;
                        }
                    }
                }

                int x = ntiles % data->width;
                int y = data->height - 1 - (ntiles / data->width);
                tiles[y * data->width + x] = code;
                ntiles++;

                while (isdigit(*parser))
                    parser++;
                if (*parser == ',')
                    parser++;
                if (!isdigit(*parser))
                    break;
            }

            if (ntiles == data->width * data->height)
            {
                Layer *l = new Layer(data->width, data->height,
                                     level, orientation, tiles);
                data->layers[data->nlayers] = l;
                data->nlayers++;
                tiles = NULL;
                //Log::Debug("new layer %ix%i\n", data->width, data->height);
            }
        }
        else if (sscanf(tmp, " <tileset firstgid=\"%i\"", &i) == 1)
        {
            /* This is a tileset description. Remember its first gid value. */
            gids[data->ntilers] = i;
        }
        else if (sscanf(tmp, " <image source=\"%[^\"]\"", str) == 1)
        {
            /* This is a tileset image file. Associate it with firstgid. */
            data->tilers[data->ntilers] = Tiler::Register(str, 32, 0,
                                                          sqrtf(2));
            data->ntilers++;
            //Log::Debug("new tiler %s\n", str);
        }
        else if (sscanf(tmp, " <layer name=\"%c%i%c%*[^\"]\" "
                        "width=\"%i\" height=\"%i\"", &a, &i, &b, &j, &k) == 5)
        {
            /* This is a layer description. Prepare to read the data. */
            data->layers = (Layer **)realloc(data->layers,
                                       sizeof(Layer **) * (data->nlayers + 1));
            orientation = toupper(a) == 'V' ? 1 : 0;
            level = i * 32;
            data->width = j;
            data->height = k;
            tiles = (uint32_t *)malloc(j * k * sizeof(uint32_t));
            ntiles = 0;
        }
    }

    fclose(fp);
}

Map::~Map()
{
    for (int i = 0; i < data->ntilers; i++)
        Tiler::Deregister(data->tilers[i]);
    for (int i = 0; i < data->nlayers; i++)
        delete data->layers[i];
    free(data->layers);
    delete data;
}

void Map::Render(int x, int y, int z)
{
    for (int i = 0; i < data->nlayers; i++)
        data->layers[i]->Render(x, y, z);
}

int Map::GetWidth()
{
    return data->width * 32;
}

int Map::GetHeight()
{
    return data->height * 32;
}

} /* namespace lol */

