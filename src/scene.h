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
    virtual void Render(Scene& scene);

private:
};

class PrimitiveRenderer
{
    friend class Scene;

public:
    PrimitiveRenderer() { }
    virtual ~PrimitiveRenderer() { }
    virtual void Render(Scene& scene, PrimitiveSource* primitive);

private:
    bool m_fire_and_forget = false;
};

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
    void AddPrimitive(class PrimitiveSource* primitive);

    /* ============================== */
#   define _KEY_IDX (uintptr_t)key /* TOUKY: I don't like that. hash should be fixed to handle these custom stuff */
    /* ============================== */
private:
    ptrdiff_t HasPrimitiveSource(uintptr_t key);
    ptrdiff_t AddPrimitiveSource(uintptr_t key, class PrimitiveSource* source);
    void SetPrimitiveSource(ptrdiff_t index, uintptr_t key, class PrimitiveSource* source);
    void ReleasePrimitiveSource(ptrdiff_t index, uintptr_t key);
    void ReleaseAllPrimitiveSource(uintptr_t key);
public:
    /* === Primitive source stuff === */
    /* Returns the number of primitive source set to the given entity */
    template <typename T>
    ptrdiff_t HasPrimitiveSource(T* key)
    {
        ASSERT(key);
        return HasPrimitiveSource(_KEY_IDX);
    }
    /* Add a primitive sources linked to the given entity
     * Returns the slot number */
    template <typename T>
    ptrdiff_t AddPrimitiveSource(T* key, class PrimitiveSource* source)
    {
        ASSERT(key);
        return AddPrimitiveSource(_KEY_IDX, source);
    }
    /* Update the primitive source at index linked to the given entity
     * Deletes the old one
     * The slot is kept even if source == nullptr */
    template <typename T>
    void SetPrimitiveSource(ptrdiff_t index, T* key, class PrimitiveSource* source)
    {
        ASSERT(key);
        SetPrimitiveSource(index, _KEY_IDX, source);
    }
    /* Remove primitive source at index set to the given entity */
    template <typename T>
    void ReleasePrimitiveSource(ptrdiff_t index, T* key)
    {
        ASSERT(key);
        ReleasePrimitiveSource(index, _KEY_IDX);
    }
    /* Remove all primitive source set to the given entity */
    template <typename T>
    void ReleaseAllPrimitiveSource(T* key)
    {
        ASSERT(key);
        ReleaseAllPrimitiveSource(_KEY_IDX);
    }

private:
    ptrdiff_t HasPrimitiveRenderer(uintptr_t key);
    void AddPrimitiveRenderer(uintptr_t key, class PrimitiveRenderer* renderer);
    void SetPrimitiveRenderer(ptrdiff_t index, uintptr_t key, class PrimitiveRenderer* renderer);
    void ReleasePrimitiveRenderer(ptrdiff_t index, uintptr_t key);
    void ReleaseAllPrimitiveRenderer(uintptr_t key);
public:
    /* === Primitive renderer stuff === */
    /* Returns the number of primitive renderer set to the given entity */
    template <typename T>
    ptrdiff_t HasPrimitiveRenderer(T* key)
    {
        ASSERT(key);
        return HasPrimitiveRenderer(_KEY_IDX);
    }
    /* Add a primitive renderer linked to the given entity
     * The primitive is considered as Fire&Forget and
     * will be destroyed at the end of the frame */
    template <typename T>
    void AddPrimitiveRenderer(T* key, class PrimitiveRenderer* renderer)
    {
        ASSERT(key);
        AddPrimitiveRenderer(_KEY_IDX, renderer);
    }
    /* Update the primitive renderer linked to the given entity
     * Deletes the old one
     * Will assert if renderer == nullptr */
    template <typename T>
    void SetPrimitiveRenderer(ptrdiff_t index, T* key, class PrimitiveRenderer* renderer)
    {
        ASSERT(key && renderer);
        SetPrimitiveRenderer(index, _KEY_IDX, renderer);
    }
    /* Remove primitive renderer at index set to the given entity */
    template <typename T>
    void ReleasePrimitiveRenderer(ptrdiff_t index, T* key)
    {
        ASSERT(key);
        ReleasePrimitiveRenderer(index, _KEY_IDX);
    }
    /* Remove all primitive renderer set to the given entity */
    template <typename T>
    void ReleaseAllPrimitiveRenderer(T* key)
    {
        ASSERT(key);
        ReleaseAllPrimitiveRenderer(_KEY_IDX);
    }
    /* ============================== */
#   undef _KEY_IDX /* (uintptr_t)key *//* TOUKY: I don't like that. hash should be fixed to handle these custom stuff */
    /* ============================== */

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

