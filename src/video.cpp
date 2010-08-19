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

#include "video.h"

/*
 * Public Video class
 */

void Video::Setup(int width, int height)
{
    /* Initialise OpenGL */
    glViewport(0, 0, width, height);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Projection matrix: once and for all */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -(width + height), width + height);
}

void Video::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Model view matrix: for each frame, just in case */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Video::Capture(uint32_t *buffer)
{
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    int width = v[2], height = v[3];

    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

    for (int j = 0; j < height / 2; j++)
        for (int i = 0; i < width; i++)
        {
            uint32_t tmp = buffer[j * width + i];
            buffer[j * width + i] = buffer[(height - j - 1) * width + i];
            buffer[(height - j - 1) * width + i] = tmp;
        }
}

int Video::GetWidth()
{
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    return v[2];
}

int Video::GetHeight()
{
    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    return v[3];
}

