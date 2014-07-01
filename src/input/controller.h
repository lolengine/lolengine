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

#include <lol/main.h>

namespace lol
{

class KeyBinding
{
public:
    KeyBinding()
      : m_current(false),
        m_previous(false)
    {}

    /** Indicates wheither the key is currently down */
    bool IsDown() const { return m_current; }
    /** Indicates wheither the key is currently up */
    bool IsUp() const { return !m_current; }
    /** Indicates wheither the key has just been pressed */
    bool IsPressed() const { return m_current && !m_previous; }
    /** Indicates wheither the key has just been released */
    bool IsReleased() const { return !m_current && m_previous; }

    /** Bind a physical device and key */
    void Bind(const String& device_name, const String& key_name);
    /** Unbind a previously bound physical device and key. Returns true if the binding was existing. */
    bool Unbind(const String& device_name, const String& key_name);
    /* Small helpers */
    void BindMouse(const String& key_name)        { Bind(g_name_mouse, key_name); }
    void BindKeyboard(const String& key_name)     { Bind(g_name_keyboard, key_name); }
    bool UnbindMouse(const String& key_name)      { return Unbind(g_name_mouse, key_name); }
    bool UnbindKeyboard(const String& key_name)   { return Unbind(g_name_mouse, key_name); }
    /** Clear current binding */
    void ClearBindings();
    /** Indicate wheither a physical device and key has been bound. Returns the number of bindings set. */
    int IsBound() const { return m_keybindings.Count(); }

protected:
    /** Update the binding value. Called internally by the controller, once per frame */
    void Update()
    {
        m_previous = m_current;
        m_current = false;
        for (int i = 0; i < m_keybindings.Count(); ++i)
        {
            m_current = m_current || m_keybindings[i].m1->GetKey(m_keybindings[i].m2);
        }
    }

    /** m1 is the InputDevice, m2 is the key index on the InputDevice */
    array<const InputDevice*, int> m_keybindings;
    /** Value at the previous frame */
    bool m_current;
    /** Value at the current frame */
    bool m_previous;

    friend class Controller;
};

class AxisBinding
{
public:
    AxisBinding()
      : m_current(0.0f),
        m_previous(0.0f)
    {}

    /** Gets the current absolute value of this axis */
    float GetValue() const { return m_current; }
    /** Gets the current delta value of this axis */
    float GetDelta() const { return m_current - m_previous; }

    /** Bind a physical device and axis */
    void Bind(const String& device_name, const String& axis_name);
    /** Bind a physical device and key over this axis. The axis value will be 0 if the key is up and 1 if it's down */
    void BindKey(const String& device_name, const String& key_name);
    /** Bind physical device and keys over this axis. The axis value will be 0 if both the key are up, -1 if minkey is down, and 1 if maxkey is down */
    void BindKeys(const String& device_name, const String& min_key_name, const String& max_key_name);
    /** Unbind a previously bound physical device and axis. Returns true if the binding was existing. */
    bool Unbind(const String& device_name, const String& axis_name);
    /** Unbind a previously bound physical device and axis. Returns true if the binding was existing. */
    bool UnbindKey(const String& device_name, const String& key_name);
    /** Unbind a previously bound physical device and axis. Returns true if the binding was existing. */
    bool UnbindKeys(const String& device_name, const String& min_key_name, const String& max_key_name);
    /* Small helpers */
    void BindMouse(const String& axis_name)                                     { Bind(g_name_mouse, axis_name); }
    void BindMouseKey(const String& key_name)                                   { BindKey(g_name_mouse, key_name); }
    void BindMouseKeys(const String& min_key_name, const String& max_key_name)  { BindKeys(g_name_mouse, min_key_name, max_key_name); }
    bool UnbindMouse(const String& axis_name)                                   { return Unbind(g_name_mouse, axis_name); }
    bool UnbindMouseKey(const String& key_name)                                 { return UnbindKey(g_name_mouse, key_name); }
    bool UnbindMouseKeys(const String& min_key_name, const String& max_key_name){ return UnbindKeys(g_name_mouse, min_key_name, max_key_name); }
    /** Clear current binding */
    void ClearBindings();
    /** Indicate wheither a physical device and axis has been bound. Returns the number of bindings set. */
    int IsBound() const { return m_axisbindings.Count() + m_keybindings.Count(); }


protected:
    void Update() { m_previous = m_current; m_current = IsBound() ? RetrieveCurrentValue() : 0.0f; }
    float RetrieveCurrentValue();

    /** m1 is the InputDevice, m2 is the axis index on the InputDevice, m3 and m4 are an optional key indices to bind one or two keys over the axis */
    array<const InputDevice*, int> m_axisbindings;
    /** m1 is the InputDevice, m2 is the key index on the InputDevice for the negative value, m3 is the key index on the InputDevice for the positive value. Only one key is required to bind key over axis. */
    array<const InputDevice*, int, int> m_keybindings;
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
    static array<Controller*> DeactivateAll();

    KeyBinding& GetKey(int index) { return m_keys[index]; }
    AxisBinding& GetAxis(int index) { return m_axis[index]; }

    static Controller* Get(char const* name);

protected:
    array<KeyBinding> m_keys;
    array<AxisBinding> m_axis;

private:
    static array<Controller*> controllers;
    String m_name;
    bool m_activate_nextframe;
    bool m_deactivate_nextframe;
    bool m_active;
};

} /* namespace lol */

#endif // __LOL_INPUT_CONTROLLER_H__

