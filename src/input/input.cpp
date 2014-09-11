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

void InputDeviceInternal::AddKey(const char* name)
{
    m_keynames.Push(name);
    m_keys.Push(false);
}

void InputDeviceInternal::AddAxis(const char* name, float sensitivity)
{
    m_axisnames.Push(name);
    m_axis.Push(0.0f, sensitivity);
}

void InputDeviceInternal::AddCursor(const char* name)
{
    m_cursornames.Push(name);
    m_cursors.Push(vec2::zero, ivec2::zero);
}

InputDeviceInternal* InputDeviceInternal::CreateStandardKeyboard()
{
    InputDeviceInternal* keyboard = new InputDeviceInternal(g_name_keyboard.C());

#if USE_OLD_SDL
    /* TODO: deprecate this */
#   define KEY_FUNC(key, value) \
        keyboard->AddKey(#key);
#   include "input/keys.h"
#   undef KEY_FUNC
#else
    /* "value" is unused, what matters is the index. */
#   define _SC(value, str, name) \
        keyboard->AddKey(#name);
#   include "input/scancodes.h"
#endif

    return keyboard;
}

InputDeviceInternal* InputDeviceInternal::CreateStandardMouse()
{
    InputDeviceInternal* mouse = new InputDeviceInternal(g_name_mouse.C());
    mouse->AddKey("Left");
    mouse->AddKey("Middle");
    mouse->AddKey("Right");
    //Added to manage if mouse is in the screen or not.
    mouse->AddKey("InScreen");

    mouse->AddAxis("X");
    mouse->AddAxis("Y");
    mouse->AddAxis("XPixel");
    mouse->AddAxis("YPixel");
    mouse->AddAxis("Scroll");

    mouse->AddCursor("Cursor");

    // TODO: extended button, and wheel (as axis or as buttons? or both?)
    return mouse;
}

} /* namespace lol */
