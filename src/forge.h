//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Forge class
// ---------------
// The Forge is a static class that manages fonts.
//

#include "font.h"

namespace lol
{

class Forge
{
public:
    static int Register(char const *path);
    static void Deregister(int id);
    static Font *GetFont(int id);

private:
    Forge() {}
};

} /* namespace lol */

