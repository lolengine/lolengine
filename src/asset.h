//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Asset class
// ---------------
// Assets are objects that can be ticked by the game loop and/or the render
// loop. Assets are implemented as one or several linked lists. See the
// Ticker class for the ticking logic and the linked list implementation.
//

#if !defined __DH_ASSET_H__
#define __DH_ASSET_H__

#include <stdint.h>

class Asset
{
    friend class Ticker;
    friend class TickerData;

public:
    Asset();
    virtual ~Asset();

    virtual void Ref();
    virtual int Unref();

protected:
    typedef enum
    {
        GROUP_BEFORE = 0,
        GROUP_DEFAULT,
        GROUP_AFTER,
        GROUP_COUNT
    }
    Group;

    virtual Group GetGroup();

    virtual void TickGame(float delta_time);
    virtual void TickRender(float delta_time);

    Asset *next;
    int ref, destroy;
};

#endif // __DH_ASSET_H__

