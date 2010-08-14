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
    static Font *GetFont(char const *path);
    static void ReleaseFont(Font *font);
};

#endif // __DH_FORGE_H__

