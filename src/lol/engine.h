﻿//
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
// The main header (for all applications)
// --------------------------------------
//

// If using Android, override main() with our version
#if __ANDROID__
#   define main lol_android_main
#endif

// If using NX, do that, too
#if __NX__
#   define main lol_nx_main
#endif

// If using SDL on Windows or OS X, let it override main()
#if LOL_USE_SDL && (_WIN32 || __APPLE__)
#   include <SDL_main.h>
#endif

namespace lol::engine
{

bool has_opengl();

}

#include <lol/public.h>
#include <lol/extras.h>

