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
// The EglApp class
// ----------------
//

#include <lol/vector>

#include "application/application.h"

namespace lol::egl
{

class app_data : public app::data
{
public:
    app_data(char const *title, ivec2 res, float fps);
    virtual ~app_data();

    void show_pointer(bool show);
    void tick();
};

} // namespace lol
