//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The main header (for internal engine use only)
// ----------------------------------------------
//

/* Include this as early as possible */
#if HAVE_CONFIG_H
#   include "config.h"
#endif

/* If using NaCl or Android, override main() with our version */
#if __native_client__
#   define main lol_nacl_main
#elif __ANDROID__
#   define main lol_android_main
#endif

/* If using SDL on Windows or OS X, let it override main() */
#if (USE_SDL || USE_OLD_SDL) && (_WIN32 || __APPLE__)
#   include <SDL_main.h>
#endif

#include <lol/public.h>
#include <lol/extras.h>

