//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Game class
// --------------
//

#if !defined __DH_GAME_H__
#define __DH_GAME_H__

#include "entity.h"
#include "scene.h"

class GameData;

class Game : public Entity
{
public:
    Game(char const *mapname);
    ~Game();

protected:
    /* Inherited from Entity */
    virtual Group GetGroup();
    virtual void TickGame(float deltams);
    virtual void TickRender(float deltams);

public:
    /* New methods */
    Scene *GetScene();
    void SetMouse(int x, int y);
    void Quit();
    int Finished();

private:
    GameData *data;
};

#endif // __DH_GAME_H__

