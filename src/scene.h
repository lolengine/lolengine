//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
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

namespace lol
{

class SceneData;

class Scene
{
public:
    Scene();
    ~Scene();

    static Scene *GetDefault();

    void SetViewMatrix(mat4 const &m);
    void SetProjMatrix(mat4 const &m);
    mat4 const &GetViewMatrix(void);
    mat4 const &GetProjMatrix(void);

    void Reset();
    void Render();

    /* FIXME: this should be deprecated -- it doesn't really match
     * the architecture we want to build */
    void AddTile(TileSet *tileset, int id, vec3 pos, int o, vec2 scale);

private:
    SceneData *data;
};

} /* namespace lol */

#endif // __LOL_SCENE_H__

