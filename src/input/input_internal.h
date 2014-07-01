//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_INPUT_DEVICE_INTERNAL_H__
#define __LOL_INPUT_DEVICE_H__

#include <lol/main.h>

namespace lol
{

/** Internal class (not public) that allows to construct an InputDevice dynamically, when the keys, axis and cursors are not known at compile time */
class InputDeviceInternal : InputDevice
{
public:
    InputDeviceInternal(const char* name) : InputDevice(name) { }

    void AddKey(const char* name);
    void AddAxis(const char* name, float sensitivity = 1.0f);
    void AddCursor(const char* name);

    void SetKey(int index, bool state) { m_keys[index] = state; }
    void SetAxis(int index, float value) { m_axis[index].m1 = value; }
    void SetCursor(int index, const vec2& position, const ivec2& pixel) { m_cursors[index].m1 = position; m_cursors[index].m2 = pixel; }
    ivec2 GetCursorPixelPos(int index) { return m_cursors[index].m2; }

    static bool GetMouseCapture() { return m_capturemouse; }

    static InputDeviceInternal* CreateStandardKeyboard();
    static InputDeviceInternal* CreateStandardMouse();
};

} /* namespace lol */

#endif // __LOL_INPUT_DEVICE_INTERNAL_H__

