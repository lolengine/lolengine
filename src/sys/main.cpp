//
//  Lol Engine
//
//  Copyright © 2010–2025 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if LOL_USE_KORE && !KINC_NO_MAIN
#   if _WIN32
int lol_kore_kickstart(int, char**);

extern "C" int kickstart(int argc, char** argv)
{
    return lol_kore_kickstart(argc, argv);
}

#   else
// One of these wrappers will be overridden by the user’s version, the
// others will just be NOPs.
int lol_kore_kickstart(void) __attribute__((weak));
int lol_kore_kickstart(int argc, char **argv) __attribute__((weak));
int lol_kore_kickstart(int argc, char **argv, char **envp) __attribute__((weak));

int lol_kore_kickstart(void) { return 0; }
int lol_kore_kickstart(int, char **) { return 0; }
int lol_kore_kickstart(int, char **, char **) { return 0; }

extern "C" int kickstart(int argc, char **argv)
{
    char *env[] = { nullptr };
    return lol_kore_kickstart()
         | lol_kore_kickstart(argc, argv)
         | lol_kore_kickstart(argc, argv, env);
}

#   endif
#endif
