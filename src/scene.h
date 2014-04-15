//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Scene class
// ---------------
//

#if !defined __LOL_SCENE_H__
#define __LOL_SCENE_H__

#include <stdint.h>

#include "tileset.h"
#include "light.h"
#include "camera.h"
#include "mesh/primitive.h"

namespace lol
{

class SceneData;

class Scene
{
    friend class Video;

private:
    Scene(ivec2 size);
    ~Scene();

public:
    Camera *GetCamera();
    void PushCamera(Camera *cam);
    void PopCamera(Camera *cam);

    void Reset();

    void RenderPrimitives();
    void RenderTiles();
    void RenderLines(float seconds);

    /* New scenegraph */
    void AddPrimitive(Mesh *mesh, Shader *shader, mat4 const &matrix);

    /* FIXME: this should be deprecated -- it doesn't really match
     * the architecture we want to build */
    void AddTile(TileSet *tileset, int id, vec3 pos, int o, vec2 scale, float angle);
    void SetLineTime(float new_time=-1.f);
    void SetLineMask(int new_mask=0xFFFFFFFF);
    void SetLineSegmentSize(float new_segment_size=100000.f);
    float GetLineSegmentSize();
    void SetLineColor(vec4 new_color=vec4(1.f));
    vec4 GetLineColor();
    void AddLine(vec3 a, vec3 b, vec4 color);
    void AddLine(vec2 a, vec3 b, vec4 color, float az=-1.f);
    void AddLine(vec2 a, vec2 b, vec4 color, float az=-1.f, float bz=-1.f);

    void AddLight(Light *light);
    Array<Light *> const &GetLights() const;

private:
    SceneData *data;
};

extern Scene *g_scene;

} /* namespace lol */

#endif // __LOL_SCENE_H__

