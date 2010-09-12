//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The MapViewer class
// -------------------
//

#if !defined __DH_MAPVIEWER_H__
#define __DH_MAPVIEWER_H__

#include "entity.h"
#include "scene.h"

class MapViewerData;

class MapViewer : public Entity
{
public:
    MapViewer(char const *mapname);
    ~MapViewer();

protected:
    /* Inherited from Entity */
    virtual void TickGame(float deltams);
    virtual void TickDraw(float deltams);

public:
    /* New methods */
    Scene *GetScene();
    int GetWidth();
    int GetHeight();
    void SetPOV(int x, int y);

private:
    MapViewerData *data;
};

#endif // __DH_MAPVIEWER_H__

