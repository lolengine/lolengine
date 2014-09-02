//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The main header
// ---------------
//

#if !defined __LOL_MAIN_H__
#define __LOL_MAIN_H__

/* If using NaCl or Android, override main() with our version */
#if defined __native_client__
#   define main lol_nacl_main
#elif defined __ANDROID__
#   define main lol_android_main
#endif

/* If using SDL on Windows or OS X, let it override main() */
#if defined USE_SDL && (defined _WIN32 || defined __APPLE__)
#   include <SDL_main.h>
#endif

#include <lol/public.h>
#include <lol/extras.h>

#endif // __LOL_MAIN_H__

