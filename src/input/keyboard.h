//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Keyboard class
// ------------------
//

#if !defined __LOL_INPUT_KEYBOARD_H__
#define __LOL_INPUT_KEYBOARD_H__

#ifndef LOL_INPUT_V2

#include "entity.h"

namespace lol
{

class KeyboardData;

class Keyboard : public Entity
{
    friend class Input;

public:
    void PushChar(uint32_t ch);
    uint32_t PopChar();

private:
    Keyboard();
    ~Keyboard();
    KeyboardData *m_data;
};

} /* namespace lol */

#endif // !LOL_INPUT_V2

#endif // __LOL_INPUT_KEYBOARD_H__

