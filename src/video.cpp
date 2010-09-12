//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <math.h>

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

#include "core.h"

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

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Video::SetFov(float theta)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float width = GetWidth();
    float height = GetHeight();
    float near = -width - height;
    float far = width + height;

    /* Set the projection matrix */
    if (theta < 1e-4f)
    {
        /* The easy way: purely orthogonal projection. */
        glOrtho(0, width, 0, height, near, far);
    }
    else
    {
        /* Compute a view that approximates the glOrtho view when theta
         * approaches zero. This view ensures that the z=0 plane fills
         * the screen. */
        float t1 = tanf(theta / 2);
        float t2 = t1 * height / width;
        float dist = (float)width / (2.0f * t1);

        near += dist;
        far += dist;

        if (near <= 0.0f)
        {
            far -= (near - 1.0f);
            near = 1.0f;
        }

        glFrustum(-near * t1, near * t1, -near * t2, near * t2, near, far);
        glTranslatef(-0.5f * width, -0.5f * height, -dist);
    }

    /* Reset the model view matrix, just in case */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Video::SetDepth(bool set)
{
    if (set)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void Video::Clear()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.01f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SetFov(0.0f);
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

