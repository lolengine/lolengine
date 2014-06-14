//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdlib>

#include "core.h"

namespace lol
{

/*
 * Primitive class
 */

Primitive::Primitive(SubMesh *submesh, mat4 const &matrix)
  : m_submesh(submesh),
    m_matrix(matrix)
{
}

Primitive::~Primitive()
{
}

} /* namespace lol */

