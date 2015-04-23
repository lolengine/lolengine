//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Scene class
// ---------------
//

#include <stdint.h>

#include "tileset.h"
#include "light.h"
#include "camera.h"
#include "mesh/mesh.h"

#define LOL_MAX_LIGHT_COUNT 8

namespace lol
{

class SceneData;

//-----------------------------------------------------------------------------
class Primitive
{
    friend class Scene;

public:
    Primitive() { }
    virtual ~Primitive() { }
    virtual void Render(Scene& scene) { }

private:
};

class PrimitiveSettings
{
    friend class Scene;

public:
    PrimitiveSettings() { }
    virtual ~PrimitiveSettings() { }
    virtual void ApplyTo(Primitive* primitive) const { UNUSED(primitive); }

private:
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class Scene
{
    friend class Video;
    
private:
    static array<Scene*> g_scenes;

    Scene(ivec2 size);
    ~Scene();

public:
    static void AddNew(ivec2 size);
private:
    //Private because I don't know if we should have it
    static void DestroyScene(Scene* scene);
private:
    static void DestroyAll();
public:
    static ptrdiff_t GetCount();
    static bool IsReady(ptrdiff_t index = 0);
    static Scene& GetScene(ptrdiff_t index = 0);

public:
    //TODO: don't like the name
    void Link(Entity* entity);
    bool IsRelevant(Entity* entity);

public:
    Camera* GetCamera(int cam_idx = -1);
    int PushCamera(Camera *cam);
    void PopCamera(Camera *cam);
    void SetTileCam(int cam_idx);

    void Reset();

    /* New scenegraph */
    void AddPrimitive(Mesh const &mesh, mat4 const &matrix);
    void AddPrimitive(Primitive* primitive);
    void AddPrimitive(Entity* entity, Primitive* primitive);

    /* FIXME: this should be deprecated -- it doesn't really match
     * the architecture we want to build */
    void AddTile(TileSet *tileset, int id, vec3 pos, int o, vec2 scale, float angle);

public:
    void SetLineTime(float new_time = -1.f);
    void SetLineMask(int new_mask = 0xFFFFFFFF);
    void SetLineSegmentSize(float new_segment_size = 100000.f);
    void SetLineColor(vec4 new_color = vec4(1.f));

    float GetLineSegmentSize();
    vec4 GetLineColor();
    void AddLine(vec3 a, vec3 b, vec4 color);

    void AddLight(Light *light);
    array<Light *> const &GetLights();

    void RenderPrimitives();
    void RenderTiles();
    void RenderLines(float seconds);

private:
    SceneData *data;
};

} /* namespace lol */

