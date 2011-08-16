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

#include <cmath>
#include <cstdio>
#include <cstring>

#include "core.h"
#include "lolgl.h"
#include "loldebug.h"

using namespace std;

namespace lol
{

/*
 * DebugQuad implementation class
 */

static int const NUM_ARRAYS = 10;
static int const NUM_BUFFERS = 20;
static int const NUM_ATTRS = 20;
static int const NUM_UNIFORMS = 20;
static int const NUM_SHADERS = 20;
static int const NUM_TEXTURES = 10;

static int const TEX_SIZE = 32;

class DebugQuadData
{
    friend class DebugQuad;

private:
    vec2 orig, step, aa, bb;

    int initialised;
    float time;
#if !defined __CELLOS_LV2__ && !defined ANDROID_NDK
    GLuint array[NUM_ARRAYS];
#endif
    GLuint buffer[NUM_BUFFERS];
    Shader *shader[NUM_SHADERS];
    GLuint attr[NUM_ATTRS];
    GLuint uni[NUM_UNIFORMS];
    GLuint texture[NUM_TEXTURES];
    uint8_t image[1][TEX_SIZE * TEX_SIZE * 4];

    GLfloat const *GetVertexArray()
    {
        GLfloat tmp[18] = { aa.x, bb.y, 0, bb.x, bb.y, 0, bb.x, aa.y, 0,
                            bb.x, aa.y, 0, aa.x, aa.y, 0, aa.x, bb.y, 0 };
        memcpy(vertices, tmp, sizeof(tmp));
        return vertices;
    }
    GLfloat vertices[18]; /* To cache quad coordinates */
};

/*
 * Public DebugQuad class
 */

DebugQuad::DebugQuad()
  : data(new DebugQuadData())
{
    data->initialised = 0;
    data->time = RandF(10000.0f);

    drawgroup = DRAWGROUP_HUD;
}

void DebugQuad::TickGame(float deltams)
{
    Entity::TickGame(deltams);

    data->time += deltams;
}

void DebugQuad::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (!data->initialised && !IsDestroying())
    {
#if !defined __CELLOS_LV2__ && !defined ANDROID_NDK
        glGenVertexArrays(NUM_ARRAYS, data->array);
#endif
        glGenBuffers(NUM_BUFFERS, data->buffer);
        glGenTextures(NUM_TEXTURES, data->texture);
        for (int i = 0; i < NUM_SHADERS; i++)
            data->shader[i] = NULL;

        /* Checkerboard texture */
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, data->texture[0]);
        for (int j = 0; j < TEX_SIZE; j++)
            for (int i = 0; i < TEX_SIZE; i++)
            {
                uint8_t wb = (((i / 2) ^ (j / 2)) & 1) * 0xff;
                data->image[0][(j * TEX_SIZE + i) * 4 + 0] = wb;
                data->image[0][(j * TEX_SIZE + i) * 4 + 1] = wb;
                data->image[0][(j * TEX_SIZE + i) * 4 + 2] = wb;
                data->image[0][(j * TEX_SIZE + i) * 4 + 3] = 0xff;
            }
        /* Use GL_RGBA instead of 4 for the internal format (Android) */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data->image[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        data->initialised = 1;
    }
    else if (data->initialised && IsDestroying())
    {
#if !defined __CELLOS_LV2__ && !defined ANDROID_NDK
        glDeleteVertexArrays(NUM_ARRAYS, data->array);
#endif
        glDeleteBuffers(NUM_BUFFERS, data->buffer);
        glDeleteTextures(NUM_TEXTURES, data->texture);

        for (int i = 0; i < NUM_SHADERS; i++)
            if (data->shader[i])
                Shader::Destroy(data->shader[i]);

        data->initialised = 0;
    }

    /* Prepare our quad coordinates */
    vec2i const layout(5, 4);
    data->step = vec2(2.0f, -2.0f) / (4 * layout + vec2i(1));
    data->orig = vec2(-1.0f, 1.0f) + data->step;
    data->aa = data->orig;
    data->bb = data->orig + 3.0f * data->step;

    /* Generate a few random numbers */
    float f1 = 0.5f + 0.5f * sinf(0.00034f * data->time);
    float f2 = 0.5f + 0.5f * sinf(0.00053f * data->time + 1.0f);
    float f3 = 0.5f + 0.5f * sinf(0.00072f * data->time + 4.0f);
    float f4 = 0.5f + 0.5f * sinf(0.00091f * data->time + 8.0f);

