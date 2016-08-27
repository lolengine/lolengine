//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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
class SceneDisplayData;

class SceneDisplay
{
    friend class Scene;

public:
    SceneDisplay() { }
    virtual ~SceneDisplay() { }

    /* pos/size/... methods */
    virtual void SetResolution(ivec2 resolution) { UNUSED(resolution); }
    virtual void SetPosition(ivec2 position) { UNUSED(position); }

    /* TODO: Should that be there or in Video ? */
    static void Add(SceneDisplay* display);
    static int GetCount();
    static SceneDisplay* GetDisplay(int index = 0);
    static void DestroyAll();

    /* Implement these in the platform section */
    static int GetPhysicalCount();
    static const char* GetPhysicalName(int index = 0);

//protected:
    virtual void Enable();
    virtual void Disable();

private:
    SceneDisplayData *data;
};

//-----------------------------------------------------------------------------
class SceneData;

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
    static int GetCount();
    static bool IsReady(int index = 0);
    static Scene& GetScene(int index = 0);

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

    /* ============================== */
#   define _KEY_IDX (uintptr_t)key /* TOUKY: I don't like that. hash should be fixed to handle these custom stuff */
    /* ============================== */
private:
    int HasPrimitiveSource(uintptr_t key);
    int AddPrimitiveSource(uintptr_t key, class PrimitiveSource* source);
    void SetPrimitiveSource(int index, uintptr_t key, class PrimitiveSource* source);
    void ReleasePrimitiveSource(int index, uintptr_t key);
    void ReleaseAllPrimitiveSources(uintptr_t key);
public:
    /* === Primitive source stuff === */
    /* Returns the number of primitive source set to the given entity */
    template <typename T>
    int HasPrimitiveSource(T* key)
    {
        ASSERT(key);
        return HasPrimitiveSource(_KEY_IDX);
    }
    /* Add a primitive sources linked to the given entity
     * Returns the slot number */
    template <typename T>
    int AddPrimitiveSource(T* key, class PrimitiveSource* source)
    {
        ASSERT(key);
        return AddPrimitiveSource(_KEY_IDX, source);
    }
    /* Update the primitive source at index linked to the given entity
     * Deletes the old one
     * The slot is kept even if source == nullptr */
    template <typename T>
    void SetPrimitiveSource(int index, T* key, class PrimitiveSource* source)
    {
        ASSERT(key);
        SetPrimitiveSource(index, _KEY_IDX, source);
    }
    /* Remove primitive source at index set to the given entity */
    template <typename T>
    void ReleasePrimitiveSource(int index, T* key)
    {
        ASSERT(key);
        ReleasePrimitiveSource(index, _KEY_IDX);
    }
    /* Remove all primitive source set to the given entity */
    template <typename T>
    void ReleaseAllPrimitiveSources(T* key)
    {
        ASSERT(key);
        ReleaseAllPrimitiveSources(_KEY_IDX);
    }

private:
    int HasPrimitiveRenderer(uintptr_t key);

    void AddPrimitiveRenderer(uintptr_t key, class PrimitiveRenderer* renderer);
    void SetPrimitiveRenderer(int index, uintptr_t key, class PrimitiveRenderer* renderer);
    void ReleasePrimitiveRenderer(int index, uintptr_t key);
    void ReleaseAllPrimitiveRenderers(uintptr_t key);
public:
    /* === Primitive renderer stuff === */
    /* Returns the number of primitive renderer set to the given entity */
    template <typename T>
    int HasPrimitiveRenderer(T* key)
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
    void SetPrimitiveRenderer(int index, T* key, class PrimitiveRenderer* renderer)
    {
        ASSERT(key && renderer);
        SetPrimitiveRenderer(index, _KEY_IDX, renderer);
    }
    /* Remove primitive renderer at index set to the given entity */
    template <typename T>
    void ReleasePrimitiveRenderer(int index, T* key)
    {
        ASSERT(key);
        ReleasePrimitiveRenderer(index, _KEY_IDX);
    }
    /* Remove all primitive renderer set to the given entity */
    template <typename T>
    void ReleaseAllPrimitiveRenderers(T* key)
    {
        ASSERT(key);
        ReleaseAllPrimitiveRenderers(_KEY_IDX);
    }
    /* ============================== */
#   undef _KEY_IDX /* (uintptr_t)key *//* TOUKY: I don't like that. hash should be fixed to handle these custom stuff */
    /* ============================== */

    /* FIXME: this should be deprecated -- it doesn't really match
     * the architecture we want to build */
    void AddTile(TileSet *tileset, int id, vec3 pos, int o, vec2 scale, float radians);
    void AddTile(TileSet *tileset, int id, mat4 model);

public:
    void SetLineTime(float new_time = -1.f);
    void SetLineMask(int new_mask = 0xFFFFFFFF);
    void SetLineSegmentSize(float new_segment_size = 1.f);
    void SetLineColor(vec4 new_color = vec4(1.f));

    float GetLineSegmentSize();
    vec4 GetLineColor();
    void AddLine(vec3 a, vec3 b, vec4 color);

    void AddLight(Light *light);
    array<Light *> const &GetLights();

    /* === Render stuff === */
    void SetDisplay(SceneDisplay* display);
    void EnableDisplay();
    void DisableDisplay();

    void render(float seconds);

private:
    void render_primitives();
    void render_tiles();
    void render_lines(float seconds);

    SceneData *data;
};

} /* namespace lol */

