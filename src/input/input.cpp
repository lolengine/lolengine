//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

#include "input/input_internal.h"

namespace lol
{

Array<InputDevice*> InputDevice::devices;
bool InputDevice::m_capturemouse;

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
    m_cursors.Push(vec2(0.0), ivec2(0));
}

InputDeviceInternal* InputDeviceInternal::CreateStandardKeyboard()
{
    InputDeviceInternal* keyboard = new InputDeviceInternal("Keyboard");
    /* "value" is unused, what matters is the index. */
#   define KEY_FUNC(key, value) \
        keyboard->AddKey(#key);
#   include "input/keys.h"
#   undef KEY_FUNC
    return keyboard;
}

InputDeviceInternal* InputDeviceInternal::CreateStandardMouse()
{
    InputDeviceInternal* mouse = new InputDeviceInternal("Mouse");
    mouse->AddKey("ButtonLeft");
    mouse->AddKey("ButtonMiddle");
    mouse->AddKey("ButtonRight");

    mouse->AddAxis("X");
    mouse->AddAxis("Y");

    mouse->AddCursor("Cursor");

    // TODO: extended button, and wheel (as axis or as buttons? or both?)
    return mouse;
}

} /* namespace lol */