    GLfloat const colors[] = { f1, f2, f3, f4, f2, f1, f3, f1, f4,
                               f3, f1, f4, f4, f3, f2, f1, f2, f3 };
    GLfloat const texcoords[] = { f1, f3, f3, f2, f2, f4,
                                  f2, f4, f4, f1, f1, f3 };

    /* Cheap iterators */
#if !defined __CELLOS_LV2__ && !defined ANDROID_NDK
    GLuint *array = data->array;
#endif
    GLuint *buffer = data->buffer;
    Shader **shader = data->shader;
    GLuint *attr = data->attr;
    GLuint *uni = data->uni;

    ResetState();

    /*
     * Test #1: simple glBegin code
     *
     * Renders an orange square.
     */
#if defined HAVE_GLBEGIN || defined USE_GLEW
    glColor3f(0.8f, 0.5f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
#endif

    Advance();
    ResetState();

    /*
     * Test #2: glBegin + per-vertex coloring
     *
     * Renders a multicoloured square with varying colors.
     */
#if defined HAVE_GLBEGIN || defined USE_GLEW
    glBegin(GL_TRIANGLES);
        glColor3f(f1, f2, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glColor3f(f4, f2, f1);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glColor3f(f3, f1, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glColor3f(f4, f3, f2);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glColor3f(f1, f2, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
#endif

    Advance();
    ResetState();

    /*
     * Test #3: glBegin + texture
     *
     * Renders a multicoloured square with varying colors multiplied with an
     * animated distorted checkerboard.
     */
#if defined HAVE_GLBEGIN || defined USE_GLEW
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glColor3f(f1, f2, f3);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glColor3f(f4, f2, f1);
        glTexCoord2f(f3, f2);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glColor3f(f3, f1, f4);
        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glColor3f(f4, f3, f2);
        glTexCoord2f(f4, f1);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glColor3f(f1, f2, f3);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
#endif

    Advance();
    ResetState();

    /*
     * Test #4: glBegin + color in fragment shader
     *
     * Renders a static, coloured and tiled pattern.
     */
#if defined HAVE_GLBEGIN || defined USE_GLEW
    if (!shader[0])
        shader[0] = Shader::Create(
            "#version 110\n"
            "void main()"
            "{"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "void main()"
            "{"
            "    float dx = mod(gl_FragCoord.x * gl_FragCoord.y, 2.0);"
            "    float dy = mod(gl_FragCoord.x * 0.125, 1.0);"
            "    float dz = mod(gl_FragCoord.y * 0.125, 1.0);"
            "    gl_FragColor = vec4(dx, dy, dz, 1.0);"
            "}");
    shader[0]->Bind();
    shader++;
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
#endif

    Advance();
    ResetState();

    /*
     * Test #5: glBegin + pass vertex coord from vertex shader to fragment
     * shader for use as color information.
     *
     * Renders a multicoloured square with varying colors.
     */
#if defined HAVE_GLBEGIN || defined USE_GLEW
    if (!shader[0])
        shader[0] = Shader::Create(
            "#version 110\n"
            "varying vec4 pass_Color;"
            "void main()"
            "{"
            "    float r = gl_MultiTexCoord0.x;"
            "    float g = gl_MultiTexCoord0.y;"
            "    float b = gl_MultiTexCoord0.z;"
            "    pass_Color = vec4(r, g, b, 1.0);"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "varying vec4 pass_Color;"
            "void main()"
            "{"
            "    gl_FragColor = pass_Color;"
            "}");
    shader[0]->Bind();
    shader++;
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glTexCoord3f(f1, f2, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glTexCoord3f(f4, f2, f1);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glTexCoord3f(f3, f1, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glTexCoord3f(f4, f3, f2);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glTexCoord3f(f1, f2, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
#endif

    Advance();
    ResetState();

    /*
     * Test #6: glBegin + apply texture in fragment shader
     *
     * Renders an animated black-and-white distorted checkerboard with a
     * zoom ratio twice the one in test #3.
     *
     * Note: there is no need to glEnable(GL_TEXTURE_2D) when the
     * texture lookup is done in a shader.
     */
#if defined HAVE_GLBEGIN || defined USE_GLEW
    if (!shader[0])
        shader[0] = Shader::Create(
            "#version 110\n"
            "void main()"
            "{"
            "    gl_TexCoord[0] = gl_MultiTexCoord0;"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    gl_FragColor = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "}");
    shader[0]->Bind();
    shader++;
    glColor3f(0.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
        glTexCoord2f(f3, f2);
        glVertex3f(data->bb.x, data->bb.y, 0.0f);
        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);

        glTexCoord2f(f2, f4);
        glVertex3f(data->bb.x, data->aa.y, 0.0f);
        glTexCoord2f(f4, f1);
        glVertex3f(data->aa.x, data->aa.y, 0.0f);
        glTexCoord2f(f1, f3);
        glVertex3f(data->aa.x, data->bb.y, 0.0f);
    glEnd();
#endif

    Advance();
    ResetState();

    /*
     * Test #7: simple vertex buffer
     *
     * Renders an orange square.
     */
#if !defined ANDROID_NDK
    glColor4f(0.8f, 0.5f, 0.2f, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, data->GetVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
#endif

    Advance();
    ResetState();

    /*
     * Test #8: vertex buffer + per-vertex coloring
     *
     * Renders a multicoloured square with varying colors.
     */
#if !defined ANDROID_NDK
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, data->GetVertexArray());
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
#endif

    Advance();
    ResetState();

    /*
     * Test #9: vertex buffer + per-vertex coloring + texture
     *
     * Renders a multicoloured square with varying colors multiplied with an
     * animated distorted checkerboard.
     */
#if !defined ANDROID_NDK
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, data->GetVertexArray());
    glColorPointer(3, GL_FLOAT, 0, colors);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
#endif

    Advance();
    ResetState();

    /*
     * Test #10: vertex buffer + hardcoded color in 1.10 fragment shader
     * (GLSL) or in Cg fragment shader (PS3)
     *
     * Renders an orange square.
     */
#if !defined ANDROID_NDK
    if (!shader[0])
#if !defined __CELLOS_LV2__
        shader[0] = Shader::Create(
            "#version 110\n"
            "void main()"
            "{"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "void main()"
            "{"
            "    gl_FragColor = vec4(0.8, 0.5, 0.2, 1.0);"
            "}");
#else
        shader[0] = Shader::Create(
            "void main(float4 in_Position : POSITION,"
            "          out float4 out_Position : POSITION)"
            "{"
            "    out_Position = in_Position;"
            "}",

            "void main(out float4 out_FragColor : COLOR)"
            "{"
            "    out_FragColor = float4(0.8, 0.5, 0.2, 1.0);"
            "}");
#endif
    shader[0]->Bind();
    shader++;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, data->GetVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);
#endif

    Advance();
    ResetState();

    /*
     * Test #11: vertex buffer + uniform color in 1.10 fragment shader
     * (GLSL) or in Cg fragment shader (PS3)
     *
     * Renders an orange square.
     */
#if !defined ANDROID_NDK
    if (!shader[0])
    {
#if !defined __CELLOS_LV2__
        shader[0] = Shader::Create(
            "#version 110\n"
            "void main()"
            "{"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "uniform vec4 in_Color;"
            "void main()"
            "{"
            "    gl_FragColor = in_Color;"
            "}");
#else
        shader[0] = Shader::Create(
            "void main(float4 in_Position : POSITION,"
            "          out float4 out_Position : POSITION)"
            "{"
            "    out_Position = in_Position;"
            "}",

            "uniform float4 in_Color;"
            "void main(out float4 out_FragColor : COLOR)"
            "{"
            "    out_FragColor = in_Color;"
            "}");
#endif
        uni[0] = shader[0]->GetUniformLocation("in_Color");
    }
    shader[0]->Bind();
    shader++;
#if !defined __CELLOS_LV2__
    glUniform4f(uni[0], 0.8f, 0.5f, 0.2f, 1.0f);
#else
    cgGLSetParameter4f((CGparameter)(intptr_t)uni[0], 0.8f, 0.5f, 0.2f, 1.0f);
#endif
    uni++;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, data->GetVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);
#endif

    Advance();
    ResetState();

    /*
     * Test #12: vertex buffer + color in 1.10 fragment shader (GLSL) or
     * in Cg fragment shader (PS3)
     *
     * Renders a static, coloured and tiled pattern.
     */
#if !defined ANDROID_NDK
    if (!shader[0])
#if !defined __CELLOS_LV2__
        shader[0] = Shader::Create(
            "#version 110\n"
            "void main()"
            "{"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "void main()"
            "{"
            "    float dx = mod(gl_FragCoord.x * gl_FragCoord.y, 2.0);"
            "    float dy = mod(gl_FragCoord.x * 0.125, 1.0);"
            "    float dz = mod(gl_FragCoord.y * 0.125, 1.0);"
            "    gl_FragColor = vec4(dx, dy, dz, 1.0);"
            "}");
#else
        shader[0] = Shader::Create(
            "void main(float4 in_Position : POSITION,"
            "          out float4 out_Position : POSITION)"
            "{"
            "    out_Position = in_Position;"
            "}",

            "void main(float4 in_FragCoord : WPOS,"
            "          out float4 out_FragColor : COLOR)"
            "{"
            "    float dx = frac(in_FragCoord.x * in_FragCoord.y * 0.5) * 2.0;"
            "    float dy = frac(in_FragCoord.x * 0.125);"
            "    float dz = frac(in_FragCoord.y * 0.125);"
            "    out_FragColor = float4(dx, dy, dz, 1.0);"
            "}");
#endif
    shader[0]->Bind();
    shader++;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, data->GetVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);
#endif

    Advance();
    ResetState();

    /*
     * Test #13: vertex buffer + texture & color in 1.10 fragment shader
     *
     * Renders a multicoloured square with varying colors xored with an
     * animated distorted checkerboard.
     */
#if !defined ANDROID_NDK
    if (!shader[0])
#if !defined __CELLOS_LV2__
        shader[0] = Shader::Create(
            "#version 110\n"
            "varying vec4 pass_Color;"
            "void main()"
            "{"
            "    gl_TexCoord[0] = gl_MultiTexCoord0;"
            "    pass_Color = gl_Color;"
            "    gl_Position = gl_Vertex;"
            "}",

            "#version 110\n"
            "varying vec4 pass_Color;"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    vec4 tmp = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "    gl_FragColor = vec4(abs(tmp.xyz - pass_Color.xyz), 1.0);"
            "}");
#else
        shader[0] = Shader::Create(
            "void main(float4 in_Position : POSITION,"
            "          float2 in_TexCoord : TEXCOORD0,"
            "          float4 in_Color : COLOR,"
            "          out float4 out_Color : COLOR,"
            "          out float4 out_Position : POSITION,"
            "          out float2 out_TexCoord : TEXCOORD0)"
            "{"
            "    out_TexCoord = in_TexCoord;"
            "    out_Color = in_Color;"
            "    out_Position = in_Position;"
            "}",

            "void main(float2 in_TexCoord : TEXCOORD0,"
            "          float4 in_Color : COLOR,"
            "          uniform sampler2D tex,"
            "          out float4 out_FragColor : COLOR)"
            "{"
            "    float4 tmp = tex2D(tex, in_TexCoord * 0.25);"
            "    out_FragColor = float4(abs(tmp.xyz - in_Color.xyz), 1);"
            "}");
#endif

    shader[0]->Bind();
    shader++;

    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, data->GetVertexArray());
    glColorPointer(3, GL_FLOAT, 0, colors);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

    Advance();
    ResetState();

    /*
     * Test #14: vertex buffer + texture & color in 1.20 fragment shader
     *
     * Renders a multicoloured square with varying colors xored with an
     * animated distorted checkerboard.
     */
#if !defined __CELLOS_LV2__ && !defined ANDROID_NDK
    if (!shader[0])
    {
        shader[0] = Shader::Create(
            "#version 120\n"
            "attribute vec3 in_Vertex;"
            "attribute vec3 in_Color;"
            "attribute vec2 in_MultiTexCoord0;"
            "varying vec4 pass_Color;"
            "void main()"
            "{"
            "    gl_TexCoord[0] = vec4(in_MultiTexCoord0, 0.0, 0.0);"
            "    pass_Color = vec4(in_Color, 1.0);"
            "    gl_Position = vec4(in_Vertex, 1.0);"
            "}",

            "#version 120\n"
            "varying vec4 pass_Color;"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    vec4 tmp = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "    gl_FragColor = vec4(abs(tmp.xyz - pass_Color.xyz), 1.0);"
            "}");
        attr[0] = shader[0]->GetAttribLocation("in_Vertex");
        attr[1] = shader[0]->GetAttribLocation("in_Color");
        attr[2] = shader[0]->GetAttribLocation("in_MultiTexCoord0");
    }
    shader[0]->Bind();
    shader++;

    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

    glBindVertexArray(*array++);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer++);
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(GLfloat),
                 data->GetVertexArray(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attr[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attr[0]);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer++);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attr[1], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attr[1]);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer++);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attr[2], 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attr[2]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisableVertexAttribArray(*attr++);
    glDisableVertexAttribArray(*attr++);
    glDisableVertexAttribArray(*attr++);
