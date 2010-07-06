
#include <cstdio>
#include <malloc.h>

#include "map.h"
#include "layer.h"

Map::Map(char const *path) :
    layers(0),
    nlayers(0)
{
    char tmp[BUFSIZ];
    int firstgid = 0, width = 0, height = 0, level = 0, data = 0;

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
            if (--data == 0)
            {
                layers[nlayers] = new Layer(width, height, level, tmp);
                nlayers++;
            }
        }
        else if (sscanf(tmp, " <tileset firstgid=\"%i\"", &i) == 1)
        {
            firstgid = i;
            fprintf(stderr, "found tileset, firstgid %i\n", firstgid);
        }
        else if (sscanf(tmp, " <image source=\"%[^\"]\"", str) == 1)
        {
            fprintf(stderr, "image %s\n", str);
        }
        else if (sscanf(tmp, " <layer name=\"%c%i%c%*[^\"]\" width=\"%i\" height=\"%i\"",
                        &a, &i, &b, &j, &k) == 5)
        {
            fprintf(stderr, "%s layer, level %i, sublevel %c, %ix%i\n",
                    a == 'H' ? "horizontal" : "vertical", i, b, j, k);
            layers = (Layer **)realloc(layers, sizeof(Layer **) * (nlayers + 1));
            width = j;
            height = k;
            data = 2;
        }
        else
        {
            fprintf(stderr, ".");
        }
    }

    fclose(fp);

/*
    char tmp[1024];

    sprintf(tmp, "grep '\\(^   [^< ]\\|layer name\\)' %s | while read i; do echo \"$i\"; read i; echo -n \"$i\" | perl -MMIME::Base64 -ne 'print decode_base64($_)' | gunzip; done", path);

    FILE *fp = popen(tmp, "r");

    while (fp && !feof(fp))
    {
        int width, height;

        fscanf(fp, "<layer name=\"%[^\"]\" ", name);
        if (feof(fp))
            break;
        fscanf(fp, "width=\"%u\" ", &width);
        fscanf(fp, "height=\"%u\" ", &height);
        fgets(tmp, 1024, fp); // Ignore rest of line
        layers = (Layer **)realloc(layers, sizeof(Layer **) * (nlayers + 1));
        layers[nlayers] = new Layer(name, width, height, fp);
        nlayers++;
    }

    pclose(fp);
*/
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
    {
        int z = layers[i]->GetZ();

        for (int y = 0; y < 32; y++)
            for (int x = 0; x < 32; x++)
                tiler->AddTile(layers[i]->GetTile(x, y), x * 32, y * 32, z);
    }
}

