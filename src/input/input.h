//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Input static class
// ----------------------
//

#if !defined __LOL_INPUT_INPUT_H__
#define __LOL_INPUT_INPUT_H__

#include <cstring>
#include <string.h>
#include "core.h"
#include "lol/math/vector.h"
#include "input/stick.h"

namespace lol
{

typedef std::string ACTION_TYPE;
static inline int ACTION_CMP(ACTION_TYPE a, ACTION_TYPE b) { return a.compare(b) == 0; }

class WorldEntity;

/*
 * The key enumeration values match libSDL's so that we can
 * easily pass information between Lol and SDL.
 */
struct Key
{
    enum Value
    {
        /* ASCII mapped keys */
        Unknown = 0,
        First = 0,
        Backspace = 8,
        Tab = 9,
        Clear = 12,
        Return = 13,
        Pause = 19,
        Escape = 27,
        Space = 32,
        Exclaim = 33,
        DoubleQuote = 34,
        Hash = 35,
        Dollar = 36,
        Ampersand = 38,
        Quote = 39,
        LeftParen = 40,
        RightParen = 41,
        Asterisk = 42,
        Plus = 43,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        K0 = 48,
        K1 = 49,
        K2 = 50,
        K3 = 51,
        K4 = 52,
        K5 = 53,
        K6 = 54,
        K7 = 55,
        K8 = 56,
        K9 = 57,
        Colon = 58,
        Semicolon = 59,
        Less = 60,
        Equals = 61,
        Greater = 62,
        Question = 63,
        At = 64,
        /* XXX: SDL decides to skip uppercase characters */
        LeftBracket = 91,
        BackSlash = 92,
        RightBracket = 93,
        Caret = 94,
        Underscore = 95,
        Backquote = 96,
        A = 97,
        B = 98,
        C = 99,
        D = 100,
        E = 101,
        F = 102,
        G = 103,
        H = 104,
        I = 105,
        J = 106,
        K = 107,
        L = 108,
        M = 109,
        N = 110,
        O = 111,
        P = 112,
        Q = 113,
        R = 114,
        S = 115,
        T = 116,
        U = 117,
        V = 118,
        W = 119,
        X = 120,
        Y = 121,
        Z = 122,
        Delete = 127,

        /* International keyboard syms */
        World0 = 160, /* 0xA0 */
        World1 = 161,
        World2 = 162,
        World3 = 163,
        World4 = 164,
        World5 = 165,
        World6 = 166,
        World7 = 167,
        World8 = 168,
        World9 = 169,
        World10 = 170,
        World11 = 171,
        World12 = 172,
        World13 = 173,
        World14 = 174,
        World15 = 175,
        World16 = 176,
        World17 = 177,
        World18 = 178,
        World19 = 179,
        World20 = 180,
        World21 = 181,
        World22 = 182,
        World23 = 183,
        World24 = 184,
        World25 = 185,
        World26 = 186,
        World27 = 187,
        World28 = 188,
        World29 = 189,
        World30 = 190,
        World31 = 191,
        World32 = 192,
        World33 = 193,
        World34 = 194,
        World35 = 195,
        World36 = 196,
        World37 = 197,
        World38 = 198,
        World39 = 199,
        World40 = 200,
        World41 = 201,
        World42 = 202,
        World43 = 203,
        World44 = 204,
        World45 = 205,
        World46 = 206,
        World47 = 207,
        World48 = 208,
        World49 = 209,
        World50 = 210,
        World51 = 211,
        World52 = 212,
        World53 = 213,
        World54 = 214,
        World55 = 215,
        World56 = 216,
        World57 = 217,
        World58 = 218,
        World59 = 219,
        World60 = 220,
        World61 = 221,
        World62 = 222,
        World63 = 223,
        World64 = 224,
        World65 = 225,
        World66 = 226,
        World67 = 227,
        World68 = 228,
        World69 = 229,
        World70 = 230,
        World71 = 231,
        World72 = 232,
        World73 = 233,
        World74 = 234,
        World75 = 235,
        World76 = 236,
        World77 = 237,
        World78 = 238,
        World79 = 239,
        World80 = 240,
        World81 = 241,
        World82 = 242,
        World83 = 243,
        World84 = 244,
        World85 = 245,
        World86 = 246,
        World87 = 247,
        World88 = 248,
        World89 = 249,
        World90 = 250,
        World91 = 251,
        World92 = 252,
        World93 = 253,
        World94 = 254,
        World95 = 255, /* 0xFF */

        /* Numeric keypad */
        KP0 = 256,
        KP1 = 257,
        KP2 = 258,
        KP3 = 259,
        KP4 = 260,
        KP5 = 261,
        KP6 = 262,
        KP7 = 263,
        KP8 = 264,
        KP9 = 265,
        KPPeriod = 266,
        KPDivide = 267,
        KPMultiply = 268,
        KPMinus = 269,
        KPPlus = 270,
        KPEnter = 271,
        KPEquals = 272,

        /* Arrows + Home/End pad */
        Up = 273,
        Down = 274,
        Right = 275,
        Left = 276,
        Insert = 277,
        Home = 278,
        End = 279,
        PageUp = 280,
        PageDown = 281,

        /* Function keys */
        F1 = 282,
        F2 = 283,
        F3 = 284,
        F4 = 285,
        F5 = 286,
        F6 = 287,
        F7 = 288,
        F8 = 289,
        F9 = 290,
        F10 = 291,
        F11 = 292,
        F12 = 293,
        F13 = 294,
        F14 = 295,
        F15 = 296,

