//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

#include "core.h"

namespace lol
{

/*
 * Keyboard implementation class
 */

static class KeyboardData
{
    friend class Keyboard;

public:
    KeyboardData() { }

private:
    Array<uint32_t> m_chars;
}
keyboarddata;

/*
 * Public Keyboard class
 */

Keyboard::Keyboard()
  : m_data(new KeyboardData())
{
}

Keyboard::~Keyboard()
{
    delete m_data;
}

void Keyboard::PushChar(uint32_t ch)
{
    m_data->m_chars.Push(ch);
}

uint32_t Keyboard::PopChar()
{
    if (!m_data->m_chars.Count())
        return 0;

    uint32_t ret = m_data->m_chars[0];
    m_data->m_chars.Remove(0);
    return ret;
}

} /* namespace lol */

