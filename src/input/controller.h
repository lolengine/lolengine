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

#include "core.h"

namespace lol
{

class KeyBinding
{
public:
    KeyBinding() : m_device(nullptr), m_current(false), m_previous(false), m_keyindex(-1) {}

    /** Indicate wheither the key is currently down */
    bool IsDown() const { return m_current; }
    /** Indicate wheither the key is currently up */
    bool IsUp() const { return !m_current; }
    /** Indicate wheither the key has just been pressed */
    bool IsPressed() const { return m_current && !m_previous; }
    /** Indicate wheither the key has just been released */
    bool IsReleased() const { return !m_current && m_previous; }

    /** Bind a physical device and key */
    void Bind(const char* device_name, const char* key_name);
    /** Clear current binding */
    void ClearBinding();

    /** Indicate wheither a physical device and key has been bound */
    bool IsBound() { return m_device && m_keyindex != -1; }

protected:
    void Update() { m_previous = m_current; m_current = IsBound() ? m_device->GetKey(m_keyindex) : false; }

    const InputDevice* m_device;
    int m_keyindex;
    bool m_current;
    bool m_previous;

    friend class Controller;
};

class AxisBinding
{
public:
    AxisBinding() : m_device(nullptr), m_current(0.0f), m_previous(0.0f), m_axisindex(-1), m_minkeyindex(-1), m_maxkeyindex(-1) {}

    /** Gets the current absolute value of this axis */
    float GetValue() const { return m_current; }
    /** Gets the current delta value of this axis */
    float GetDelta() const { return m_current - m_previous; }

    /** Bind a physical device and axis */
    void Bind(const char* device_name, const char* axis_name);
    /** Bind a physical device and key over this axis. The axis value will be 0 if the key is up and 1 if it's down */
    void BindKey(const char* device_name, const char* key_name);
    /** Bind physical device and keys over this axis. The axis value will be 0 if both the key are up, -1 if minkey is down, and 1 if maxkey is down */
    void BindKeys(const char* device_name, const char* min_key_name, const char* max_key_name);
    /** Clear current binding */
    void ClearBinding();

    /** Indicate wheither a physical device and axis has been bound */
    bool IsBound() { return m_device && m_axisindex != -1 || m_maxkeyindex != -1; }

protected:
    void Update() { m_previous = m_current; m_current = IsBound() ? RetrieveCurrentValue() : 0.0f; }
    float RetrieveCurrentValue();

    const InputDevice* m_device;
    int m_axisindex;
    int m_minkeyindex;
    int m_maxkeyindex;
    float m_current;
    float m_previous;

    friend class Controller;
};


class Controller : public Entity
{
public:
    Controller(char const* name, int nb_keys, int nb_axis);
    ~Controller();

    virtual void TickGame(float seconds);

    /** Activate the controller on next frame */
    void Activate();
    /** Deactivate the controller on next frame */
    void Deactivate();
    /** Deactivate every active controller on next frame and return an array of deactivated (previously active) controllers */
    static Array<Controller*> DeactivateAll();

    KeyBinding& GetKey(int index) { return m_keys[index]; }
    AxisBinding& GetAxis(int index) { return m_axis[index]; }

    static Controller* Get(char const* name);

protected:
    Array<KeyBinding> m_keys;
    Array<AxisBinding> m_axis;

private:
    static Array<Controller*> controllers;
    String m_name;
    bool m_activate_nextframe;
    bool m_deactivate_nextframe;
    bool m_active;
};

} /* namespace lol */

#endif // __LOL_INPUT_CONTROLLER_H__

