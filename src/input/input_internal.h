//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

namespace lol
{

/** Internal class (not public) that allows to construct an InputDevice
  * dynamically, when the keys, axis and cursors are not known at
  * compile time. */
class InputDeviceInternal : InputDevice
{
public:
    inline InputDeviceInternal(char const * name) : InputDevice(name) { }

    void AddKey(int id, char const * name);

    inline void AddKey(char const * name)
    {
        AddKey(-1, name);
    }

    void AddAxis(int id, char const * name, float sensitivity = 1.0f);

    inline void AddAxis(char const * name, float sensitivity = 1.0f)
    {
        AddAxis(-1, name, sensitivity);
    }

    void AddCursor(int id, char const * name);

    inline void AddCursor(char const * name)
    {
        AddCursor(-1, name);
    }

    void SetKey(int id, bool state)
    {
        m_keys[id] = state;
    }

    void AddText(String const &text)
    {
        m_text += text;
    }

    void SetAxis(int id, float value)
    {
        m_axis[id].m1 = value;
    }

    void SetCursor(int id, vec2 const & position, ivec2 const & pixel)
    {
        m_cursors[id].m1 = position;
        m_cursors[id].m2 = pixel;
    }

    ivec2 GetCursorPixelPos(int id)
    {
        return m_cursors[id].m2;
    }

    static bool GetMouseCapture()
    {
        return m_capturemouse;
    }

    static InputDeviceInternal* CreateStandardKeyboard();
    static InputDeviceInternal* CreateStandardMouse();
};

} /* namespace lol */

