//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Primitive class
// -------------------
//

namespace lol
{

class PrimitiveMesh : public PrimitiveRenderer
{
    friend class Scene;

public:
    PrimitiveMesh(std::shared_ptr<SubMesh> submesh, mat4 const &matrix);
    virtual ~PrimitiveMesh();
    virtual void Render(Scene& scene);

private:
    std::shared_ptr<SubMesh> m_submesh;
    mat4 m_matrix;
};

} // namespace lol
