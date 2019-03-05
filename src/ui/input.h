//
//  Lol Engine
//
//  Copyright © 2017—2019 Sam Hocevar <sam@hocevar.net>
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

// FIXME: maybe m_key_names is no longer required?

class input
{
public:
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

    class device
    {
    public:
        device(std::string const &name) : m_name(name) {}

        class mouse;
        class keyboard;
        class joystick;

        // Get the name of this input device
        const std::string& name() const { return m_name; }

#if 0
        //
        // Bindings section
        //

        void bind(input::key key, uint16_t event);
        void unbind(input::key key, uint16_t event);

        void bind(input::button button, uint16_t event);
        void unbind(input::button button, uint16_t event);

        void bind(input::axis axis, uint16_t event);
        void unbind(input::axis axis, uint16_t event);
#endif

        //
        // Key, button etc. state
        //

        // Get the current state of the given key
        bool key(input::key key) const
        {
            return (size_t)key >= m_keys.size() ? false : m_keys[(int)key];
        }

        bool key_released(input::key key) const
        {
            return m_released_keys.find(key) != m_released_keys.end();
        }

        bool key_pressed(input::key key) const
        {
            return m_pressed_keys.find(key) != m_pressed_keys.end();
        }

        // Get the current state of the given button
        bool button(input::button button) const
        {
            return (size_t)button >= m_buttons.size() ? false : m_buttons[(int)button];
        }

        bool button_released(input::button button) const
        {
            return m_released_buttons.find(button) != m_released_buttons.end();
        }

        bool button_pressed(input::button button) const
        {
            return m_pressed_buttons.find(button) != m_pressed_buttons.end();
        }

        // Get the current state of the given axis
        float axis(input::axis axis) const
        {
            return (size_t)axis >= m_axes.size() ? 0.0f : m_axes[(int)axis];
        }

        bool axis_changed(input::axis axis) const
        {
            return m_changed_axes.find(axis) != m_changed_axes.end();
        }

        // TODO: axis sensitivity was removed

    public:
        /** Internal functions that allow to construct an input device
          * dynamically, when the keys, axis and cursors are not known at
          * compile time. */
        void internal_add_button(input::button, char const *name);
        void internal_add_axis(input::axis, char const *name);
        void internal_add_key(input::key, char const *name);

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

    protected:
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
    };

    // Default devices

    static std::shared_ptr<class device::keyboard> keyboard() { return get()->m_keyboard; }
    static std::shared_ptr<class device::mouse> mouse() { return get()->m_mouse; }
    static std::shared_ptr<class device::joystick> joystick(int n);

    static std::vector<key> const &all_keys();
    static std::string const &key_to_name(key k);
    static key name_to_key(std::string const &name);

private:
    input();

    static std::shared_ptr<input> get();

    std::shared_ptr<device::keyboard> m_keyboard;
    std::shared_ptr<device::mouse> m_mouse;
    std::map<int, std::shared_ptr<device::joystick>> m_joysticks;
};

//
// The mouse class
//

class input::device::mouse : public input::device
{
public:
    mouse(std::string const &name) : input::device(name) {}

    // Gets and sets whether the mouse cursor should be captured.
    void capture(bool value) { m_capture = value; }
    bool capture() const { return m_capture; }

private:
    // Capture mouse pointer
    bool m_capture = false;
};

//
// The keyboard class
//

class input::device::keyboard : public input::device
{
public:
    keyboard(std::string const &name) : input::device(name) {}

    // Get the names of all available keys on this device
    std::vector<std::string> const& key_names() const { return m_key_names; }

    // Get the current state of all keys
    std::vector<bool> const &keys() const { return m_keys; }

    // Gets the latest contents of text input.
    std::string text();

    bool capture_text();
    void capture_text(bool status);

    void internal_add_text(std::string const &text)
    {
        m_text += text;
    }

private:
    // Text input state
    std::string m_text;
    bool m_input_active = false;
};

//
// The joystick class
//

class input::device::joystick : public input::device
{
public:
    joystick(std::string const &name) : input::device(name) {}
};

} /* namespace lol */

