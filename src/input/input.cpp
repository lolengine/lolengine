//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "input/input_internal.h"

namespace lol
{

array<InputDevice*> InputDevice::devices;
bool InputDevice::m_capturemouse;

array<String> InputDevice::GetAvailableDevices()
{
    array<String> result;
    for (int i = 0; i < devices.Count(); ++i)
    {
        result.Push(devices[i]->m_name);
    }
    return result;
}

String InputDevice::GetText()
{
    String ret = m_text;
    m_text = "";
    return ret;
}

void InputDeviceInternal::AddKey(int index, const char* name)
{
    if (index == -1)
        index = (int)m_keynames.Count();

    while (index >= m_keynames.Count())
    {
        m_keynames.Push(name);
        m_keys.Push(false);
    }

    m_keynames.Last() = name;
}

void InputDeviceInternal::AddAxis(int index, const char* name, float sensitivity)
{
    if (index == -1)
        index = (int)m_axisnames.Count();

    while (index >= m_axisnames.Count())
    {
        m_axisnames.Push(name);
        m_axis.Push(0.0f, 1.0f);
    }

    m_axisnames.Last() = name;
    m_axis.Last().m1 = 0.0f;
    m_axis.Last().m2 = sensitivity;
}

void InputDeviceInternal::AddCursor(int index, const char* name)
{
    if (index == -1)
        index = (int)m_cursornames.Count();

    while (index >= m_cursornames.Count())
    {
        m_cursornames.Push(name);
        m_cursors.Push(vec2::zero, ivec2::zero);
    }

    m_cursornames.Last() = name;
}

InputDeviceInternal* InputDeviceInternal::CreateStandardKeyboard()
{
    InputDeviceInternal* keyboard = new InputDeviceInternal(g_name_keyboard.C());

    /* Register all scancodes known to SDL (from the USB standard) */
#   define _SC(id, str, name) keyboard->AddKey(id, #name);
#   include "input/keys.h"

    return keyboard;
}

InputDeviceInternal* InputDeviceInternal::CreateStandardMouse()
{
    InputDeviceInternal* mouse = new InputDeviceInternal(g_name_mouse.C());
    mouse->AddKey(g_name_mouse_key_left.C());
    mouse->AddKey(g_name_mouse_key_middle.C());
    mouse->AddKey(g_name_mouse_key_right.C());
    //Added to manage if mouse is in the screen or not.
    mouse->AddKey(g_name_mouse_key_inScreen.C());

    mouse->AddAxis(g_name_mouse_axis_x.C());
    mouse->AddAxis(g_name_mouse_axis_y.C());
    mouse->AddAxis(g_name_mouse_axis_xpixel.C());
    mouse->AddAxis(g_name_mouse_axis_ypixel.C());
    mouse->AddAxis(g_name_mouse_axis_scroll.C());

    mouse->AddCursor(g_name_mouse_cursor.C());

    // TODO: extended button, and wheel (as axis or as buttons? or both?)
    return mouse;
}

} /* namespace lol */