        /* Modifier keys */
        NumLock = 300,
        CapsLock = 301,
        ScrollLock= 302,
        RightShift = 303,
        LeftShift = 304,
        RightCtrl = 305,
        LeftCtrl = 306,
        RightAlt = 307,
        LeftAlt = 308,
        RightMeta = 309,
        LeftMeta = 310,
        LeftSuper = 311,  /* Left "Windows" key */
        RightSuper = 312, /* Right "Windows" key */
        Mode = 313,       /* "Alt Gr" key */
        Compose = 314,    /* Multi-key compose key */

        /* Miscellaneous function keys */
        Help = 315,
        Print = 316,
        SysReq = 317,
        Break = 318,
        Menu = 319,
        Power = 320, /* Power Macintosh power key */
        Euro = 321,  /* Some european keyboards */
        Undo = 322,  /* Atari keyboard has Undo */

        /* Add any other keys here */
        Last

    }
    m_value;

    //BH : Removed KMod from main enum, because I don't have any idea about handling them correctly for now.
    /*
        //Enumeration of valid key mods (possibly OR'd together)
        KM_NONE          = 0x0000,
        KM_LSHIFT        = 0x0001,
        KM_RSHIFT        = 0x0002,
        KM_LCTRL         = 0x0040,
        KM_RCTRL         = 0x0080,
        KM_LALT          = 0x0100,
        KM_RALT          = 0x0200,
        KM_LMETA         = 0x0400,
        KM_RMETA         = 0x0800,
        KM_NUM           = 0x1000,
        KM_CAPS          = 0x2000,
        KM_MODE          = 0x4000,

        KM_RESERVED        = 0x8000,

        //Left/Right independent key mods definition
        KM_CTRL            = (KM_LCTRL|KM_RCTRL),
        KM_SHIFT        = (KM_LSHIFT|KM_RSHIFT),
        KM_ALT            = (KM_LALT|KM_RALT),
        KM_META            = (KM_LMETA|KM_RMETA),
    */

    inline Key(Value v) { m_value = v; }
    inline operator Value() { return m_value; }
    inline bool operator==(const Key& val)
    {
        return m_value == val.m_value;
    }
};

struct ActionSetting
{
    ACTION_TYPE                m_action;
    float                    m_buffering_time;
    float                    m_buffered_since;

    ActionSetting(ACTION_TYPE NewAction) :
        m_action(NewAction),
        m_buffering_time(.0f),
        m_buffered_since(.0f)
    { }
};

struct ButtonSetting
{
    Key                        m_raw_button;
    Array<ActionSetting>    m_associated_action_list;

    ButtonSetting(Key NewRawButton)
        : m_raw_button(NewRawButton) { }
    int GetActionSettingIdx(ACTION_TYPE SearchAction);
};

class InputTracker : public Entity
{

    friend class Input;

public:
    InputTracker();

private:
    Array<uint8_t>            m_input_status;
    Array<ButtonSetting>    m_input_assocation_list;

    int                        GetButtonSettingIdx(struct Key Button);
    int                        GetCurrentButtonStatus(struct Key Button);
    int                        GetPreviousButtonStatus(struct Key Button);
    void                    UpdateActionStatus(float seconds);

protected:
    virtual char const *    GetName()
    {
        return "<InputTracker>";
    }
    virtual void            TickGame(float seconds)
    {
        Entity::TickGame(seconds);

        UpdateActionStatus(seconds);
    }

    void                    LinkActionToKey(ACTION_TYPE Action, struct Key Button);
    void                    UnlinkAction(ACTION_TYPE Action);
    int                        GetStatus(ACTION_TYPE Action);
    bool                    WasPressed(ACTION_TYPE Action);
    bool                    WasReleased(ACTION_TYPE Action);

    //You should probably use the Action System
    int                        GetStatus(Key Button);
    bool                    WasPressed(Key Button);
    bool                    WasReleased(Key Button);
};

class Input
{
private:
    static InputTracker*        m_input_tracker;

    static bool                    CheckInputTrackerInit()
    {
        if (Input::m_input_tracker)
            return true;

        Input::m_input_tracker = new InputTracker();
        return true;
    }

public:

    /* These methods are general queries */
    static ivec2 GetMousePos();
    static ivec3 GetMouseButtons();

    //BH : Shouldn't use this
    static int GetButtonState(int button);

    /* Action management */
    static void LinkActionToKey(ACTION_TYPE Action, struct Key Button);
    static void UnlinkAction(ACTION_TYPE Action);
    static int GetStatus(ACTION_TYPE Action);
    static bool    WasPressed(ACTION_TYPE Action);
    static bool WasReleased(ACTION_TYPE Action);

    /* Raw Button management. You should use actions. */
    static int GetStatus(Key Button);
    static bool    WasPressed(Key Button);
    static bool WasReleased(Key Button);

    /* Entities can subscribe to events */
    static void TrackMouse(WorldEntity *e);
    static void UntrackMouse(WorldEntity *e);

    /* These methods are called by the underlying input listeners */
    static void SetMousePos(ivec2 coord);
    static void SetMouseButton(int index);
    static void UnsetMouseButton(int index);

    /* Joystick handling */
    static Stick *CreateStick();
    static void DestroyStick(Stick *stick);

    static Stick *TrackStick(int desired);
    static void UntrackStick(Stick *stick);
};

} /* namespace lol */

#endif // __LOL_INPUT_INPUT_H__

