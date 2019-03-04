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
#include <unordered_set>
#include <memory>

namespace lol
{

extern const std::string g_name_mouse;
extern const std::string g_name_keyboard;
extern std::string g_name_joystick(const uint64_t num);

class input
{
public:
    static std::shared_ptr<input> get();

    // Default devices

    static std::shared_ptr<class InputDevice> keyboard() { return get()->m_keyboard; }
    static std::shared_ptr<class InputDevice> mouse() { return get()->m_mouse; }

    // Keyboard API

    enum class key : uint16_t
    {
        #define _SC(id, str, name) SC_##name = id,
        #include "ui/keys.inc"
    };

    enum class button : uint16_t
    {
        #define _BTN(id, name) BTN_##name = id,
        #include "ui/buttons.inc"
    };

    enum class axis : uint16_t
    {
        // Mouse
        Wheel,
        X,
        Y,
        ScreenX,
        ScreenY,
        MoveX,
        MoveY,
        ScreenMoveX,
        ScreenMoveY,
        // Joystick
        LeftX,
        LeftY,
        RightX,
        RightY,
        LeftShoulder,
        RightShoulder,
    };

    static std::vector<key> const &all_keys();
    static std::string const &key_to_name(key k);
    static key name_to_key(std::string const &name);

    // Joystick API

private:
    input();

    std::shared_ptr<class InputDevice> m_keyboard;
    std::shared_ptr<class InputDevice> m_mouse;
};

class InputDevice
{
public:
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

    /** Gets the name of this input device */
    const std::string& GetName() const
    {
        return m_name;
    }

    /** Gets the index of the corresponding key, needed to call GetKey */
    int GetKeyIndex(std::string const &name) const
    {
        return GetItemIndex(name, m_key_names);
    }

    /** Gets the index of the corresponding axis, needed to call GetAxis */
    int GetAxisIndex(std::string const &name) const
    {
        return GetItemIndex(name, m_axis_names);
    }

    //
    // Bindings section
    //

    void bind(input::key key, uint16_t event);
    void unbind(input::key key, uint16_t event);

    void bind(input::button button, uint16_t event);
    void unbind(input::button button, uint16_t event);

    void bind(input::axis axis, uint16_t event);
    void unbind(input::axis axis, uint16_t event);

    //
    // Key, button etc. state
    //

    // Get the current state of the given key
    bool key(input::key key) const { return m_keys[(int)key]; }

    bool key_released(input::key key) const
    {
        return m_released_keys.find(key) != m_released_keys.end();
    }

    bool key_pressed(input::key key) const
    {
        return m_pressed_keys.find(key) != m_pressed_keys.end();
    }

    // Get the current state of the given button
    bool button(input::button button) const { return m_buttons[(int)button]; }

    bool button_released(input::button button) const
    {
        return m_released_buttons.find(button) != m_released_buttons.end();
    }

    bool button_pressed(input::button button) const
    {
        return m_pressed_buttons.find(button) != m_pressed_buttons.end();
    }

    // Get the current state of the given axis
    float axis(input::axis axis) const { return m_axes[(int)axis]; }

    bool axis_changed(input::axis axis) const
    {
        return m_changed_axes.find(axis) != m_changed_axes.end();
    }

    // TODO: axis sensitivity was removed

    //
    // Keyboard-specific section
    //

    /** Get the names of all available keys on this device */
    std::vector<std::string> const& key_names() const { return m_key_names; }

    /** Get the current state of all keys */
    std::vector<bool> const &keys() const { return m_keys; }

    /** Gets the latest contents of text input. */
    std::string text();

    bool capture_text();
    void capture_text(bool status);

    //
    // Mouse-specific section
    //

    // Gets and sets whether the mouse cursor should be captured.
    void capture(bool value) { m_capture = value; }
    bool capture() const { return m_capture; }

    /** Gets an input device by its name */
    static InputDevice* Get(std::string const &name)
    {
        for (int i = 0; i < devices.count(); ++i)
        {
            if (devices[i]->m_name == name)
                return devices[i];
        }
        return nullptr;
    }

public:
    /** Internal functions that allow to construct an InputDevice
      * dynamically, when the keys, axis and cursors are not known at
      * compile time. */
    void internal_add_key(input::key, char const *name);
    void internal_add_button(input::button, char const *name);
    void internal_add_axis(input::axis, char const *name);

    /* Internal functions for the platform-specific drivers. */
    void internal_begin_frame()
    {
        m_pressed_keys.clear();
        m_released_keys.clear();
        m_pressed_buttons.clear();
        m_released_buttons.clear();
        m_changed_axes.clear();
    }

    void internal_set_key(input::key key, bool state)
    {
        if (m_keys[(int)key] != state)
            (state ? m_pressed_keys : m_released_keys).insert(key);
        m_keys[(int)key] = state;
    }

    void internal_set_button(input::button button, bool state)
    {
        if (m_buttons[(int)button] != state)
            (state ? m_pressed_buttons : m_released_buttons).insert(button);
        m_buttons[(int)button] = state;
    }

    void internal_set_axis(input::axis axis, float value)
    {
        if (m_axes[(int)axis] != value)
            m_changed_axes.insert(axis);
        m_axes[(int)axis] = value;
    }

    void internal_add_text(std::string const &text)
    {
        m_text += text;
    }

protected:
    // TODO: hide all of this in a InputDeviceData?

    std::string m_name;

    std::vector<std::string> m_key_names;
    std::vector<std::string> m_button_names;
    std::vector<std::string> m_axis_names;

    /** Key and button states (pressed/released) */
    std::vector<bool> m_keys;
    std::unordered_set<input::key> m_pressed_keys, m_released_keys;
    std::vector<bool> m_buttons;
    std::unordered_set<input::button> m_pressed_buttons, m_released_buttons;
    std::vector<float> m_axes;
    std::unordered_set<input::axis> m_changed_axes;

    // Text input state
    std::string m_text;
    bool m_input_active;

    // Capture (for mouse devices)
    bool m_capture = false;

private:
    static array<InputDevice*> devices;

    template <typename... T>
    int GetItemIndex(std::string const &name, std::vector<std::string, T...> const& a) const
    {
        for (size_t i = 0; i < a.size(); ++i)
            if (a[i] == name)
                return (int)i;
        return -1;
    }
};

} /* namespace lol */

