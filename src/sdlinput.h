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
    SdlInput();
    virtual ~SdlInput();

protected:
    virtual Group GetGroup();
    virtual void TickGame(float deltams);

private:
    SdlInputData *data;
};

#endif // __DH_SDLINPUT_H__

