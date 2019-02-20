//
//  Lol Engine
//
//  Copyright © 2017—2019 Sam Hocevar <sam@hocevar.net>
//            © 2010—2015 Benjamin Litzelmann
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <string>
#include <vector>

namespace lol
{

class input
{
public:
    enum class key : uint16_t
    {
#define _SC(id, str, name) SC_##name = id,
#include "ui/keys.inc"
    };

    static std::vector<key> const &all_keys();
    static std::string const &key_to_name(key k);
    static key name_to_key(std::string const &name);
};

const std::string g_name_mouse("Mouse");
const std::string g_name_keyboard("Keyboard");

static std::string g_name_joystick() { return "Joystick"; }
static std::string g_name_joystick(const uint64_t num)
{
    return format("Joystick%d", (int)num);
}

// Mouse default buttons/axis
const std::string g_name_mouse_key_left("Left");
const std::string g_name_mouse_key_middle("Middle");
const std::string g_name_mouse_key_right("Right");
const std::string g_name_mouse_key_in_screen("InScreen");
const std::string g_name_mouse_axis_x("X");
const std::string g_name_mouse_axis_y("Y");
const std::string g_name_mouse_axis_xpixel("XPixel");
const std::string g_name_mouse_axis_ypixel("YPixel");
const std::string g_name_mouse_axis_scroll("Scroll");
const std::string g_name_mouse_cursor("Cursor");

class InputDevice
{
public:
    /** Gets the name of this input device */
    const std::string& GetName() const
    {
        return m_name;
    }

    /** Gets the index of the corresponding key, needed to call GetKey */
    ptrdiff_t GetKeyIndex(std::string const &name) const
    {
        return GetItemIndex(name, m_key_names);
    }

    /** Gets the index of the corresponding axis, needed to call GetAxis */
    ptrdiff_t GetAxisIndex(std::string const &name) const
    {
        return GetItemIndex(name, m_axis_names);
    }

    /** Gets the index of the corresponding cursor, needed to call GetCursor */
    ptrdiff_t GetCursorIndex(std::string const &name) const
    {
        return GetItemIndex(name, m_cursor_names);
    }

    /** Get the names of all available keys on this device */
    std::vector<std::string> const& key_names() const { return m_key_names; }

    /** Get the current state of all keys */
    std::vector<bool> const &keys() const { return m_keys; }

    /** Get the current state of the given key, true being pressed and
      * false being released */
    bool key(ptrdiff_t index) const { return m_keys[index]; }

    /** Gets the latest contents of text input. */
    std::string GetText();
    bool IsTextInputActive();
    void SetTextInputActive(bool status);

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

    /** Gets a list of the name of all available axis in this device */
    const std::vector<std::string>& GetAllAxis() const
    {
        return m_axis_names;
    }
    /** Gets a list of the name of all available cursors in this device */
    const std::vector<std::string>& GetAllCursors() const
    {
        return m_cursor_names;
    }

    /** Gets a list of the name of all available input devices */
    static array<std::string> GetAvailableDevices();

    /** Gets an input device by its name */
    static InputDevice* Get(std::string const &name)
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

    std::string m_name;

    std::vector<std::string> m_key_names;
    std::vector<std::string> m_axis_names;
    std::vector<std::string> m_cursor_names;

    /** Key states (pressed/released) */
    std::vector<bool> m_keys;

    /** Text input state */
    std::string m_text;
    bool m_input_active;

    /** Axis states (value and sensitivity) */
    array<float, float> m_axis;

    /** Cursor position */
    array<vec2, ivec2> m_cursors;

    static bool m_capturemouse;

    InputDevice(std::string const &name)
      : m_name(name),
        m_input_active(false)
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
    size_t GetItemIndex(std::string const &name, std::vector<std::string, T...> const& a) const
    {
        for (size_t i = 0; i < a.size(); ++i)
            if (a[i] == name)
                return i;
        return -1;
    }

    static InputDevice* GetDevice(std::string const &name)
    {
        //Count the device types. TODO: Multi mouse/keyboard
        if (name.find(g_name_joystick()) != std::string::npos)
            ++joystick_count;

        for (int i = 0; i < devices.count(); ++i)
        {
            if (devices[i]->m_name == name)
                return devices[i];
        }
        return nullptr;
    }
};

} /* namespace lol */

