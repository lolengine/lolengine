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

#include "input/input_internal.h"

namespace lol
{

// Lookup tables for scancode and key name lookups
static std::vector<input::key> g_all_keys
{
#define _SC(code, str, name) input::key::SC_##name,
#include "input/keys.inc"
};

static std::map<input::key, std::string> g_key_to_name
{
#define _SC(code, str, name) { input::key::SC_##name, #name },
#include "input/keys.inc"
};

static std::map<std::string, input::key> g_name_to_key
{
#define _SC(code, str, name) { #name, input::key::SC_##name },
#include "input/keys.inc"
};

std::vector<input::key> const &input::all_keys()
{
    return g_all_keys;
}

std::string const &input::key_to_name(input::key k)
{
    auto it = g_key_to_name.find(k);
    return it == g_key_to_name.end() ? "Unknown" : it->second;
}

input::key input::name_to_key(std::string const &name)
{
    auto it = g_name_to_key.find(name);
    return it == g_name_to_key.end() ? key::SC_Unknown : it->second;
}

array<InputDevice*> InputDevice::devices;
int InputDevice::joystick_count = 0;
bool InputDevice::m_capturemouse;

array<std::string> InputDevice::GetAvailableDevices()
{
    array<std::string> result;
    for (auto const &device : devices)
        result.push(device->m_name);
    return result;
}

std::string InputDevice::GetText()
{
    std::string ret = m_text;
    m_text = "";
    return ret;
}

bool InputDevice::IsTextInputActive()
{
    return m_input_active;
}

void InputDevice::SetTextInputActive(bool status)
{
    m_input_active = status;
}

void InputDeviceInternal::AddKey(int index, const char* name)
{
    if (index == -1)
        index = (int)m_key_names.size();

    while (index >= (int)m_key_names.size())
    {
        m_key_names.push_back(name);
        m_keys.push_back(false);
    }

    m_key_names[index] = name;
}

void InputDeviceInternal::AddAxis(int index, const char* name, float sensitivity)
{
    if (index == -1)
        index = (int)m_axis_names.size();

    while (index >= (int)m_axis_names.size())
    {
        m_axis_names.push_back(name);
        m_axis.push(0.0f, 1.0f);
    }

    m_axis_names[index] = name;
    m_axis[index].m1 = 0.0f;
    m_axis[index].m2 = sensitivity;
}

void InputDeviceInternal::AddCursor(int index, const char* name)
{
    if (index == -1)
        index = (int)m_cursor_names.size();

    while (index >= (int)m_cursor_names.size())
    {
        m_cursor_names.push_back(name);
        m_cursors.push(vec2::zero, ivec2::zero);
    }

    m_cursor_names[index] = name;
}

InputDeviceInternal* InputDeviceInternal::CreateStandardKeyboard()
{
    InputDeviceInternal* keyboard = new InputDeviceInternal(g_name_keyboard.c_str());

    /* Register all scancodes known to SDL (from the USB standard) */
#   define _SC(id, str, name) keyboard->AddKey(id, #name);
#   include "input/keys.inc"

    return keyboard;
}

InputDeviceInternal* InputDeviceInternal::CreateStandardMouse()
{
    InputDeviceInternal* mouse = new InputDeviceInternal(g_name_mouse.c_str());
    mouse->AddKey(g_name_mouse_key_left.c_str());
    mouse->AddKey(g_name_mouse_key_middle.c_str());
    mouse->AddKey(g_name_mouse_key_right.c_str());
    //Added to manage if mouse is in the screen or not.
    mouse->AddKey(g_name_mouse_key_in_screen.c_str());

    mouse->AddAxis(g_name_mouse_axis_x.c_str());
    mouse->AddAxis(g_name_mouse_axis_y.c_str());
    mouse->AddAxis(g_name_mouse_axis_xpixel.c_str());
    mouse->AddAxis(g_name_mouse_axis_ypixel.c_str());
    mouse->AddAxis(g_name_mouse_axis_scroll.c_str(), .0000001f);

    mouse->AddCursor(g_name_mouse_cursor.c_str());

    // TODO: extended button, and wheel (as axis or as buttons? or both?)
    return mouse;
}

} /* namespace lol */
