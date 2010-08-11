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

class GameData;

class Game
{
public:
    Game(char const *mapname);
    ~Game();

    void SetMouse(int x, int y);
    void Render();

private:
    GameData *data;
};

#endif // __DH_GAME_H__

