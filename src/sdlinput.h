//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The SdlInput class
// ------------------
//

#if !defined __DH_SDLINPUT_H__
#define __DH_SDLINPUT_H__

#include "entity.h"
#include "game.h"

class SdlInputData;

class SdlInput : public Entity
{
public:
    SdlInput(Game *game);
    virtual ~SdlInput();

protected:
    virtual Group GetGroup();
    virtual void TickGame(float delta_time);

private:
    SdlInputData *data;
};

#endif // __DH_SDLINPUT_H__

