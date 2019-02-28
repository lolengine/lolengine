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

#pragma once

#include <string>
#include <map>

namespace lol
{

//-----------------------------------------------------------------------------
class KeyBinding
{
    friend class Controller;

protected:
    //Status methods ----------------------------------------------------------
    /** Indicates wheither the key is currently down */
    bool IsPressed() const { return m_current; }
    /** Indicates wheither the key is currently up */
    bool IsReleased() const { return !m_current; }
    /** Indicates wheither the key has just been pressed */
    bool WasPressedThisFrame() const { return m_current && !m_previous; }
    /** Indicates wheither the key has just been released */
    bool WasReleasedThisFrame() const { return !m_current && m_previous; }

public:
    //Binding methods ---------------------------------------------------------
    /** Bind a physical device and key */
    void Bind(const std::string& device_name, const std::string& key_name);
    /** Unbind a previously bound physical device and key. Returns true if the binding was existing. */
    bool Unbind(const std::string& device_name, const std::string& key_name);
    /* Small helpers */
    void BindMouse(const std::string& key_name)                          { Bind(g_name_mouse, key_name); }
    void BindKeyboard(const std::string& key_name)                       { Bind(g_name_keyboard, key_name); }
    void BindJoystick(const uint64_t num, const std::string& key_name)   { Bind(g_name_joystick(num), key_name); }
    bool UnbindMouse(const std::string& key_name)                        { return Unbind(g_name_mouse, key_name); }
    bool UnbindKeyboard(const std::string& key_name)                     { return Unbind(g_name_keyboard, key_name); }
    bool UnbindJoystick(const uint64_t num, const std::string& key_name) { return Unbind(g_name_joystick(num), key_name); }

protected:
    /** Update the binding value. Called internally by the controller, once per frame */
    void Update()
    {
        m_previous = m_current;
        m_current = false;
        for (int i = 0; i < m_keybindings.count(); ++i)
            m_current = m_current || m_keybindings[i].m1->key(m_keybindings[i].m2);
    }

    /** m1 is the InputDevice, m2 is the key index on the InputDevice */
    array<const InputDevice*, int> m_keybindings;
    /** Value at the previous frame */
    bool m_current = false;
    /** Value at the current frame */
    bool m_previous = false;
};

//-----------------------------------------------------------------------------
class AxisBinding
{
    friend class Controller;

protected:
    //Status methods ----------------------------------------------------------
    /** Gets the current absolute value of this axis */
    float GetValue() const { return m_current; }
    /** Gets the current delta value of this axis */
    float GetDelta() const { return m_current - m_previous; }

public:
    //Binding methods ---------------------------------------------------------
    /** Bind a physical device and axis */
    void Bind(const std::string& device_name, const std::string& axis_name);
    /** Bind a physical device and key over this axis. The axis value will be 0 if the key is up and 1 if it's down */
    void BindKey(const std::string& device_name, const std::string& key_name);
    /** Bind physical device and keys over this axis. The axis value will be 0 if both the key are up, -1 if minkey is down, and 1 if maxkey is down */
    void BindKeys(const std::string& device_name, const std::string& min_key_name, const std::string& max_key_name);
    /** Unbind a previously bound physical device and axis. Returns true if the binding was existing. */
    bool Unbind(const std::string& device_name, const std::string& axis_name);
    /** Unbind a previously bound physical device and axis. Returns true if the binding was existing. */
    bool UnbindKey(const std::string& device_name, const std::string& key_name);
    /** Unbind a previously bound physical device and axis. Returns true if the binding was existing. */
    bool UnbindKeys(const std::string& device_name, const std::string& min_key_name, const std::string& max_key_name);
    /* Small helpers */
    void BindMouse(const std::string& axis_name)   { Bind(g_name_mouse, axis_name); }
    bool UnbindMouse(const std::string& axis_name) { return Unbind(g_name_mouse, axis_name); }
    /* */
    void BindJoystick(const uint64_t num, const std::string& axis_name)   { Bind(g_name_joystick(num), axis_name); }
    bool UnbindJoystick(const uint64_t num, const std::string& axis_name) { return Unbind(g_name_joystick(num), axis_name); }

protected:
    void Update()
    {
        m_previous = m_current;
        m_current = RetrieveCurrentValue();
    }
    float RetrieveCurrentValue();

