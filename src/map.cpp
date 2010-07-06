
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <malloc.h>
#include <ctype.h>

#include "map.h"
#include "layer.h"

Map::Map(char const *path) :
    layers(0),
    nlayers(0)
{
    char tmp[BUFSIZ];
    uint32_t *data = NULL;
    int width = 0, height = 0, level = 0, orientation = 0;
    int firstgid = 0, ntiles = 0;

    FILE *fp = fopen(path, "r");

    if (!fp)
        return;

    while (!feof(fp))
    {
        char str[1024];
        int i, j, k;
        char a, b;

        fgets(tmp, BUFSIZ, fp);

        if (data)
        {
            /* We are in the process of reading layer data. Only stop
             * when we have read the expected number of tiles. */
            char const *parser = tmp;
            while (ntiles < width * height)
            {
                data[ntiles++] = atoi(parser);
                while (isdigit(*parser))
                    parser++;
                if (*parser == ',')
                    parser++;
                if (!isdigit(*parser))
                    break;
            }

            if (ntiles == width * height)
            {
                layers[nlayers] = new Layer(width, height, level, data);
                nlayers++;
                data = NULL;
            }
        }
        else if (sscanf(tmp, " <tileset firstgid=\"%i\"", &i) == 1)
        {
            /* This is a tileset description. Remember its firstgid value. */
            firstgid = i;
        }
        else if (sscanf(tmp, " <image source=\"%[^\"]\"", str) == 1)
        {
            /* This is a tileset image file. Associate it with firstgid. */
        }
        else if (sscanf(tmp, " <layer name=\"%c%i%c%*[^\"]\" "
                        "width=\"%i\" height=\"%i\"", &a, &i, &b, &j, &k) == 5)
        {
            /* This is a layer description. Prepare to read the data. */
            layers = (Layer **)realloc(layers,
                                       sizeof(Layer **) * (nlayers + 1));
            orientation = toupper(a) == 'V' ? 1 : 0;
            width = j;
            height = k;
            ntiles = 0;
            data = (uint32_t *)malloc(width * height * sizeof(uint32_t));
        }
    }

    fclose(fp);
}

Map::~Map()
{
    for (int i = 0; i < nlayers; i++)
        delete layers[i];
    free(layers);
}

void Map::Draw(Tileset *tileset)
{
    for (int i = 0; i < nlayers; i++)
    {
        int z = layers[i]->GetZ();

        for (int y = 0; y < 32; y++)
            for (int x = 0; x < 32; x++)
                tileset->AddTile(layers[i]->GetTile(x, y), x * 32, y * 32, z);
    }
}

