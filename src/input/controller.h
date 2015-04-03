//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

namespace lol
{

//-----------------------------------------------------------------------------
class KeyBinding
{
    friend class Controller;
public:
    KeyBinding()
      : m_current(false),
        m_previous(false)
    {}

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
    void Bind(const String& device_name, const String& key_name);
    /** Unbind a previously bound physical device and key. Returns true if the binding was existing. */
    bool Unbind(const String& device_name, const String& key_name);
    /* Small helpers */
    void BindMouse(const String& key_name)                          { Bind(g_name_mouse, key_name); }
    void BindKeyboard(const String& key_name)                       { Bind(g_name_keyboard, key_name); }
    void BindJoystick(const uint64_t num, const String& key_name)   { Bind(g_name_joystick(num), key_name); }
    bool UnbindMouse(const String& key_name)                        { return Unbind(g_name_mouse, key_name); }
    bool UnbindKeyboard(const String& key_name)                     { return Unbind(g_name_keyboard, key_name); }
    bool UnbindJoystick(const uint64_t num, const String& key_name) { return Unbind(g_name_joystick(num), key_name); }
    /** Clear current binding */
    void ClearBindings();
    /** Indicate wheither a physical device and key has been bound. Returns the number of bindings set. */
    ptrdiff_t IsBound() const { return m_keybindings.Count(); }

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
};

//-----------------------------------------------------------------------------
class AxisBinding
{
    friend class Controller;
public:
    AxisBinding()
      : m_current(0.0f),
        m_previous(0.0f)
    {}

protected:
    //Status methods ----------------------------------------------------------
    /** Gets the current absolute value of this axis */
    float GetValue() const { return m_current; }
    /** Gets the current delta value of this axis */
    float GetDelta() const { return m_current - m_previous; }

public:
    //Binding methods ---------------------------------------------------------
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
    /* */
    void BindJoystick(const uint64_t num, const String& axis_name)                                     { Bind(g_name_joystick(num), axis_name); }
    void BindJoystickKey(const uint64_t num, const String& key_name)                                   { BindKey(g_name_joystick(num), key_name); }
    void BindJoystickKeys(const uint64_t num, const String& min_key_name, const String& max_key_name)  { BindKeys(g_name_joystick(num), min_key_name, max_key_name); }
    bool UnbindJoystick(const uint64_t num, const String& axis_name)                                   { return Unbind(g_name_joystick(num), axis_name); }
    bool UnbindJoystickKey(const uint64_t num, const String& key_name)                                 { return UnbindKey(g_name_joystick(num), key_name); }
    bool UnbindJoystickKeys(const uint64_t num, const String& min_key_name, const String& max_key_name){ return UnbindKeys(g_name_joystick(num), min_key_name, max_key_name); }
    /** Clear current binding */
    void ClearBindings();
    /** Indicate wheither a physical device and axis has been bound. Returns the number of bindings set. */
    ptrdiff_t IsBound() const { return m_axisbindings.Count() + m_keybindings.Count(); }

protected:
    void Update()
    {
        m_previous = m_current;
        m_current = IsBound() ? RetrieveCurrentValue() : 0.0f;
    }
    float RetrieveCurrentValue();

