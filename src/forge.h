//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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
    static int Register(std::string const &path);
    static void Deregister(int id);
    static Font *GetFont(int id);

private:
    Forge() {}
};

} /* namespace lol */

