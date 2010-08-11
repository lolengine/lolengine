//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Joystick class
// ------------------
//

#if !defined __DH_JOYSTICK_H__
#define __DH_JOYSTICK_H__

class JoystickData;

class Joystick
{
public:
    Joystick();
    ~Joystick();

    int GetState();

    static int const NORTH = (1<<0);
    static int const EAST  = (1<<1);
    static int const SOUTH = (1<<2);
    static int const WEST  = (1<<3);

private:
    JoystickData *data;
};

#endif // __DH_JOYSTICK_H__

