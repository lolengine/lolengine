//
// Lol Engine - EasyMesh tutorial
//
// Copyright: (c) 2011-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2012-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

using namespace std;
using namespace lol;
#define MESH_DIST 5.0f

class MeshViewer : public WorldEntity
{
public:
    MeshViewer()
    {
        int i=10;
        while (i--)
        {
            m_meshes.Push(EasyMesh());
            m_meshes.Last().Compile("[sc#0f0 ab 2 2 2 t .8 .8 .8 rx 20 ry 20 [sc#00f ab 2 2 2 tx 0 csgu]]");
        }

        m_angle = 0;

        m_camera = new Camera(vec3(0.f, 600.f, 0.f),
                              vec3(0.f, 0.f, 0.f),
                              vec3(0, 1, 0));
        m_camera->SetPerspective(60.f, 16, 9, .1f, 1000.f);
        m_camera->SetTarget(vec3(0.f, 0.f, 0.f));
        m_camera->SetPosition(vec3(0.f, 0.f, 5.f));
        m_camera->ForceSceneUpdate();
        Ticker::Ref(m_camera);
        min_pos = vec3(FLT_MAX);
        max_pos = vec3(-FLT_MAX);

        m_ready = false;
    }

    ~MeshViewer()
    {
        Ticker::Unref(m_camera);
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);
        //vec4 vertex = in_ModelView * vec4(in_Vertex, 1.0);
        //    gl_Position = in_Proj * vertex;


        m_angle += seconds * 70.0f;
        m_mat = mat4::rotate(m_angle, vec3(0, 1, 0));

        //mat4 screen_matrix = Scene::GetDefault()->GetProjMatrix() * Scene::GetDefault()->GetViewMatrix();
        mat4 world_view_matrix = m_camera->GetViewMatrix() * m_mat;
        mat4 world_screen_matrix = m_camera->GetProjMatrix() * world_view_matrix;
        mat4 view_world_matrix = inverse(m_camera->GetViewMatrix());
        mat4 screen_view_matrix = inverse(m_camera->GetProjMatrix());
        vec4 a(0, 2, 0, 1.0f);
        vec4 b(0,-2, 0, 1.0f);
        vec4 c(0, 0, -2, 1.0f);
        vec4 d(-1, -1, 1, 1.0f);
        
        //vec2 toto;
        //near plane : vec4(toto, 1.f, 1.f);
        //far plane : vec4(toto, -1.f, 1.f);

        a = vec4((world_screen_matrix * a).xyz / a.w, a.w);
        b = vec4((world_screen_matrix * b).xyz / b.w, b.w);
        c = vec4((inverse(world_screen_matrix) * c).xyz / c.w, c.w);
        d = vec4((inverse(world_screen_matrix) * d).xyz / d.w, d.w);
        a = b;
        c = d;

        //this is the algorithm for a camera that must keep n target in the screen
        {
            //Get the Min/Max needed
            vec3 new_min_pos(FLT_MAX);
            vec3 new_max_pos(-FLT_MAX);
            for (int i = 0; i < m_meshes.Last().GetVertexCount(); i++)
            {
                vec4 vpos = world_view_matrix * vec4(m_meshes.Last().GetVertexLocation(i), 1.0f);
                new_min_pos = min(vpos.xyz, new_min_pos);
                new_max_pos = max(vpos.xyz, new_max_pos);
            }

            //Actual camera algorithm
            {
                vec4 BottomLeft = m_camera->GetProjMatrix() * vec4(new_min_pos.xy, new_min_pos.z, 1.0f);
                vec4 TopRight = m_camera->GetProjMatrix() * vec4(new_max_pos.xy, new_min_pos.z, 1.0f);
                BottomLeft = vec4(BottomLeft.xyz / BottomLeft.w, BottomLeft.w);
                TopRight = vec4(TopRight.xyz / TopRight.w, TopRight.w);
                //vec2 NewSize = TopRight.xy - BottomLeft.xy;
                //NewSize.x = max(NewSize.x, NewSize.y) * 1.5f;
                vec4 DistantPoint = screen_view_matrix * vec4(vec2(1.0f, 1.0f), TopRight.z * TopRight.w, TopRight.w);

                vec3 vcenter = vec3(new_min_pos.xy + new_max_pos.xy, .0f) * .5f;
                vec4 new_pos = screen_view_matrix * vec4(.0f, .0f, new_min_pos.z, 1.0f);
                //vcenter.z += (new_pos.z - new_pos.z * NewSize.x);
                vcenter = (view_world_matrix * vec4(vcenter, 1.0f)).xyz;

                m_camera->SetPosition(damp(m_camera->GetPosition(), vcenter, 0.4f, seconds));
                //m_camera->SetPosition(vcenter);
                m_camera->SetTarget(m_camera->GetPosition() + vec3(0, 0, -5.0f));
            }
            //
        }
    }

    virtual void TickDraw(float seconds)
    {
        WorldEntity::TickDraw(seconds);

        if (!m_ready)
        {
            for (int i = 0; i < m_meshes.Count(); i++)
                m_meshes[i].MeshConvert();
            m_ready = true;
        }

        Video::SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        m_mat = mat4::translate(vec3(m_meshes.Count() * MESH_DIST * 0.5f, 0, m_meshes.Count() * MESH_DIST) * -1.0f) * m_mat;
        for (int i = 0; i < m_meshes.Count(); i++)
        {
            m_mat = mat4::translate(vec3(MESH_DIST * 0.5f, 0, MESH_DIST)) * m_mat;
            m_meshes[i].Render(m_mat);
            Video::Clear(ClearMask::Depth);
        }
        //m_mat = mat4::translate(vec3(.0f));
        //m_meshes.Last().Render(m_mat);
    }

private:
    //Array<EasyMesh, mat4, float> m_gears;
    float m_angle;
    mat4 m_mat;
    vec3 min_pos;
    vec3 max_pos;

    Array<EasyMesh> m_meshes;
    Camera *m_camera;

    bool m_ready;
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("MeshViewer", ivec2(960, 600), 60.0f);
    new MeshViewer();
    app.Run();

    return EXIT_SUCCESS;
}

