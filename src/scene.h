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
    virtual void Render() const { }

private:
};

//-----------------------------------------------------------------------------
class Scene
{
    friend class Video;
    friend class TickerData; //TODO: Smells shitty

private:
    static Scene *g_scene;

    Scene(ivec2 size);
    ~Scene();

    static bool GetScene(Scene*& scene);
    static bool GetSceneData(SceneData*& data);

public:
    static bool IsReady();

    static Camera *GetCamera(int cam_idx=-1);
private:
    static int PushCamera(Scene* scene, Camera *cam);
    static void PopCamera(Scene* scene, Camera *cam);
public:
    static int PushCamera(Camera *cam);
    static void PopCamera(Camera *cam);
    static void SetTileCam(int cam_idx);

    static void Reset();

    /* New scenegraph */
    void AddPrimitive(Mesh const &mesh, mat4 const &matrix);
    void AddPrimitive(Primitive* primitive);

    /* FIXME: this should be deprecated -- it doesn't really match
     * the architecture we want to build */
    void AddTile(TileSet *tileset, int id, vec3 pos, int o, vec2 scale, float angle);

private:
    static void SetLineTime(Scene* scene, float new_time = -1.f);
    static void SetLineMask(Scene* scene, int new_mask = 0xFFFFFFFF);
    static void SetLineSegmentSize(Scene* scene, float new_segment_size = 100000.f);
    static void SetLineColor(Scene* scene, vec4 new_color = vec4(1.f));
public:
    static void SetLineTime(float new_time = -1.f);
    static void SetLineMask(int new_mask = 0xFFFFFFFF);
    static void SetLineSegmentSize(float new_segment_size = 100000.f);
    static void SetLineColor(vec4 new_color = vec4(1.f));

    static float GetLineSegmentSize();
    static vec4 GetLineColor();
    static void AddLine(vec3 a, vec3 b, vec4 color);

    static void AddLight(Light *light);
    static array<Light *> const &GetLights();

    static void RenderPrimitives();
    static void RenderTiles();
    static void RenderLines(float seconds);

private:
    SceneData *data;
};

} /* namespace lol */

