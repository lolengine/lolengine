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

#include <lol/engine-internal.h>

#include <string>
#include <map>
#include <memory>

#include "ui/input.h"

namespace lol
{

std::shared_ptr<input> input::get()
{
    static auto instance = new input();
    static auto shared_instance = std::shared_ptr<input>(instance);
    return shared_instance;
}

input::input()
{
    // Create default keyboard device
    m_keyboard = std::make_shared<input::device::keyboard>("default keyboard");
    /* Register all scancodes known to SDL (from the USB standard) */
    #define _SC(id, str, name) m_keyboard->internal_add_key(input::key::SC_##name, #name);
    #include "ui/keys.inc"

    // Create default mouse device
    m_mouse = std::make_shared<input::device::mouse>("default mouse");
    #define _BTN(id, name) m_mouse->internal_add_button(input::button::BTN_##name, #name);
    #include "ui/buttons.inc" // FIXME: this will also add joystick buttons!
    m_mouse->internal_add_axis(input::axis::X, "X");
    m_mouse->internal_add_axis(input::axis::Y, "Y");
    m_mouse->internal_add_axis(input::axis::ScreenX, "ScreenX");
    m_mouse->internal_add_axis(input::axis::ScreenY, "ScreenY");
    m_mouse->internal_add_axis(input::axis::MoveX, "MoveX");
    m_mouse->internal_add_axis(input::axis::MoveY, "MoveY");
    m_mouse->internal_add_axis(input::axis::ScreenMoveX, "ScreenMoveX");
    m_mouse->internal_add_axis(input::axis::ScreenMoveY, "ScreenMoveY");
    m_mouse->internal_add_axis(input::axis::Wheel, "Wheel");

    // FIXME: there are no default joysticks and they will all have zero axes by default
}

// Lookup tables for scancode and key name lookups
static std::vector<input::key> g_all_keys
{
#define _SC(code, str, name) input::key::SC_##name,
#include "ui/keys.inc"
};

static std::map<input::key, std::string> g_key_to_name
{
#define _SC(code, str, name) { input::key::SC_##name, #name },
#include "ui/keys.inc"
};

static std::map<std::string, input::key> g_name_to_key
{
#define _SC(code, str, name) { #name, input::key::SC_##name },
#include "ui/keys.inc"
};

std::vector<input::key> const &input::all_keys()
{
    return g_all_keys;
}

std::string const &input::key_to_name(input::key k)
{
    static std::string unknown("Unknown");
    auto it = g_key_to_name.find(k);
    return it == g_key_to_name.end() ? unknown : it->second;
}

input::key input::name_to_key(std::string const &name)
{
    auto it = g_name_to_key.find(name);
    return it == g_name_to_key.end() ? key::SC_Unknown : it->second;
}

std::shared_ptr<input::device::joystick> input::joystick(int n)
{
    auto &joysticks = get()->m_joysticks;

    if (joysticks.find(n) == joysticks.end())
        joysticks[n] = std::make_shared<input::device::joystick>(format("joystick %d", n));

    return joysticks[n];
}

//
// input::device
//

void input::device::internal_add_key(input::key key, const char* name)
{
    while ((int)key >= (int)m_key_names.size())
    {
        m_key_names.push_back("");
        m_keys.push_back(false);
    }

    m_key_names[(int)key] = name;
}

void input::device::internal_add_button(input::button button, const char* name)
{
    while ((int)button >= (int)m_button_names.size())
    {
        m_button_names.push_back("");
        m_buttons.push_back(false);
    }

    m_button_names[(int)button] = name;
}

void input::device::internal_add_axis(input::axis axis, const char* name)
{
    while ((int)axis >= (int)m_axis_names.size())
    {
        m_axis_names.push_back("");
        m_axes.push_back(0.0f);
    }

    m_axis_names[(int)axis] = name;
}

//
// input::device::keyboard
//

std::string input::device::keyboard::text()
{
    std::string ret = m_text;
    m_text = "";
    return ret;
}

bool input::device::keyboard::capture_text()
{
    return m_input_active;
}

void input::device::keyboard::capture_text(bool status)
{
    m_input_active = status;
}

} /* namespace lol */