    /** m1 is the InputDevice, m2 is the axis index on the InputDevice */
    array<const InputDevice*, int> m_axisbindings;
    /** m1 is the InputDevice, m2 is the key index on the InputDevice for the negative value, m3 is the key index on the InputDevice for the positive value. Only one key is required to bind key over axis. */
    array<const InputDevice*, int, int> m_keybindings;
    float m_current = 0.0f;
    float m_previous = 0.0f;
};

//-------------------------------------------------------------------------
class InputProfile
{
    friend class Controller;

private:
    class Key
    {
        friend class Controller;
        friend class InputProfile;
    public:
        Key() { }
        Key(int idx, std::string const& name) : m_idx(idx), m_name(name) { }
        Key(const Key& other) : m_idx(other.m_idx), m_name(other.m_name) { }
        ~Key() { }
        bool operator==(const Key& other) { return m_name == other.m_name; }
    private:
        int m_idx = 0;
        std::string m_name;
    };

    class Joystick
    {
        friend class Controller;
        friend class InputProfile;
    public:
        Joystick() { }
        Joystick(uint64_t joy, int idx, std::string const& name) : m_joy(joy), m_idx(idx), m_name(name) { }
        Joystick(const Joystick& other) : m_joy(other.m_joy), m_idx(other.m_idx), m_name(other.m_name) { }
        ~Joystick() { }
        bool operator==(const Joystick& other) { return m_name == other.m_name; }
    private:
        uint64_t m_joy = 0;
        int m_idx = 0;
        std::string m_name;
    };

public:
    class KeyboardKey : public Key
    {
        friend class Controller;
        friend class InputProfile;
    public:
        KeyboardKey(int idx, std::string const& name) : Key(idx, name) { }
    };

    class MouseKey : public Key
    {
        friend class Controller;
        friend class InputProfile;
    public:
        MouseKey(int idx, std::string const& name) : Key(idx, name) { }
    };

    class MouseAxis : public Key
    {
        friend class Controller;
        friend class InputProfile;
    public:
        MouseAxis(int idx, std::string const& name) : Key(idx, name) { }
    };

    class JoystickKey : public Joystick
    {
        friend class Controller;
        friend class InputProfile;
    public:
        JoystickKey(uint64_t joy, int idx, std::string const& name) : Joystick(joy, idx, name) { }
    };

    class JoystickAxis : public Joystick
    {
        friend class Controller;
        friend class InputProfile;
    public:
        JoystickAxis(uint64_t joy, int idx, std::string const& name) : Joystick(joy, idx, name) { }
    };

public:
    InputProfile() = default;
    virtual ~InputProfile() = default;

    bool IsEmpty() const
    {
        return !GetKeyCount() && !GetAxisCount();
    }
    int GetKeyCount() const
    {
        return m_keys.count() + m_mouse_keys.count() + m_joystick_keys.count();
    }
    int GetAxisCount() const
    {
        return m_mouse_axis.count() + m_joystick_axis.count();
    }
    //Keys --------------------------------------------------------------------
    InputProfile& operator<<(InputProfile::KeyboardKey const& binding)
    {
        m_keys.push_unique(binding);
        return *this;
    }
    InputProfile& operator<<(array<InputProfile::KeyboardKey> const& bindings)
    {
        m_keys += bindings;
        return *this;
    }
    //------
    InputProfile& operator<<(InputProfile::MouseKey const& binding)
    {
        m_mouse_keys.push_unique(binding);
        return *this;
    }
    InputProfile& operator<<(array<InputProfile::MouseKey> const& bindings)
    {
        m_mouse_keys += bindings;
        return *this;
    }
    //------
    InputProfile& operator<<(InputProfile::JoystickKey const& binding)
    {
        m_joystick.push_unique(binding.m_joy);
        m_joystick_keys.push_unique(binding);
        return *this;
    }
    InputProfile& operator<<(array<InputProfile::JoystickKey> const& bindings)
    {
        for (InputProfile::JoystickKey const& binding : bindings)
            m_joystick.push_unique(binding.m_joy);
        m_joystick_keys += bindings;
        return *this;
    }
    //Axis --------------------------------------------------------------------
    InputProfile& operator<<(InputProfile::MouseAxis const& binding)
    {
        m_mouse_axis.push_unique(binding);
        return *this;
    }
    InputProfile& operator<<(array<InputProfile::MouseAxis> const& bindings)
    {
        m_mouse_axis += bindings;
        return *this;
    }
    //------
    InputProfile& operator<<(InputProfile::JoystickAxis const& binding)
    {
        m_joystick.push_unique(binding.m_joy);
        m_joystick_axis.push_unique(binding);
        return *this;
    }
    InputProfile& operator<<(array<InputProfile::JoystickAxis> const& bindings)
    {
        for (InputProfile::JoystickAxis const& binding : bindings)
            m_joystick.push_unique(binding.m_joy);
        m_joystick_axis += bindings;
        return *this;
    }

