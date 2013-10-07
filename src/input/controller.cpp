//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

namespace lol
{

///////////////////////////////////////////////////////////////////////////////
// KeyBinding

void KeyBinding::Bind(const String& device_name, const String& key_name)
{
    const InputDevice* device = InputDevice::Get(device_name.C());

    if (!device)
    {
        Log::Warn("Trying to bind controller to device %s which doesn't exist", device_name.C());
        return;
    }

    int keyindex = device->GetKeyIndex(key_name.C());
    if (keyindex < 0)
    {
        Log::Warn("Trying to bind controller to key %s.%s which doesn't exist", device_name.C(), key_name.C());
        return;
    }

    m_keybindings.Push(device, keyindex);
}

bool KeyBinding::Unbind(const String& device_name, const String& key_name)
{
    for (int i = 0; i < m_keybindings.Count(); ++i)
    {
        if (m_keybindings[i].m1->GetName() == device_name)
        {
            if (m_keybindings[i].m2 == m_keybindings[i].m1->GetKeyIndex(key_name.C()))
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
    const InputDevice* device = InputDevice::Get(device_name.C());
    if (!device)
    {
        Log::Warn("Trying to bind controller to device %s which doesn't exist", device_name.C());
        return;
    }

    int axisindex = device->GetAxisIndex(axis_name.C());
    if (axisindex < 0)
    {
        Log::Warn("Trying to bind controller to axis %s.%s which doesn't exist", device_name.C(), axis_name.C());
        return;
    }

    m_axisbindings.Push(device, axisindex);
}

void AxisBinding::BindKey(const String& device_name, const String& key_name)
{
    const InputDevice* device = InputDevice::Get(device_name.C());
    if (!device)
    {
        Log::Warn("Trying to bind controller to device %s which doesn't exist", device_name.C());
        return;
    }

    int keyindex = device->GetKeyIndex(key_name.C());
    if (keyindex < 0)
    {
        Log::Warn("Trying to bind controller to key %s.%s which doesn't exist", device_name.C(), key_name.C());
        return;
    }

    m_keybindings.Push(device, -1, keyindex);
}

void AxisBinding::BindKeys(const String& device_name, const String& min_key_name, const String& max_key_name)
{
    const InputDevice* device = InputDevice::Get(device_name.C());
    if (!device)
    {
        Log::Warn("Trying to bind controller to device %s which doesn't exist", device_name.C());
        return;
    }

    int minkeyindex = device->GetKeyIndex(min_key_name.C());
    if (minkeyindex < 0)
    {
        Log::Warn("Trying to bind controller to key %s.%s which doesn't exist", device_name, min_key_name.C());
        return;
    }

    int maxkeyindex = device->GetKeyIndex(max_key_name.C());
    if (maxkeyindex < 0)
    {
        Log::Warn("Trying to bind controller to key %s.%s which doesn't exist", device_name, max_key_name.C());
        return;
    }

    m_keybindings.Push(device, minkeyindex, maxkeyindex);
}

bool AxisBinding::Unbind(const String& device_name, const String& axis_name)
{
    for (int i = 0; i < m_keybindings.Count(); ++i)
    {
        if (m_axisbindings[i].m1->GetName() == device_name)
        {
            if (m_axisbindings[i].m2 == m_axisbindings[i].m1->GetAxisIndex(axis_name.C()))
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
            if (m_keybindings[i].m2 == -1 && m_keybindings[i].m3 == m_keybindings[i].m1->GetKeyIndex(key_name.C()))
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
            if (m_keybindings[i].m2 == m_keybindings[i].m1->GetKeyIndex(min_key_name.C())
                && m_keybindings[i].m3 == m_keybindings[i].m1->GetKeyIndex(max_key_name.C()))
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

Array<Controller*> Controller::controllers;

Controller::Controller(char const* name, int nb_keys, int nb_axis)
{
    m_gamegroup = GAMEGROUP_BEFORE;
    m_name = name;
    m_keys.Resize(nb_keys);
    m_axis.Resize(nb_axis);
    m_activate_nextframe = false;
    m_deactivate_nextframe = false;
    m_active = false;
    if (Get(name) != nullptr)
    {
        Log::Warn("a controller with this name has already been registered");
    }
    controllers.Push(this);
}

Controller::~Controller()
{
    for (int i = 0; i < controllers.Count(); ++i)
    {
        if (controllers[i] == this)
        {
            controllers.Remove(i);
            break;
        }
    }
}

Controller* Controller::Get(char const* name)
{
    for (int i = 0; i < controllers.Count(); ++i)
    {
        if (controllers[i]->m_name == name)
            return controllers[i];
    }
    return nullptr;
}

void Controller::TickGame(float seconds)
{
    Entity::TickGame(seconds);

    for (int i = 0; i < m_keys.Count(); ++i)
        m_keys[i].Update();

    for (int i = 0; i < m_axis.Count(); ++i)
        m_axis[i].Update();

    if (m_activate_nextframe)
        m_active = true;

    if (m_deactivate_nextframe)
        m_active = false;

    m_activate_nextframe = false;
    m_deactivate_nextframe = false;
}

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

Array<Controller*> Controller::DeactivateAll()
{
    Array<Controller*> result;

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
