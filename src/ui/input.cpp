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

#include <lol/engine-internal.h>

#include <string>
#include <map>
#include <memory>

#include "ui/input.h"

namespace lol
{

const std::string g_name_mouse("mouse");
const std::string g_name_keyboard("keyboard");

std::string g_name_joystick(const uint64_t num)
{
    return format("joystick%d", (int)num);
}

std::shared_ptr<input> input::get()
{
    static auto instance = new input();
    static auto shared_instance = std::shared_ptr<input>(instance);
    return shared_instance;
}

input::input()
{
    // Create default keyboard device
    m_keyboard = std::make_shared<InputDevice>(g_name_keyboard.c_str());
    /* Register all scancodes known to SDL (from the USB standard) */
#   define _SC(id, str, name) m_keyboard->AddKey(id, #name);
#   include "ui/keys.inc"

    // Create default mouse device
    m_mouse = std::make_shared<InputDevice>(g_name_mouse.c_str());
    m_mouse->AddKey(g_name_mouse_key_left.c_str());
    m_mouse->AddKey(g_name_mouse_key_middle.c_str());
    m_mouse->AddKey(g_name_mouse_key_right.c_str());
    // Added to manage if mouse is in the screen or not.
    m_mouse->AddKey(g_name_mouse_key_in_screen.c_str());
    m_mouse->AddAxis(g_name_mouse_axis_x.c_str());
    m_mouse->AddAxis(g_name_mouse_axis_y.c_str());
    m_mouse->AddAxis(g_name_mouse_axis_xpixel.c_str());
    m_mouse->AddAxis(g_name_mouse_axis_ypixel.c_str());
    m_mouse->AddAxis(g_name_mouse_axis_scroll.c_str(), .0000001f);
    m_mouse->AddCursor(g_name_mouse_cursor.c_str());
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

array<InputDevice*> InputDevice::devices;

std::string InputDevice::text()
{
    std::string ret = m_text;
    m_text = "";
    return ret;
}

bool InputDevice::capture_text()
{
    return m_input_active;
}

void InputDevice::capture_text(bool status)
{
    m_input_active = status;
}

void InputDevice::AddKey(int index, const char* name)
{
    if (index == -1)
        index = (int)m_key_names.size();

    while (index >= (int)m_key_names.size())
    {
        m_key_names.push_back("");
        m_keys.push_back(false);
    }

    m_key_names[index] = name;
}

void InputDevice::AddAxis(int index, const char* name, float sensitivity)
{
    if (index == -1)
        index = (int)m_axis_names.size();

    while (index >= (int)m_axis_names.size())
    {
        m_axis_names.push_back("");
        m_axis.push(0.0f, 1.0f);
    }

    m_axis_names[index] = name;
    m_axis[index].m1 = 0.0f;
    m_axis[index].m2 = sensitivity;
}

void InputDevice::AddCursor(int index, const char* name)
{
    if (index == -1)
        index = (int)m_cursor_names.size();

    while (index >= (int)m_cursor_names.size())
    {
        m_cursor_names.push_back("");
        m_cursors.push(vec2::zero, ivec2::zero);
    }

    m_cursor_names[index] = name;
}

} /* namespace lol */
