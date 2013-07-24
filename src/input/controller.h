//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_INPUT_CONTROLLER_H__
#define __LOL_INPUT_CONTROLLER_H__

#if defined LOL_INPUT_V2

#include "core.h"

namespace lol
{

class KeyBinding
{
public:
    KeyBinding() : m_current(false), m_previous(false), m_device(nullptr) {}

    bool IsDown() const { return m_current; }
    bool IsUp() const { return !m_current; }
    bool IsPressed() const { return m_current && !m_previous; }
    bool IsReleased() const { return !m_current && m_previous; }

    void Bind(const char* device_name, const char* key_name);

protected:
    void Update() { m_previous = m_current; m_current = m_device ? m_device->GetKey(m_keyindex) : false; }

    const InputDevice* m_device;
    int m_keyindex;
    bool m_current;
    bool m_previous;

    friend class Controller;
};

class AxisBinding
{
public:
    AxisBinding() : m_current(0.0f), m_previous(0.0f), m_device(nullptr) {}

    float GetValue() const { return m_current; }
    float GetDelta() const { return m_current - m_previous; }

    void Bind(const char* device_name, const char* axis_name);

protected:
    void Update() { m_previous = m_current; m_current = m_device ? m_device->GetAxis(m_axisindex) : 0.0f; }

    const InputDevice* m_device;
    int m_axisindex;
    float m_current;
    float m_previous;

    friend class Controller;
};


class Controller : Entity
{
public:
    Controller(int nb_keys, int nb_axis);
    ~Controller();

    virtual void TickGame(float seconds);

    /** Activate the controller on next frame */
    void Activate();
    /** Deactivate the controller on next frame */
    void Deactivate();
    /** Deactivate every active controller on next frame and return an array of deactivated (previously active) controllers */
    static Array<Controller*> DeactivateAll();

    KeyBinding& GetKey(int index) { ASSERT(index >= 0 && index < m_nb_keys); return m_keys[index]; }
    AxisBinding& GetAxis(int index) { ASSERT(index >= 0 && index < m_nb_axis); return m_axis[index]; }

protected:
    KeyBinding* m_keys;
    AxisBinding* m_axis;
    int m_nb_keys;
    int m_nb_axis;

private:
    static Array<Controller*> controllers;
    bool m_activate_nextframe;
    bool m_deactivate_nextframe;
    bool m_active;
};

} /* namespace lol */

#endif // LOL_INPUT_V2

#endif // __LOL_INPUT_CONTROLLER_H__

