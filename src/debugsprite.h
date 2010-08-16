//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The DebugSprite class
// ---------------------
//

#if !defined __DH_DEBUGSPRITE_H__
#define __DH_DEBUGSPRITE_H__

#include "asset.h"
#include "game.h"

class DebugSpriteData;

class DebugSprite : public Asset
{
public:
    DebugSprite(Game *game);
    virtual ~DebugSprite();

protected:
    virtual Group GetGroup();
    virtual void TickGame(float delta_time);
    virtual void TickRender(float delta_time);

private:
    DebugSpriteData *data;
};

#endif // __DH_DEBUGSPRITE_H__

