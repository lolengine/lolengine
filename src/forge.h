//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Forge class
// ---------------
// The Forge is a static class that manages fonts.
//

#if !defined __DH_FORGE_H__
#define __DH_FORGE_H__

#include "font.h"

class Forge
{
public:
    static int Register(char const *path);
    static void Deregister(int id);
    static Font *GetFont(int id);
};

#endif // __DH_FORGE_H__

