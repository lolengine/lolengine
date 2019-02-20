//
//  Lol Engine
//
//  Copyright © 2010—2015 Benjamin Litzelmann
//            © 2017—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <string>

namespace lol
{

///////////////////////////////////////////////////////////////////////////////
// KeyBinding

void KeyBinding::Bind(const std::string& device_name, const std::string& key_name)
{
    const InputDevice* device = InputDevice::Get(device_name);

    if (!device)
    {
        msg::warn("trying to bind key to nonexistent input device %s\n",
                  device_name.c_str());
        return;
    }

    int keyindex = (int)device->GetKeyIndex(key_name);
    if (keyindex < 0)
    {
        msg::warn("trying to bind nonexistent key %s.%s\n",
                  device_name.c_str(), key_name.c_str());
        return;
    }

    m_keybindings.push(device, keyindex);
}

bool KeyBinding::Unbind(const std::string& device_name, const std::string& key_name)
{
    for (int i = 0; i < m_keybindings.count(); ++i)
    {
        if (m_keybindings[i].m1->GetName() == device_name)
        {
            if (m_keybindings[i].m2 == m_keybindings[i].m1->GetKeyIndex(key_name))
            {
                m_keybindings.remove(i);
                return true;
            }
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// AxisBinding

void AxisBinding::Bind(const std::string& device_name, const std::string& axis_name)
{
    const InputDevice* device = InputDevice::Get(device_name);
    if (!device)
    {
        msg::warn("trying to bind axis to nonexistent input device %s\n",
                  device_name.c_str());
        return;
    }

    int axisindex = (int)device->GetAxisIndex(axis_name);
    if (axisindex < 0)
    {
        msg::warn("trying to bind nonexistent axis %s.%s\n",
                  device_name.c_str(), axis_name.c_str());
        return;
    }

    m_axisbindings.push(device, axisindex);
}

void AxisBinding::BindKey(const std::string& device_name, const std::string& key_name)
{
    const InputDevice* device = InputDevice::Get(device_name);
    if (!device)
    {
        msg::warn("trying to bind axis key to nonexistent input device %s\n",
                  device_name.c_str());
        return;
    }

    int keyindex = (int)device->GetKeyIndex(key_name);
    if (keyindex < 0)
    {
        msg::warn("trying to bind nonexistent axis key %s.%s\n",
                  device_name.c_str(), key_name.c_str());
        return;
    }

    m_keybindings.push(device, -1, keyindex);
}

void AxisBinding::BindKeys(const std::string& device_name, const std::string& min_key_name, const std::string& max_key_name)
{
    const InputDevice* device = InputDevice::Get(device_name);
    if (!device)
    {
        msg::warn("trying to bind axis keys to nonexistent input device %s\n",
                  device_name.c_str());
        return;
    }

    int minkeyindex = (int)device->GetKeyIndex(min_key_name);
    if (minkeyindex < 0)
    {
        msg::warn("trying to bind nonexistent axis key %s.%s\n",
                  device_name.c_str(), min_key_name.c_str());
        return;
    }

    int maxkeyindex = (int)device->GetKeyIndex(max_key_name);
    if (maxkeyindex < 0)
    {
        msg::warn("trying to bind nonexistent axis key %s.%s\n",
                  device_name.c_str(), max_key_name.c_str());
        return;
    }

    m_keybindings.push(device, minkeyindex, maxkeyindex);
}

bool AxisBinding::Unbind(const std::string& device_name, const std::string& axis_name)
{
    for (int i = 0; i < m_keybindings.count(); ++i)
    {
        if (m_axisbindings[i].m1->GetName() == device_name)
        {
            if (m_axisbindings[i].m2 == m_axisbindings[i].m1->GetAxisIndex(axis_name))
            {
                m_axisbindings.remove(i);
                return true;
            }
        }
    }
    return false;
}

bool AxisBinding::UnbindKey(const std::string& device_name, const std::string& key_name)
{
    for (int i = 0; i < m_keybindings.count(); ++i)
    {
        if (m_keybindings[i].m1->GetName() == device_name)
        {
            if (m_keybindings[i].m2 == -1 && m_keybindings[i].m3 == m_keybindings[i].m1->GetKeyIndex(key_name))
            {
                m_keybindings.remove(i);
                return true;
            }
        }
    }
    return false;
}

bool AxisBinding::UnbindKeys(const std::string& device_name, const std::string& min_key_name, const std::string& max_key_name)
{
    for (int i = 0; i < m_keybindings.count(); ++i)
    {
        if (m_keybindings[i].m1->GetName() == device_name)
        {
            if (m_keybindings[i].m2 == m_keybindings[i].m1->GetKeyIndex(min_key_name)
                && m_keybindings[i].m3 == m_keybindings[i].m1->GetKeyIndex(max_key_name))
            {
                m_keybindings.remove(i);
                return true;
            }
        }
    }
    return false;

}

float AxisBinding::RetrieveCurrentValue()
{
    float max_positive = 0.0f;
    float max_negative = 0.0f;

    for (int i = 0; i < m_axisbindings.count(); ++i)
    {
        float value = m_axisbindings[i].m1->GetAxis(m_axisbindings[i].m2);
        if (value > max_positive)
            max_positive = value;
        if (value < max_negative)
            max_negative = value;
    }

    for (int i = 0; i < m_keybindings.count(); ++i)
    {
        float value = 0.0f;
        m_keybindings[i].m1->key(m_keybindings[i].m2);
        value += m_keybindings[i].m1->key(m_keybindings[i].m3) ? 1.0f : 0.0f;
        if (m_keybindings[i].m2 != -1)
            value += m_keybindings[i].m1->key(m_keybindings[i].m2) ? -1.0f : 0.0f;

        if (value > max_positive)
            max_positive = value;
        if (value < max_negative)
            max_negative = value;
    }

    return max_negative + max_positive;
}

///////////////////////////////////////////////////////////////////////////////
// Controller

array<Controller*> Controller::controllers;
uint32_t Controller::m_active_layer = ~((uint32_t)0);

//-----------------------------------------------------------------------------
Controller::Controller(std::string const &name)
{
    m_gamegroup = GAMEGROUP_INPUT;
    m_name = name;
    m_activate_nextframe = true;
    m_deactivate_nextframe = false;
    m_active = false;
    if (Get(name) != nullptr)
    {
        msg::warn("controller “%s” has already been registered\n", name.c_str());
    }
    controllers.push(this);
}

Controller::Controller(std::string const &name, InputProfile const& profile)
    : Controller(name)
{
    Init(profile);
}

Controller::~Controller()
{
    ClearProfile();
    for (int i = 0; i < controllers.count(); ++i)
    {
        if (controllers[i] == this)
        {
            controllers.remove(i);
            break;
        }
    }
}

//Init mode 1: Input profile system -------------------------------------------
void Controller::Init(InputProfile const& profile)
{
    UnbindProfile();
    BindProfile(profile);
}
void Controller::ClearProfile()
{
    UnbindProfile();
}

//Layer mask stuff ------------------------------------------------------------
void Controller::SetLayerMask(uint32_t layer_mask)
{
    m_layer_mask = layer_mask;
}
uint32_t Controller::GetLayerMask()
{
    return m_layer_mask;
}
bool Controller::IsLayerActive()
{
    return !!(m_layer_mask & m_active_layer);
}

//GetKeys/Axis stuff ----------------------------------------------------------
KeyBinding& Controller::GetKey(int index)
{
    return m_keys[index];
}

AxisBinding& Controller::GetAxis(int index)
{
    return m_axis[index];
}

// Key methods: should not go directly to binding
bool Controller::IsKeyPressed(int index) const
{
    auto key = m_keys.find(index);
    return key != m_keys.end() && key->second.IsPressed();
}

bool Controller::IsKeyReleased(int index) const
{
    auto key = m_keys.find(index);
    return key != m_keys.end() && key->second.IsReleased();
}

bool Controller::WasKeyPressedThisFrame(int index) const
{
    auto key = m_keys.find(index);
    return key != m_keys.end() && key->second.WasPressedThisFrame();
}

bool Controller::WasKeyReleasedThisFrame(int index) const
{
    auto key = m_keys.find(index);
    return key != m_keys.end() && key->second.WasReleasedThisFrame();
}

//Axis methods: should not go directly to binding -----------------------------
float Controller::GetAxisValue(int index) const
{
    auto axis = m_axis.find(index);
    return axis != m_axis.end() ? axis->second.GetValue() : 0.f;
}

float Controller::GetAxisDelta(int index) const
{
    auto axis = m_axis.find(index);
    return axis != m_axis.end() ? axis->second.GetDelta() : 0.f;
}

//-----------------------------------------------------------------------------
Controller* Controller::Get(std::string const &name)
{
    for (auto controller : controllers)
        if (controller->m_name == name)
            return controller;
    return nullptr;
}

//Input profile system --------------------------------------------------------
void Controller::UnbindProfile()
{
    if (m_profile.IsEmpty())
        return;

    m_mutex.lock();

    //Keyboard
    if (m_keyboard)
    {
        for (InputProfile::Keyboard& key : m_profile.m_keys)
            GetKey(key.m_idx).UnbindKeyboard(key.m_name);
        m_keyboard = nullptr;
    }

    //Mouse
    if (m_mouse)
    {
        for (InputProfile::MouseKey& key : m_profile.m_mouse_keys)
            GetKey(key.m_idx).UnbindMouse(key.m_name);
        for (InputProfile::MouseAxis& axis : m_profile.m_mouse_axis)
            GetAxis(axis.m_idx).UnbindMouse(axis.m_name);
        m_mouse = nullptr;
    }

    //Joystick
    for (InputProfile::JoystickKey& key : m_profile.m_joystick_keys)
    {
        if (m_joystick_idx.find(key.m_joy) != INDEX_NONE)
            GetKey(key.m_idx).UnbindJoystick(key.m_joy, key.m_name);
    }
    for (InputProfile::JoystickAxis& axis : m_profile.m_joystick_axis)
    {
        if (m_joystick_idx.find(axis.m_joy) != INDEX_NONE)
            GetAxis(axis.m_idx).UnbindJoystick(axis.m_joy, axis.m_name);
    }
    m_joystick.clear();
    m_joystick_idx.clear();

    m_mutex.unlock();
}
//Input profile system --------------------------------------------------------
void Controller::BindProfile(InputProfile const& setup)
{
    ASSERT(!setup.IsEmpty());

    m_mutex.lock();
    m_profile = setup;

    // Keyboard
    m_keyboard = InputDevice::GetKeyboard();
    if (m_keyboard)
    {
        for (InputProfile::Keyboard& key : m_profile.m_keys)
            GetKey(key.m_idx).BindKeyboard(key.m_name);
    }

    // Mouse
    m_mouse = InputDevice::GetMouse();
    if (m_mouse)
    {
        for (InputProfile::MouseKey& key : m_profile.m_mouse_keys)
            GetKey(key.m_idx).BindMouse(key.m_name);
        for (InputProfile::MouseAxis& axis : m_profile.m_mouse_axis)
            GetAxis(axis.m_idx).BindMouse(axis.m_name);
    }

    // Joystick
    for (uint64_t joy_idx : m_profile.m_joystick)
    {
        class InputDevice* joystick = InputDevice::GetJoystick(joy_idx);
        if (joystick)
        {
            m_joystick << joystick;
            m_joystick_idx << joy_idx;
        }
    }
    for (InputProfile::JoystickKey& key : m_profile.m_joystick_keys)
    {
        if (m_joystick_idx.find(key.m_joy) != INDEX_NONE)
            GetKey(key.m_idx).BindJoystick(key.m_joy, key.m_name);
    }
    for (InputProfile::JoystickAxis& axis : m_profile.m_joystick_axis)
    {
        if (m_joystick_idx.find(axis.m_joy) != INDEX_NONE)
            GetAxis(axis.m_idx).BindJoystick(axis.m_joy, axis.m_name);
    }

    m_mutex.unlock();
}

//-----------------------------------------------------------------------------
void Controller::tick_game(float seconds)
{
    Entity::tick_game(seconds);

    if (m_active)
    {
        for (auto &kv : m_keys)
            kv.second.Update();

        for (auto &kv : m_axis)
            kv.second.Update();
    }

    if (m_activate_nextframe)
        m_active = true;

    if (m_deactivate_nextframe)
        m_active = false;

    m_activate_nextframe = false;
    m_deactivate_nextframe = false;
}

//-----------------------------------------------------------------------------
void Controller::Activate()
{
    m_activate_nextframe = true;
    m_deactivate_nextframe = false;
}

void Controller::Deactivate()
{
    m_deactivate_nextframe = true;
    m_activate_nextframe = false;
}

//-----------------------------------------------------------------------------
array<Controller*> Controller::DeactivateAll()
{
    array<Controller*> result;

    for (int i = 0; i < controllers.count(); ++i)
    {
        if (controllers[i]->m_active || controllers[i]->m_activate_nextframe)
        {
            result.push(controllers[i]);
            controllers[i]->Deactivate();
        }
    }

    return result;
}

} /* namespace lol */