#endif

    Advance();
    ResetState();

    /*
     * Test #15: vertex buffer + texture & color in 1.30 fragment shader
     *
     * Renders a multicoloured square with varying colors xored with an
     * animated distorted checkerboard.
     */
#if !defined __CELLOS_LV2__ && !defined ANDROID_NDK
    if (!shader[0])
    {
        shader[0] = Shader::Create(
            "#version 130\n"
            "in vec3 in_Vertex;"
            "in vec3 in_Color;"
            "in vec2 in_MultiTexCoord0;"
            "out vec4 pass_Color;"
            "void main()"
            "{"
            "    gl_TexCoord[0] = vec4(in_MultiTexCoord0, 0.0, 0.0);"
            "    pass_Color = vec4(in_Color, 1.0);"
            "    gl_Position = vec4(in_Vertex, 1.0);"
            "}",

            "#version 130\n"
            "in vec4 pass_Color;"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    vec4 tmp = texture2D(tex, gl_TexCoord[0].xy * 0.25);"
            "    gl_FragColor = vec4(abs(tmp.xyz - pass_Color.xyz), 1.0);"
            "}");
        attr[0] = shader[0]->GetAttribLocation("in_Vertex");
        attr[1] = shader[0]->GetAttribLocation("in_Color");
        attr[2] = shader[0]->GetAttribLocation("in_MultiTexCoord0");
    }
    shader[0]->Bind();
    shader++;

    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

    glBindVertexArray(*array++);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer++);
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(GLfloat),
                 data->GetVertexArray(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attr[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attr[0]);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer++);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attr[1], 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attr[1]);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer++);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attr[2], 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attr[2]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisableVertexAttribArray(*attr++);
    glDisableVertexAttribArray(*attr++);
    glDisableVertexAttribArray(*attr++);