    /** m1 is the InputDevice, m2 is the axis index on the InputDevice, m3 and m4 are an optional key indices to bind one or two keys over the axis */
    array<const InputDevice*, int> m_axisbindings;
    /** m1 is the InputDevice, m2 is the key index on the InputDevice for the negative value, m3 is the key index on the InputDevice for the positive value. Only one key is required to bind key over axis. */
    array<const InputDevice*, int, int> m_keybindings;
    float m_current;
    float m_previous;
};

//-------------------------------------------------------------------------
class InputProfile
{
    friend class Controller;
private:
    //---------------------------------------------------------------------
    class Key
    {
        friend class Controller;
        friend class InputProfile;
    public:
        Key() { }
        Key(int idx, String const& name) : m_idx(idx), m_name(name) { }
        Key(const Key& other) : m_idx(other.m_idx), m_name(other.m_name) { }
        ~Key() { }
        bool operator==(const Key& other) { return m_name == other.m_name; }
    private:
        int m_idx = 0;
        String m_name;
    };
    //---------------------------------------------------------------------
    class Joystick
    {
        friend class Controller;
        friend class InputProfile;
    public:
        Joystick() { }
        Joystick(uint64_t joy, int idx, String const& name) : m_joy(joy), m_idx(idx), m_name(name) { }
        Joystick(const Joystick& other) : m_joy(other.m_joy), m_idx(other.m_idx), m_name(other.m_name) { }
        ~Joystick() { }
        bool operator==(const Joystick& other) { return m_name == other.m_name; }
    private:
        uint64_t m_joy = 0;
        int m_idx = 0;
        String m_name;
    };
public:
    //---------------------------------------------------------------------
    class Keyboard : public Key
    {
        friend class Controller;
        friend class InputProfile;
    public:
        Keyboard() : Key() { }
        Keyboard(int idx, String const& name) : Key(idx, name) { }
        Keyboard(const Keyboard& other) : Key(other.m_idx, other.m_name) { }
    };
    //---------------------------------------------------------------------
    class MouseKey : public Key
    {
        friend class Controller;
        friend class InputProfile;
    public:
        MouseKey() : Key() { }
        MouseKey(int idx, String const& name) : Key(idx, name) { }
        MouseKey(const Keyboard& other) : Key(other.m_idx, other.m_name) { }
    };
    //---------------------------------------------------------------------
    class MouseAxis : public Key
    {
        friend class Controller;
        friend class InputProfile;
    public:
        MouseAxis() : Key() { }
        MouseAxis(int idx, String const& name) : Key(idx, name) { }
        MouseAxis(const Keyboard& other) : Key(other.m_idx, other.m_name) { }
    };
    //---------------------------------------------------------------------
    class JoystickKey : public Joystick
    {
        friend class Controller;
        friend class InputProfile;
    public:
        JoystickKey() : Joystick() { }
        JoystickKey(uint64_t joy, int idx, String const& name) : Joystick(joy, idx, name) { }
        JoystickKey(const JoystickKey& other) : Joystick(other.m_joy, other.m_idx, other.m_name) { }
    };
    //---------------------------------------------------------------------
    class JoystickAxis : public Joystick
    {
        friend class Controller;
        friend class InputProfile;
    public:
        JoystickAxis() : Joystick() { }
        JoystickAxis(uint64_t joy, int idx, String const& name) : Joystick(joy, idx, name) { }
        JoystickAxis(const JoystickAxis& other) : Joystick(other.m_joy, other.m_idx, other.m_name) { }
    };
public:
    InputProfile() { }
    InputProfile(const InputProfile& other)
    {
        m_keys = other.m_keys;
        m_mouse_keys = other.m_mouse_keys;
        m_mouse_axis = other.m_mouse_axis;
        m_joystick = other.m_joystick;
        m_joystick_keys = other.m_joystick_keys;
        m_joystick_axis = other.m_joystick_axis;
    }
    virtual ~InputProfile() { }

    bool IsEmpty() const
    {
        return !GetKeyCount() && !GetAxisCount();
    }
    int GetKeyCount() const
    {
        return (int)(m_keys.Count() + m_mouse_keys.Count() + m_joystick_keys.Count());
    }
    int GetAxisCount() const
    {
        return (int)(m_mouse_axis.Count() + m_joystick_axis.Count());
    }
    //Keys --------------------------------------------------------------------
    InputProfile& operator<<(InputProfile::Keyboard const& binding)
    {
        m_keys.PushUnique(binding);
        return *this;
    }
    InputProfile& operator<<(array<InputProfile::Keyboard> const& bindings)
    {
        m_keys += bindings;
        return *this;
    }
    //------
    InputProfile& operator<<(InputProfile::MouseKey const& binding)
    {
        m_mouse_keys.PushUnique(binding);
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
        m_joystick.PushUnique(binding.m_joy);
        m_joystick_keys.PushUnique(binding);
        return *this;
    }
    InputProfile& operator<<(array<InputProfile::JoystickKey> const& bindings)
    {
        for (InputProfile::JoystickKey const& binding : bindings)
            m_joystick.PushUnique(binding.m_joy);
        m_joystick_keys += bindings;
        return *this;
    }
    //Axis --------------------------------------------------------------------
    InputProfile& operator<<(InputProfile::MouseAxis const& binding)
    {
        m_mouse_axis.PushUnique(binding);
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
        m_joystick.PushUnique(binding.m_joy);
        m_joystick_axis.PushUnique(binding);
        return *this;
    }
    InputProfile& operator<<(array<InputProfile::JoystickAxis> const& bindings)
    {
        for (InputProfile::JoystickAxis const& binding : bindings)
            m_joystick.PushUnique(binding.m_joy);
        m_joystick_axis += bindings;
        return *this;
    }

