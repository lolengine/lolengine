//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "lolgl.h"

using namespace std;

namespace lol
{

/* For some reason defining this in the .h leads to duplicate definitions
 * between the executable and the static library. */
template<> void VertexBuffer::AddStream<void>(int n, int index)
{
    (void)index;
    m_streams[n].size = 0;
}

void VertexBuffer::Initialize()
{
#if defined _XBOX || defined USE_D3D9
    
#endif
}

} /* namespace lol */