#endif

    Advance();
    ResetState();

    /* Check that we didn't overflow our list */
#if !defined __CELLOS_LV2__ && !defined ANDROID_NDK
    if (array > data->array + NUM_ARRAYS)
        Log::Error("too many arrays used\n");
#endif
    if (buffer > data->buffer + NUM_BUFFERS)
        Log::Error("too many buffers used\n");
    if (shader > data->shader + NUM_SHADERS)
        Log::Error("too many shaders used\n");
    if (attr > data->attr + NUM_ATTRS)
        Log::Error("too many attributes used\n");
}

void DebugQuad::ResetState()
{
    /* Reset GL states to something reasonably safe */

#if defined HAVE_GLBEGIN || defined USE_GLEW || defined __CELLOS_LV2__
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
#if defined HAVE_GLBEGIN || defined USE_GLEW || defined __CELLOS_LV2__
    glClientActiveTexture(GL_TEXTURE0);
#endif
    glDisable(GL_TEXTURE_2D);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
#if !defined __CELLOS_LV2__
    glUseProgram(0);
#else
    cgGLDisableProfile(cgGLGetLatestProfile(CG_GL_VERTEX));
    cgGLDisableProfile(cgGLGetLatestProfile(CG_GL_FRAGMENT));
#endif
}

void DebugQuad::Advance()
{
    data->aa.x += 4.0f * data->step.x;
    data->bb.x += 4.0f * data->step.x;
    if (data->bb.x > 1.0f)
    {
        data->aa.x = data->orig.x;
        data->bb.x = data->orig.x + 3.0f * data->step.x;
        data->aa.y += 4.0f * data->step.y;
        data->bb.y += 4.0f * data->step.y;
    }
}

DebugQuad::~DebugQuad()
{
    delete data;
}

} /* namespace lol */

