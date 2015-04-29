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

const String g_name_max("MAX");
const String g_name_mouse("Mouse");
const String g_name_keyboard("Keyboard");
static String g_name_joystick()
{
    return String("Joystick");
}
static String g_name_joystick(const uint64_t num)
{
    return String::format("Joystick%d", (int)num);
}

#   define _SC(id, str, name) const String g_name_key_##name(#name);
#   include "input/keys.h"

//Mouse default buttons/axis
const String g_name_mouse_key_left("Left");
const String g_name_mouse_key_middle("Middle");
const String g_name_mouse_key_right("Right");
const String g_name_mouse_key_in_screen("InScreen");
const String g_name_mouse_axis_x("X");
const String g_name_mouse_axis_y("Y");
const String g_name_mouse_axis_xpixel("XPixel");
const String g_name_mouse_axis_ypixel("YPixel");
const String g_name_mouse_axis_scroll("Scroll");
const String g_name_mouse_cursor("Cursor");

//Xbox default buttons/axis
const String g_name_xbox_key_dpad_up("DPadUp");
const String g_name_xbox_key_dpad_down("DPadDown");
const String g_name_xbox_key_dpad_left("DPadLeft");
const String g_name_xbox_key_dpad_right("DPadRight");
const String g_name_xbox_key_left_thumb("LeftThumb");
const String g_name_xbox_key_right_thumb("RightThumb");
const String g_name_xbox_key_left_shoulder("LeftShoulder");
const String g_name_xbox_key_right_shoulder("Rightshoulder");
const String g_name_xbox_key_a("A");
const String g_name_xbox_key_b("B");
const String g_name_xbox_key_x("X");
const String g_name_xbox_key_y("Y");
const String g_name_xbox_key_start("Start");
const String g_name_xbox_key_back("Back");
const String g_name_xbox_axis_left_x("Axis1");
const String g_name_xbox_axis_left_y("Axis2");
const String g_name_xbox_axis_right_x("Axis3");
const String g_name_xbox_axis_right_y("Axis4");
const String g_name_xbox_axis_left_trigger("Axis5");
const String g_name_xbox_axis_right_trigger("Axis6");

class InputDevice
{
public:
    /** Gets the name of this input device */
    const String& GetName() const
    {
        return m_name;
    }

    /** Gets the index of the corresponding key, needed to call GetKey */
    ptrdiff_t GetKeyIndex(String const &name) const
    {
        return GetItemIndex(name, m_keynames);
    }

    /** Gets the index of the corresponding axis, needed to call GetAxis */
    ptrdiff_t GetAxisIndex(String const &name) const
    {
        return GetItemIndex(name, m_axisnames);
    }

    /** Gets the index of the corresponding cursor, needed to call GetCursor */
    ptrdiff_t GetCursorIndex(String const &name) const
    {
        return GetItemIndex(name, m_cursornames);
    }

    /** Gets the current state of the given key, true being pressed and
      * false being released */
    bool GetKey(ptrdiff_t index) const
    {
        return m_keys[index];
    }

    /** Gets the latest contents of text input. */
    String GetText();

    /** Gets the current value of the given axis. Devices should try to
      * clamp this value between -1 and 1, though it is not guaranteed. */
    float GetAxis(ptrdiff_t index) const
    {
        return m_axis[index].m1 * m_axis[index].m2;
    }

    /** Gets the current value of the given cursor, 0,0 being the bottom-left
      * corner and 1,1 being the top-right corner */
    vec2 GetCursor(ptrdiff_t index) const
    {
        return m_cursors[index].m1;
    }

    /** Gets the coordinate of the pixel the cursor is currently over,
      * 0,0 being the bottom-left corner. */
    ivec2 GetCursorPixel(ptrdiff_t index) const
    {
        return m_cursors[index].m2;
    }

    /** Sets a per-device-axis sensitivity factor. The value returned by
      * the operating system will be multiplied by this value before being
      * returned by GetAxis */
    void SetAxisSensitivity(ptrdiff_t index, float sensitivity)
    {
        m_axis[index].m2 = sensitivity;
    }

    /** Gets the per-device-axis sensitivity factor. The value returned by
      * the operating system will be multiplied by this value before being
      * returned by GetAxis */
    float GetAxisSensitivity(ptrdiff_t index) const
    {
        return m_axis[index].m2;
    }

    /** Gets a list of the name of all available keys in this device */
    const array<String>& GetAllKeys() const
    {
        return m_keynames;
    }
    /** Gets a list of the name of all available axis in this device */
    const array<String>& GetAllAxis() const
    {
        return m_axisnames;
    }
    /** Gets a list of the name of all available cursors in this device */
    const array<String>& GetAllCursors() const
    {
        return m_cursornames;
    }

    /** Gets a list of the name of all available input devices */
    static array<String> GetAvailableDevices();

    /** Gets an input device by its name */
    static InputDevice* Get(String const &name)
    {
        return GetDevice(name);
    }

    /** Default helpers */
    static InputDevice* GetKeyboard()
    {
        return GetDevice(g_name_keyboard);
    }
    static InputDevice* GetMouse()
    {
        return GetDevice(g_name_mouse);
    }
    static int GetJoystickCount()
    {
        return joystick_count;
    }
    static InputDevice* GetJoystick(const uint64_t num)
    {
        return GetDevice(g_name_joystick(num));
    }

    /** Sets whether the mouse cursor should be captured. */
    static void CaptureMouse(bool activated)
    {
        m_capturemouse = activated;
    }

protected:
    // TODO: hide all of this in a InputDeviceData?

    String m_name;

    array<String> m_keynames;
    array<String> m_axisnames;
    array<String> m_cursornames;

    /** Key states (pressed/released) */
    array<bool> m_keys;

    /** Text input state */
    String m_text;

    /** Axis states (value and sensitivity) */
    array<float, float> m_axis;

    /** Cursor position */
    array<vec2, ivec2> m_cursors;

    static bool m_capturemouse;

    InputDevice(String const &name) : m_name(name)
    {
        devices.push_unique(this);
    }

    ~InputDevice()
    {
        for (int i = 0; i < devices.count(); ++i)
        {
            if (devices[i] == this)
            {
                devices.remove(i);
                return;
            }
        }
    }

private:
    static array<InputDevice*> devices;
    static int joystick_count;

    template <typename... T>
    ptrdiff_t GetItemIndex(String const &name, const array<String, T...>& a) const
    {
        for (int i = 0; i < a.count(); ++i)
        {
            if (a[i] == name)
                return i;
        }
        return -1;
    }

    static InputDevice* GetDevice(String const &name)
    {
        //Count the device types. TODO: Multi mouse/keyboard
        if (name.contains(g_name_joystick())) joystick_count++;

        for (int i = 0; i < devices.count(); ++i)
        {
            if (devices[i]->m_name == name)
                return devices[i];
        }
        return nullptr;
    }
};

} /* namespace lol */