    //BindingType -------------------------------------------------------------
    struct InputTypeBase : public StructSafeEnum
    {
        enum Type
        {
            Keyboard = 0,
            MouseKey,
            JoystickKey,
            MouseAxis,
            JoystickAxis,

            MAX,
        };
    protected:
        virtual bool BuildEnumMap(map<int64_t, String>& enum_map) { return true; }
    };
    typedef SafeEnum<InputTypeBase> InputType;

    //Template helper ---------------------------------------------------------
    template <typename T, ptrdiff_t BEGIN, ptrdiff_t END>
    void AddBindings(InputType const& type, uint64_t joy = 0)
    {
        for (ptrdiff_t i = BEGIN; i < END; ++i)
        {
            switch (type.ToScalar())
            {
            case InputType::Keyboard:/******/*this << InputProfile::Keyboard/******/(/***/(int)i, T((int)i).ToString()); break;
            case InputType::MouseKey:/******/*this << InputProfile::MouseKey/******/(/***/(int)i, T((int)i).ToString()); break;
            case InputType::JoystickKey:/***/*this << InputProfile::JoystickKey/***/(joy, (int)i, T((int)i).ToString()); break;
            case InputType::MouseAxis:/*****/*this << InputProfile::MouseAxis/*****/(/***/(int)i, T((int)i).ToString()); break;
            case InputType::JoystickAxis:/**/*this << InputProfile::JoystickAxis/**/(joy, (int)i, T((int)i).ToString()); break;
            }
        }
    }

private:
    array<Keyboard>     m_keys;
    array<MouseKey>     m_mouse_keys;
    array<MouseAxis>    m_mouse_axis;
    array<uint64_t>     m_joystick;
    array<JoystickKey>  m_joystick_keys;
    array<JoystickAxis> m_joystick_axis;
};
typedef InputProfile::InputType InputProfileType;

//-----------------------------------------------------------------------------
//TODO: Add mask|layer system to prevent several controllers from interfering with another. (input overlay in menus)
class Controller : public Entity
{
public:
    Controller(String const &name);
    Controller(String const &name, InputProfile const& setup);
    virtual ~Controller();

    virtual void TickGame(float seconds);

    /** Activate the controller on next frame */
    void Activate();
    /** Deactivate the controller on next frame */
    void Deactivate();
    /** Deactivate every active controller on next frame and return an array of deactivated (previously active) controllers */
    static array<Controller*> DeactivateAll();

    /** Init mode 1: Input profile system */
    void Init(InputProfile const& profile);
    void ClearProfile();

    /** Init mode 2: By hand, key/axis by key/axis */
    void SetInputCount(int nb_keys, int nb_axis);

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

    /** Get named controller */
    static Controller* Get(String const &name);

protected:
    /** Input profile system */
    void UnbindProfile();
    void BindProfile(InputProfile const& setup);

private:
    uint32_t m_layer_mask = 1; //plugged on the first by default
    array<KeyBinding> m_keys;
    array<AxisBinding> m_axis;

    static uint32_t m_active_layer; //All active by default
    static array<Controller*> controllers;
    String m_name;
    bool m_activate_nextframe;
    bool m_deactivate_nextframe;
    bool m_active;

    //Input profile stuff
    mutex m_mutex;
    class InputProfile m_profile;
    class InputDevice* m_keyboard = nullptr;
    class InputDevice* m_mouse = nullptr;
    array<class InputDevice*> m_joystick;
    array<uint64_t> m_joystick_idx;
};

} /* namespace lol */

