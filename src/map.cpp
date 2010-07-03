
#include <cstdio>
#include <malloc.h>

#include "map.h"
#include "layer.h"

Map::Map(char const *path) :
    layers(0),
    nlayers(0)
{
    char tmp[1024];

    sprintf(tmp, "grep '\\(^   [^< ]\\|layer name\\)' %s | while read i; do echo \"$i\"; read i; echo -n \"$i\" | perl -MMIME::Base64 -ne 'print decode_base64($_)' | gunzip; done", path);

    FILE *fp = popen(tmp, "r");

    while (fp && !feof(fp))
    {
        char name[1024];
        int width, height;

        fscanf(fp, "<layer name=\"%[^\"]\" ", name);
        if (feof(fp))
            break;
        fscanf(fp, "width=\"%u\" ", &width);
        fscanf(fp, "height=\"%u\" ", &height);
        fgets(tmp, 1024, fp); // Ignore rest of line
        layers = (Layer **)realloc(layers, sizeof(Layer **) * (nlayers + 1));
        layers[nlayers] = new Layer(width, height, fp);
        nlayers++;
    }

    pclose(fp);
}

Map::~Map()
{
    for (int i = 0; i < nlayers; i++)
        delete layers[i];
    free(layers);
}

void Map::Draw(Tiler *tiler)
{
    for (int i = 0; i < nlayers; i++)
        for (int y = 0; y < 32; y++)
            for (int x = 0; x < 32; x++)
                tiler->AddTile(layers[i]->GetTile(x, y), x * 32, y * 32, i);
}

