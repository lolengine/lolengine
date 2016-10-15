//
//  Lol Engine — Cube tutorial
//
//  Copyright © 2011—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <set>

#include <lol/engine.h>
#include "loldebug.h"

using namespace lol;

LOLFX_RESOURCE_DECLARE(15_scenegraph);

class SceneGraphNode : public std::enable_shared_from_this<SceneGraphNode>, public Entity
{
public:
    SceneGraphNode(lol::mat4 _transformationMatrix = lol::mat4(1.0f)) :
        transformationMatrix(_transformationMatrix)
    {
        Ticker::Ref(this);
    }

    std::shared_ptr<SceneGraphNode> createChild(lol::mat4 _transformationMatrix = lol::mat4(1.0f))
    {
        std::shared_ptr<SceneGraphNode> child = std::make_shared<SceneGraphNode>(_transformationMatrix);
        children.insert(child);
        child->parent = shared_from_this();

        return child;
    }

    bool removeChild(std::shared_ptr<SceneGraphNode> node)
    {
        bool ret = false;

        if (children.find(node) != children.end())
        {
            children.erase(node);
            ret = true;
        }

        return ret;
    }

    lol::mat4 getTransformation(bool cascade = false) const
    {
        lol::mat4 result = transformationMatrix;

        if (cascade && parent.lock())
        {
            result = parent.lock()->getTransformation(cascade) * result;
        }

        return result;
    }

    void setTransformation(lol::mat4 const & transformation)
    {
        transformationMatrix = transformation;
    }

    bool hasParent()
    {
        return bool(parent.lock());
    }

    virtual void TickGame(float seconds)
    {
        Entity::TickGame(seconds);

        transformationMatrix *= mat4::rotate(seconds, vec3(0, 0, 1));
    }


private:

    lol::mat4 transformationMatrix;
    std::weak_ptr<SceneGraphNode> parent;
    std::set<std::shared_ptr<SceneGraphNode>> children;
};

class Triangle : public WorldEntity
{
public:
    Triangle(std::shared_ptr<SceneGraphNode> node)
      : m_mesh({
            { vec3( -1.0f, 0.0f, 0.0f), vec3(1.0, 0.0, 1.0) },
            { vec3(  1.0f, 0.0f, 0.0f), vec3(0.0, 1.0, 0.0) },
            { vec3(  0.0f, 2.0f, 0.0f), vec3(1.0, 0.5, 0.0) } }),
        m_ready(false),
        m_node(node)
    {
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        if (!m_ready)
        {
            m_shader = Shader::Create(LOLFX_RESOURCE_NAME(15_scenegraph));
            m_coord = m_shader->GetAttribLocation(VertexUsage::Position, 0);
            m_color = m_shader->GetAttribLocation(VertexUsage::Color, 0);
            m_projection_matrix = m_shader->GetUniformLocation("u_projection_matrix");
            m_view_matrix = m_shader->GetUniformLocation("u_view_matrix");
            m_model_matrix = m_shader->GetUniformLocation("u_model_matrix");

            m_vdecl = new VertexDeclaration(VertexStream<vec3, vec3>(VertexUsage::Position, VertexUsage::Color));

            m_vbo = new VertexBuffer(m_mesh.bytes());
            void *vertices = m_vbo->Lock(0, 0);
            memcpy(vertices, &m_mesh[0], m_mesh.bytes());
            m_vbo->Unlock();

            m_ready = true;

            /* FIXME: this object never cleans up */
        }

        m_shader->Bind();
        m_vdecl->SetStream(m_vbo, m_coord, m_color);
        m_vdecl->Bind();

        m_shader->SetUniform(m_projection_matrix, Scene::GetScene().GetCamera()->GetProjection());
        m_shader->SetUniform(m_view_matrix, Scene::GetScene().GetCamera()->GetView());
        m_shader->SetUniform(m_model_matrix, m_node->getTransformation(true));

        m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, 3);
        m_vdecl->Unbind();
    }

private:
    array<vec3, vec3> m_mesh;
    Shader *m_shader;
    ShaderAttrib m_coord, m_color;
    ShaderUniform m_projection_matrix, m_view_matrix, m_model_matrix;
    VertexDeclaration *m_vdecl;
    VertexBuffer *m_vbo;
    bool m_ready;
    std::shared_ptr<SceneGraphNode> m_node;
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    Application app("Tutorial 15: Scenegraph", ivec2(640, 480), 60.0f);

    std::shared_ptr<SceneGraphNode> node1 = std::make_shared<SceneGraphNode>(mat4{
        vec4(0.1f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.1f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 0.1f, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f) });
    new Triangle(node1);

    std::shared_ptr<SceneGraphNode> node2 = node1->createChild(mat4{
        vec4(1.0f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 1.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(0.0f, 2.0f, 0.0f, 1.0f) });
    new Triangle(node2);

    std::shared_ptr<SceneGraphNode> current = node2;
    std::shared_ptr<SceneGraphNode> next;
    for (int i = 0 ; i < 8 ; ++i)
    {
        next = current->createChild(current->getTransformation());
        new Triangle(next);
        current = next;
    }

    new DebugFps(5, 5);

    Camera * m_camera = new Camera();
    m_camera->SetProjection(mat4::perspective(radians(90.f), 960.f, 600.f, .1f, 1000.f));
    m_camera->SetView(mat4::lookat(vec3(0.f, 0.f, 1.f),
                                   vec3(0.f, 0.f, 0.f),
                                   vec3(0.f, 1.f, 0.f)));

    Scene& scene = Scene::GetScene();
    scene.PushCamera(m_camera);


    app.Run();

    return EXIT_SUCCESS;
}

