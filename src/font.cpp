//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#endif

#include <SDL.h>
#include <SDL_image.h>

#include "font.h"

/*
 * Font implementation class
 */

class FontData
{
    friend class Font;

private:
    char *name;

    SDL_Surface *img;
    GLuint texture[1];
};

/*
 * Public Font class
 */

Font::Font(char const *path)
{
    data = new FontData();
    data->name = strdup(path);
    data->img = NULL;

    for (char const *name = path; *name; name++)
        if ((data->img = IMG_Load(name)))
            break;

    if (!data->img)
    {
        SDL_Quit();
        exit(1);
    }

    glGenTextures(1, data->texture);
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, data->img->w, data->img->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data->img->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

Font::~Font()
{
    glDeleteTextures(1, data->texture);
    SDL_FreeSurface(data->img);

    delete data;
}

Asset::Group Font::GetGroup()
{
    return GROUP_BEFORE;
}

void Font::TickRender(float delta_time)
{
    Asset::TickRender(delta_time);
}

char const *Font::GetName()
{
    return data->name;
}

void Font::Print(int x, int y, char const *str)
{
    int w = data->img->w / 16;
    int h = data->img->h / 16;

    glBindTexture(GL_TEXTURE_2D, data->texture[0]);
    glBegin(GL_QUADS);
    while (*str)
    {
        uint32_t ch = (uint8_t)*str++;
        float tx = .0625f * (ch & 0xf);
        float ty = .0625f * ((ch >> 4) & 0xf);

        glTexCoord2f(tx, ty);
        glVertex2f(x, y);
        glTexCoord2f(tx + .0625f, ty);
        glVertex2f(x + w, y);
        glTexCoord2f(tx + .0625f, ty + .0625f);
        glVertex2f(x + w, y + h);
        glTexCoord2f(tx, ty + .0625f);
        glVertex2f(x, y + h);

        x += w;
    }
    glEnd();
}

void Font::PrintBold(int x, int y, char const *str)
{
    Print(x, y, str);
    Print(x + 1, y, str);
    Print(x, y + 1, str);
}

