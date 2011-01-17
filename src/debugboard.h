//
// Deus Hax (working title)
// Copyright (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//

//
// The DebugBoard class
// --------------------
//

#if !defined __DH_DEBUGBOARD_H__
#define __DH_DEBUGBOARD_H__

#include "entity.h"
#include "game.h"

class DebugBoardData;

class DebugBoard : public Entity
{
public:
    DebugBoard(Game *game);
    virtual ~DebugBoard();

protected:
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

private:
    DebugBoardData *data;
};

#endif // __DH_DEBUGBOARD_H__

