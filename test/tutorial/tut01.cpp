//
// Lol Engine - Triangle tutorial
//
// Copyright: (c) 2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "lolgl.h"
#include "loldebug.h"

using namespace std;
using namespace lol;

#if defined _WIN32 && defined USE_D3D9
#   define FAR
#   define NEAR
#   include <d3d9.h>
#endif

#if USE_SDL && defined __APPLE__
#   include <SDL_main.h>
#endif

#if defined __native_client__
#   define main old_main
#endif

#if defined _WIN32
#   undef main /* FIXME: still needed? */
#   include <direct.h>
#endif

#if defined USE_D3D9
extern IDirect3DDevice9 *g_d3ddevice;
#elif defined _XBOX
extern D3DDevice *g_d3ddevice;
#endif

class Triangle : public WorldEntity
{
public:
    Triangle()
    {
        m_vertices[0] = vec2( 0.0,  0.8);
        m_vertices[1] = vec2(-0.8, -0.8);
        m_vertices[2] = vec2( 0.8, -0.8);
        m_ready = false;
    }

    virtual void TickDraw(float deltams)
    {
        WorldEntity::TickDraw(deltams);

        if (!m_ready)
        {
            VertexBuffer vb(VertexDeclaration<vec2, float, ivec3>(0, 0, 1));















            m_shader = Shader::Create(
#if !defined __CELLOS_LV2__ && !defined _XBOX && !defined USE_D3D9
                "#version 120\n"
                "attribute vec2 in_Position;"
                "void main(void) {"
                "    gl_Position = vec4(in_Position, 0.0, 1.0);"
                "}",

                "#version 120\n"
                "void main(void) {"
                "    gl_FragColor = vec4(0.7, 0.5, 0.2, 1.0);"
                "}"
#else
                "void main(float2 in_Position : POSITION,"
                "          out float4 out_Position : POSITION) {"
                "    out_Position = float4(in_Position, 0.0, 1.0);"
                "}",

                "void main(out float4 out_FragColor : COLOR) {"
                "    out_FragColor = float4(0.7, 0.5, 0.2, 1.0);"
                "}"
#endif
            );
#if !defined _XBOX && !defined USE_D3D9
            m_attrib = m_shader->GetAttribLocation("in_Position");
#endif
            m_ready = true;

#if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__ && !defined _XBOX && !defined USE_D3D9
            /* Method 1: store vertex buffer on the GPU memory */
            glGenBuffers(1, &m_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices,
                         GL_STATIC_DRAW);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__ && !defined _XBOX && !defined USE_D3D9
            /* Method 2: upload vertex information at each frame */
#elif defined _XBOX || defined USE_D3D9
            D3DVERTEXELEMENT9 const elements[2] =
            {
                { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                D3DDECL_END()
            };
            g_d3ddevice->CreateVertexDeclaration(elements, &m_vdecl);
            if (FAILED(g_d3ddevice->CreateVertexBuffer(sizeof(m_vertices), D3DUSAGE_WRITEONLY, NULL, D3DPOOL_MANAGED, &m_vbo, NULL)))
                exit(0);

            vec2 *vertices;
            if (FAILED(m_vbo->Lock(0, 0, (void **)&vertices, 0)))
                exit(0);
            memcpy(vertices, m_vertices, sizeof(m_vertices));
            m_vbo->Unlock();
#else
#endif

            /* FIXME: this object never cleans up */
        }

        m_shader->Bind();
#if defined _XBOX || defined USE_D3D9
        g_d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        g_d3ddevice->SetVertexDeclaration(m_vdecl);
        g_d3ddevice->SetStreamSource(0, m_vbo, 0, sizeof(*m_vertices));
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glEnableVertexAttribArray(m_attrib);
        glVertexAttribPointer(m_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        /* Never used for now */
        glEnableVertexAttribArray(m_attrib);
        glVertexAttribPointer(m_attrib, 2, GL_FLOAT, GL_FALSE, 0, m_vertices);
#else
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, m_vertices);
#endif

#if defined _XBOX || defined USE_D3D9
        g_d3ddevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
#else
        glDrawArrays(GL_TRIANGLES, 0, 3);
#endif

#if defined _XBOX || defined USE_D3D9
        /* FIXME: do we need to unset anything here? */
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        glDisableVertexAttribArray(m_attrib);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#elif !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
        /* Never used for now */
        glDisableVertexAttribArray(m_attrib);
#else
        glDisableClientState(GL_VERTEX_ARRAY);
#endif
    }

private:
    vec2 m_vertices[3];
    Shader *m_shader;
#if defined USE_D3D9
    IDirect3DVertexDeclaration9 *m_vdecl;
    IDirect3DVertexBuffer9 *m_vbo;
#elif defined _XBOX
    D3DVertexDeclaration *m_vdecl;
    D3DVertexBuffer *m_vbo;
#else
    int m_attrib;
#   if !defined __CELLOS_LV2__ && !defined __ANDROID__ && !defined __APPLE__
    GLuint m_vbo;
#   endif
#endif
    bool m_ready;
};

int main(int argc, char **argv)
{
    Application app("Tutorial 1: Triangle", ivec2(640, 480), 60.0f);

#if defined _MSC_VER && !defined _XBOX
    _chdir("..");
#elif defined _WIN32 && !defined _XBOX
    _chdir("../..");
#endif

    new DebugFps(5, 5);
    new Triangle();

    app.Run();
    return EXIT_SUCCESS;
}

