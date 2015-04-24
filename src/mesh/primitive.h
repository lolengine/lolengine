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
// The Primitive class
// -------------------
//

namespace lol
{

class PrimitiveMesh : public PrimitiveSource
{
    friend class Scene;

public:
    PrimitiveMesh(SubMesh *submesh, mat4 const &matrix);
    virtual ~PrimitiveMesh();
    virtual void Render(Scene& scene);

private:
    SubMesh *m_submesh;
    mat4 m_matrix;
};

} /* namespace lol */

