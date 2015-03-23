//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

namespace lol
{

///////////////////////////////////////////////////////////////////////////////
// KeyBinding

void KeyBinding::Bind(const String& device_name, const String& key_name)
{
    const InputDevice* device = InputDevice::Get(device_name);

    if (!device)
    {
        Log::Warn("trying to bind key to nonexistent input device %s\n",
                  device_name.C());
        return;
    }

    ptrdiff_t keyindex = device->GetKeyIndex(key_name);
    if (keyindex < 0)
    {
        Log::Warn("trying to bind nonexistent key %s.%s\n",
                  device_name.C(), key_name.C());
        return;
    }

    m_keybindings.Push(device, (int)keyindex);
}

bool KeyBinding::Unbind(const String& device_name, const String& key_name)
{
    for (int i = 0; i < m_keybindings.Count(); ++i)
    {
        if (m_keybindings[i].m1->GetName() == device_name)
        {
            if (m_keybindings[i].m2 == m_keybindings[i].m1->GetKeyIndex(key_name))
            {
                m_keybindings.Remove(i);
                return true;
            }
        }
    }
    return false;
}

void KeyBinding::ClearBindings()
{
    m_keybindings.Empty();
}

///////////////////////////////////////////////////////////////////////////////
// AxisBinding

void AxisBinding::Bind(const String& device_name, const String& axis_name)
{
    const InputDevice* device = InputDevice::Get(device_name);
    if (!device)
    {
        Log::Warn("trying to bind axis to nonexistent input device %s\n",
                  device_name.C());
        return;
    }

    ptrdiff_t axisindex = device->GetAxisIndex(axis_name);
    if (axisindex < 0)
    {
        Log::Warn("trying to bind nonexistent axis %s.%s\n",
                  device_name.C(), axis_name.C());
        return;
    }

    m_axisbindings.Push(device, (int)axisindex);
}

void AxisBinding::BindKey(const String& device_name, const String& key_name)
{
    const InputDevice* device = InputDevice::Get(device_name);
    if (!device)
    {
        Log::Warn("trying to bind axis key to nonexistent input device %s\n",
                  device_name.C());
        return;
    }

    ptrdiff_t keyindex = device->GetKeyIndex(key_name);
    if (keyindex < 0)
    {
        Log::Warn("trying to bind nonexistent axis key %s.%s\n",
                  device_name.C(), key_name.C());
        return;
    }

    m_keybindings.Push(device, -1, (int)keyindex);
}

void AxisBinding::BindKeys(const String& device_name, const String& min_key_name, const String& max_key_name)
{
    const InputDevice* device = InputDevice::Get(device_name);
    if (!device)
    {
        Log::Warn("trying to bind axis keys to nonexistent input device %s\n",
                  device_name.C());
        return;
    }

    ptrdiff_t minkeyindex = device->GetKeyIndex(min_key_name);
    if (minkeyindex < 0)
    {
        Log::Warn("trying to bind nonexistent axis key %s.%s\n",
                  device_name.C(), min_key_name.C());
        return;
    }

    ptrdiff_t maxkeyindex = device->GetKeyIndex(max_key_name);
    if (maxkeyindex < 0)
    {
        Log::Warn("trying to bind nonexistent axis key %s.%s\n",
                  device_name.C(), max_key_name.C());
        return;
    }

    m_keybindings.Push(device, (int)minkeyindex, (int)maxkeyindex);
}

bool AxisBinding::Unbind(const String& device_name, const String& axis_name)
{
    for (int i = 0; i < m_keybindings.Count(); ++i)
    {
        if (m_axisbindings[i].m1->GetName() == device_name)
        {
            if (m_axisbindings[i].m2 == m_axisbindings[i].m1->GetAxisIndex(axis_name))
            {
                m_axisbindings.Remove(i);
                return true;
            }
        }
    }
    return false;
}

bool AxisBinding::UnbindKey(const String& device_name, const String& key_name)
{
    for (int i = 0; i < m_keybindings.Count(); ++i)
    {
        if (m_keybindings[i].m1->GetName() == device_name)
        {
            if (m_keybindings[i].m2 == -1 && m_keybindings[i].m3 == m_keybindings[i].m1->GetKeyIndex(key_name))
            {
                m_keybindings.Remove(i);
                return true;
            }
        }
    }
    return false;
}

bool AxisBinding::UnbindKeys(const String& device_name, const String& min_key_name, const String& max_key_name)
{
    for (int i = 0; i < m_keybindings.Count(); ++i)
    {
        if (m_keybindings[i].m1->GetName() == device_name)
        {
            if (m_keybindings[i].m2 == m_keybindings[i].m1->GetKeyIndex(min_key_name)
                && m_keybindings[i].m3 == m_keybindings[i].m1->GetKeyIndex(max_key_name))
            {
                m_keybindings.Remove(i);
                return true;
            }
        }
    }
    return false;

}

void AxisBinding::ClearBindings()
{
    m_axisbindings.Empty();
    m_keybindings.Empty();
}

float AxisBinding::RetrieveCurrentValue()
{
    float max_positive = 0.0f;
    float max_negative = 0.0f;

    for (int i = 0; i < m_axisbindings.Count(); ++i)
    {
        float value = m_axisbindings[i].m1->GetAxis(m_axisbindings[i].m2);
        if (value > max_positive)
            max_positive = value;
        if (value < max_negative)
            max_negative = value;
    }

    for (int i = 0; i < m_keybindings.Count(); ++i)
    {
        float value = 0.0f;
        m_keybindings[i].m1->GetKey(m_keybindings[i].m2);
        value += m_keybindings[i].m1->GetKey(m_keybindings[i].m3) ? 1.0f : 0.0f;
        if (m_keybindings[i].m2 != -1)
            value += m_keybindings[i].m1->GetKey(m_keybindings[i].m2) ? -1.0f : 0.0f;

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
Controller::Controller(String const &name)
{
    m_gamegroup = GAMEGROUP_INPUT;
    m_name = name;
    m_activate_nextframe = true;
    m_deactivate_nextframe = false;
    m_active = false;
    if (Get(name) != nullptr)
    {
        Log::Warn("controller “%s” has already been registered\n", name.C());
    }
    controllers.Push(this);
}

Controller::Controller(String const &name, InputProfile const& profile)
    : Controller(name)
{
    Init(profile);
}

Controller::~Controller()
{
    ClearProfile();
    for (int i = 0; i < controllers.Count(); ++i)
    {
        if (controllers[i] == this)
        {
            controllers.Remove(i);
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

//Init mode 2: By hand, key/axis by key/axis ----------------------------------
void Controller::SetInputCount(int nb_keys, int nb_axis)
{
    m_keys.Resize(nb_keys);
    m_axis.Resize(nb_axis);
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
//#error do something better IsLayerActive()
    return m_keys[index];
}
AxisBinding& Controller::GetAxis(int index)
{
    return m_axis[index];
}

//-----------------------------------------------------------------------------
Controller* Controller::Get(String const &name)
{
    for (int i = 0; i < controllers.Count(); ++i)
    {
        if (controllers[i]->m_name == name)
            return controllers[i];
    }
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
        if (m_joystick_idx.Find(key.m_joy) != INDEX_NONE)
            GetKey(key.m_idx).UnbindJoystick(key.m_joy, key.m_name);
    }
    for (InputProfile::JoystickAxis& axis : m_profile.m_joystick_axis)
    {
        if (m_joystick_idx.Find(axis.m_joy) != INDEX_NONE)
            GetAxis(axis.m_idx).UnbindJoystick(axis.m_joy, axis.m_name);
    }
    m_joystick.Empty();
    m_joystick_idx.Empty();

    m_mutex.unlock();
}
//Input profile system --------------------------------------------------------
void Controller::BindProfile(InputProfile const& setup)
{
    ASSERT(!setup.IsEmpty());

    m_mutex.lock();
    m_profile = setup;

    m_keys.Resize(m_profile.GetKeyCount());
    m_axis.Resize(m_profile.GetAxisCount());

    //Keyboard
    m_keyboard = InputDevice::GetKeyboard();
    if (m_keyboard)
    {
        for (InputProfile::Keyboard& key : m_profile.m_keys)
            GetKey(key.m_idx).BindKeyboard(key.m_name);
    }

    //Mouse
    m_mouse = InputDevice::GetMouse();
    if (m_mouse)
    {
        for (InputProfile::MouseKey& key : m_profile.m_mouse_keys)
            GetKey(key.m_idx).BindMouse(key.m_name);
        for (InputProfile::MouseAxis& axis : m_profile.m_mouse_axis)
            GetAxis(axis.m_idx).BindMouse(axis.m_name);
    }

    //Joystick
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
        if (m_joystick_idx.Find(key.m_joy) != INDEX_NONE)
            GetKey(key.m_idx).BindJoystick(key.m_joy, key.m_name);
    }
    for (InputProfile::JoystickAxis& axis : m_profile.m_joystick_axis)
    {
        if (m_joystick_idx.Find(axis.m_joy) != INDEX_NONE)
            GetAxis(axis.m_idx).BindJoystick(axis.m_joy, axis.m_name);
    }

    m_mutex.unlock();
}

//-----------------------------------------------------------------------------
void Controller::TickGame(float seconds)
{
    Entity::TickGame(seconds);

    if (m_active)
    {
        for (int i = 0; i < m_keys.Count(); ++i)
            m_keys[i].Update();

        for (int i = 0; i < m_axis.Count(); ++i)
            m_axis[i].Update();
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

    for (int i = 0; i < controllers.Count(); ++i)
    {
        if (controllers[i]->m_active || controllers[i]->m_activate_nextframe)
        {
            result.Push(controllers[i]);
            controllers[i]->Deactivate();
        }
    }

    return result;
}

} /* namespace lol */
