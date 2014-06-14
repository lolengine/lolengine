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
// The Primitive class
// -------------------
//

#if !defined __MESH_PRIMITIVE_H__
#define __MESH_PRIMITIVE_H__

#include "mesh/mesh.h"

namespace lol
{

class Primitive
{
    friend class Scene;

public:
    Primitive(SubMesh *submesh, mat4 const &matrix);
    ~Primitive();

private:
    SubMesh *m_submesh;
    mat4 m_matrix;
};

} /* namespace lol */

#endif /* __MESH_PRIMITIVE_H__ */

