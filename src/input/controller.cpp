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

void KeyBinding::Bind(const char* device_name, const char* key_name)
{
    ClearBinding();

    m_device = InputDevice::Get(device_name);
    if (!m_device)
    {
        Log::Warn("Trying to bind controller to device %s which doesn't exist", device_name);
        return;
    }

    m_keyindex = m_device->GetKeyIndex(key_name);

    if (m_keyindex < 0)
    {
        Log::Warn("Trying to bind controller to key %s.%s which doesn't exist", device_name, key_name);
    }
}

void KeyBinding::ClearBinding()
{
    m_keyindex = -1;
}

///////////////////////////////////////////////////////////////////////////////
// AxisBinding

void AxisBinding::Bind(const char* device_name, const char* axis_name)
{
    ClearBinding();

    m_device = InputDevice::Get(device_name);
    if (!m_device)
    {
        Log::Warn("Trying to bind controller to device %s which doesn't exist", device_name);
        return;
    }

    m_axisindex = m_device->GetAxisIndex(axis_name);

    if (m_axisindex < 0)
    {
        Log::Warn("Trying to bind controller to axis %s.%s which doesn't exist", device_name, axis_name);
    }
}

void AxisBinding::BindKey(const char* device_name, const char* key_name)
{
    ClearBinding();

    m_device = InputDevice::Get(device_name);
    if (!m_device)
    {
        Log::Warn("Trying to bind controller to device %s which doesn't exist", device_name);
        return;
    }

    m_maxkeyindex = m_device->GetKeyIndex(key_name);

    if (m_maxkeyindex < 0)
    {
        Log::Warn("Trying to bind controller to key %s.%s which doesn't exist", device_name, key_name);
    }
}

void AxisBinding::BindKeys(const char* device_name, const char* min_key_name, const char* max_key_name)
{
    ClearBinding();
    m_device = InputDevice::Get(device_name);
    if (!m_device)
    {
        Log::Warn("Trying to bind controller to device %s which doesn't exist", device_name);
        return;
    }

    m_minkeyindex = m_device->GetKeyIndex(min_key_name);
    m_maxkeyindex = m_device->GetKeyIndex(max_key_name);

    if (m_minkeyindex < 0)
    {
        Log::Warn("Trying to bind controller to key %s.%s which doesn't exist", device_name, min_key_name);
        ClearBinding();
    }

    if (m_maxkeyindex < 0)
    {
        Log::Warn("Trying to bind controller to key %s.%s which doesn't exist", device_name, max_key_name);
        ClearBinding();
    }
}

void AxisBinding::ClearBinding()
{
    m_axisindex = -1;
    m_minkeyindex = -1;
    m_maxkeyindex = -1;
}

float AxisBinding::RetrieveCurrentValue()
{
    if (m_axisindex != -1)
    {
        return m_device->GetAxis(m_axisindex);
    }

    float value = 0.0f;
    if (m_minkeyindex != -1)
        value += m_device->GetKey(m_minkeyindex) ? -1.0f : 0.0f;
    if (m_maxkeyindex != -1)
        value += m_device->GetKey(m_maxkeyindex) ? 1.0f : 0.0f;

    return value;
}

///////////////////////////////////////////////////////////////////////////////
// Controller

Array<Controller*> Controller::controllers;

Controller::Controller(int nb_keys, int nb_axis)
{
    m_gamegroup = GAMEGROUP_BEFORE;
    m_keys.Resize(nb_keys);
    m_axis.Resize(nb_axis);
    m_activate_nextframe = false;
    m_deactivate_nextframe = false;
    m_active = false;
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
