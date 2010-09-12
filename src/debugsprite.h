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

#include "entity.h"
#include "game.h"

class DebugSpriteData;

class DebugSprite : public Entity
{
public:
    DebugSprite(Game *game);
    virtual ~DebugSprite();

protected:
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

private:
    DebugSpriteData *data;
};

#endif // __DH_DEBUGSPRITE_H__

