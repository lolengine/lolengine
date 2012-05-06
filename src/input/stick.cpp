//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cmath>

#include "core.h"

namespace lol
{

/*
 * Stick implementation class
 */

static class StickData
{
    friend class Stick;

public:
    StickData() { }

private:
    Array<float> m_axes;
    Array<int> m_buttons;
}
stickdata;

/*
 * Public Stick class
 */

Stick::Stick()
  : m_data(new StickData())
{
}

Stick::~Stick()
{
    delete m_data;
}

void Stick::SetAxisCount(int n)
{
    m_data->m_axes.Empty();
    for (int i = 0; i < n; i++)
        m_data->m_axes.Push(0.f);
}

void Stick::SetButtonCount(int n)
{
    m_data->m_buttons.Empty();
    for (int i = 0; i < n; i++)
        m_data->m_buttons.Push(0);
}

void Stick::SetAxis(int n, float val)
{
    m_data->m_axes[n] = val;
}

void Stick::SetButton(int n, int val)
{
    m_data->m_buttons[n] = val;
}

int Stick::GetAxisCount()
{
    return m_data->m_axes.Count();
}

int Stick::GetButtonCount()
{
    return m_data->m_buttons.Count();
}

float Stick::GetAxis(int n)
{
    return m_data->m_axes[n];
}

int Stick::GetButton(int n)
{
    return m_data->m_buttons[n];
}

} /* namespace lol */

