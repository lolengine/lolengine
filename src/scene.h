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
class PrimitiveSource
{
    friend class Scene;

public:
    PrimitiveSource() { }
    virtual ~PrimitiveSource() { }
    virtual void Render(Scene& scene) { }

private:
};

class PrimitiveRenderer
{
    friend class Scene;

public:
    PrimitiveRenderer() { }
    virtual ~PrimitiveRenderer() { }
    virtual void Render(PrimitiveSource* primitive) const { UNUSED(primitive); }

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
private: //Private because I don't know if we should have it
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
    void AddPrimitive(PrimitiveSource* primitive);

    /* === Primitive source stuff === */
    /* Returns the number of primitive source set to the given entity */
    ptrdiff_t HasPrimitiveSource(Entity* entity);
    /* Add a primitive sources linked to the given entity
     * Returns the slot number */
    ptrdiff_t AddPrimitiveSource(Entity* entity, PrimitiveSource* source);
    /* Update the primitive source at index linked to the given entity
     * Deletes the old one
     * The slot is kept even if source == nullptr */
    void SetPrimitiveSource(ptrdiff_t index, Entity* entity, PrimitiveSource* source);
    /* Remove primitive source at index set to the given entity */
    void ReleasePrimitiveSource(ptrdiff_t index, Entity* entity);
    /* Remove all primitive source set to the given entity */
    void ReleaseAllPrimitiveSource(Entity* entity);

    /* === Primitive renderer stuff === */
    /* Returns the number of primitive renderer set to the given entity */
    ptrdiff_t HasPrimitiveRenderer(Entity* entity);
    /* Add a primitive renderer linked to the given entity
     * Returns the slot number */
    ptrdiff_t AddPrimitiveRenderer(Entity* entity, PrimitiveRenderer* renderer);
    /* Update the primitive renderer linked to the given entity
     * Deletes the old one
     * Will assert if renderer == nullptr */
    void SetPrimitiveRenderer(ptrdiff_t index, Entity* entity, PrimitiveRenderer* renderer);
    /* Remove primitive renderer at index set to the given entity */
    void ReleasePrimitiveRenderer(ptrdiff_t index, Entity* entity);
    /* Remove all primitive renderer set to the given entity */
    void ReleaseAllPrimitiveRenderer(Entity* entity);


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

