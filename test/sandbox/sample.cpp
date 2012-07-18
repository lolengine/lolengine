//
// Lol Engine - Sandbox program
//
// Copyright: (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

class Moo
{
    Moo() {}

    virtual int SetState(int state) { return state; }
};

using namespace std;
using namespace lol;

int main(int argc, char **argv)
{
    /* Insert any test code here */
    return EXIT_SUCCESS;
}