    void register_default_keys()
    {
#define _SC(id, str, name) *this << InputProfile::KeyboardKey(id, #name);
#include "ui/keys.inc"
    }

private:
    array<KeyboardKey>  m_keys;
    array<MouseKey>     m_mouse_keys;
    array<MouseAxis>    m_mouse_axis;
    array<uint64_t>     m_joystick;
    array<JoystickKey>  m_joystick_keys;
    array<JoystickAxis> m_joystick_axis;
};

//-----------------------------------------------------------------------------
//TODO: Add mask|layer system to prevent several controllers from interfering with another. (input overlay in menus)
class Controller : public Entity
{
public:
    Controller(std::string const &name);
    Controller(std::string const &name, InputProfile const& setup);
    virtual ~Controller();

    virtual void tick_game(float seconds);

    /** Activate the controller on next frame */
    void Activate();
    /** Deactivate the controller on next frame */
    void Deactivate();
    /** Deactivate every active controller on next frame and return an array of deactivated (previously active) controllers */
    static array<Controller*> DeactivateAll();

    /** Init mode 1: Input profile system */
    void Init(InputProfile const& profile);
    void ClearProfile();

    /** layer mask stuff */
    void SetLayerMask(uint32_t layer_mask);
    uint32_t GetLayerMask();
protected:
    bool IsLayerActive();
public:

    /** GetKeys/Axis stuff */
    KeyBinding& GetKey(int index);
    KeyBinding const& GetKey(int index) const;
    AxisBinding& GetAxis(int index);
    AxisBinding const& GetAxis(int index) const;

    /** Key methods: should not go directly to binding */
    /** Indicates wheither the key is currently down */
    bool IsKeyPressed(int index) const;
    /** Indicates wheither the key is currently up */
    bool IsKeyReleased(int index) const;
    /** Indicates wheither the key has just been pressed */
    bool WasKeyPressedThisFrame(int index) const;
    /** Indicates wheither the key has just been released */
    bool WasKeyReleasedThisFrame(int index) const;

    /** Axis methods: should not go directly to binding */
    /** Gets the current absolute value of this axis */
    float GetAxisValue(int index) const;
    /** Gets the current delta value of this axis */
    float GetAxisDelta(int index) const;

protected:
    /** Input profile system */
    void UnbindProfile();
    void BindProfile(InputProfile const& setup);

private:
    uint32_t m_layer_mask = 1; // plugged on the first by default
    std::map<int, KeyBinding> m_key_bindings;
    std::map<int, AxisBinding> m_axis_bindings;

    std::string m_name;
    bool m_activate_nextframe;
    bool m_deactivate_nextframe;
    bool m_active;

    //Input profile stuff
    mutex m_mutex;
    class InputProfile m_profile;
    std::shared_ptr<InputDevice> m_keyboard;
    std::shared_ptr<InputDevice> m_mouse;
    array<class InputDevice*> m_joysticks;
    array<uint64_t> m_joystick_idx;
};

} /* namespace lol */

