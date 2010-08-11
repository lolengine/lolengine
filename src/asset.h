
/*
 * The asset object
 */

#if !defined __DH_ASSET_H__
#define __DH_ASSET_H__

#include <stdint.h>

class Asset
{
    friend class Ticker;

public:
    Asset();
    virtual ~Asset();

    virtual void TickGame(float delta_time);
    virtual void TickRender(float delta_time);

    virtual void Ref();
    virtual int Unref();

protected:
    int index, ref, destroy;
};

#endif // __DH_ASSET_H__

