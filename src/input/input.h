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
#include "core.h"
#include "lol/math/vector.h"
#include "input/stick.h"

namespace lol
{

class WorldEntity;

//FULL Key reap-off of the SDLK enum
typedef enum {
    /** @name ASCII mapped keysyms
        *  The keyboard syms have been cleverly chosen to map to ASCII
        */
    /*@{*/
	LOLK_UNKNOWN		= 0,
	LOLK_FIRST		= 0,
	LOLK_BACKSPACE		= 8,
	LOLK_TAB		= 9,
	LOLK_CLEAR		= 12,
	LOLK_RETURN		= 13,
	LOLK_PAUSE		= 19,
	LOLK_ESCAPE		= 27,
	LOLK_SPACE		= 32,
	LOLK_EXCLAIM		= 33,
	LOLK_QUOTEDBL		= 34,
	LOLK_HASH		= 35,
	LOLK_DOLLAR		= 36,
	LOLK_AMPERSAND		= 38,
	LOLK_QUOTE		= 39,
	LOLK_LEFTPAREN		= 40,
	LOLK_RIGHTPAREN		= 41,
	LOLK_ASTERISK		= 42,
	LOLK_PLUS		= 43,
	LOLK_COMMA		= 44,
	LOLK_MINUS		= 45,
	LOLK_PERIOD		= 46,
	LOLK_SLASH		= 47,
	LOLK_0			= 48,
	LOLK_1			= 49,
	LOLK_2			= 50,
	LOLK_3			= 51,
	LOLK_4			= 52,
	LOLK_5			= 53,
	LOLK_6			= 54,
	LOLK_7			= 55,
	LOLK_8			= 56,
	LOLK_9			= 57,
	LOLK_COLON		= 58,
	LOLK_SEMICOLON		= 59,
	LOLK_LESS		= 60,
	LOLK_EQUALS		= 61,
	LOLK_GREATER		= 62,
	LOLK_QUESTION		= 63,
	LOLK_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	LOLK_LEFTBRACKET	= 91,
	LOLK_BACKSLASH		= 92,
	LOLK_RIGHTBRACKET	= 93,
	LOLK_CARET		= 94,
	LOLK_UNDERSCORE		= 95,
	LOLK_BACKQUOTE		= 96,
	LOLK_a			= 97,
	LOLK_b			= 98,
	LOLK_c			= 99,
	LOLK_d			= 100,
	LOLK_e			= 101,
	LOLK_f			= 102,
	LOLK_g			= 103,
	LOLK_h			= 104,
	LOLK_i			= 105,
	LOLK_j			= 106,
	LOLK_k			= 107,
	LOLK_l			= 108,
	LOLK_m			= 109,
	LOLK_n			= 110,
	LOLK_o			= 111,
	LOLK_p			= 112,
	LOLK_q			= 113,
	LOLK_r			= 114,
	LOLK_s			= 115,
	LOLK_t			= 116,
	LOLK_u			= 117,
	LOLK_v			= 118,
	LOLK_w			= 119,
	LOLK_x			= 120,
	LOLK_y			= 121,
	LOLK_z			= 122,
	LOLK_DELETE		= 127,
	/* End of ASCII mapped keysyms */
        /*@}*/

	/** @name International keyboard syms */
        /*@{*/
	LOLK_WORLD_0		= 160,		/* 0xA0 */
	LOLK_WORLD_1		= 161,
	LOLK_WORLD_2		= 162,
	LOLK_WORLD_3		= 163,
	LOLK_WORLD_4		= 164,
	LOLK_WORLD_5		= 165,
	LOLK_WORLD_6		= 166,
	LOLK_WORLD_7		= 167,
	LOLK_WORLD_8		= 168,
	LOLK_WORLD_9		= 169,
	LOLK_WORLD_10		= 170,
	LOLK_WORLD_11		= 171,
	LOLK_WORLD_12		= 172,
	LOLK_WORLD_13		= 173,
	LOLK_WORLD_14		= 174,
	LOLK_WORLD_15		= 175,
	LOLK_WORLD_16		= 176,
	LOLK_WORLD_17		= 177,
	LOLK_WORLD_18		= 178,
	LOLK_WORLD_19		= 179,
	LOLK_WORLD_20		= 180,
	LOLK_WORLD_21		= 181,
	LOLK_WORLD_22		= 182,
	LOLK_WORLD_23		= 183,
	LOLK_WORLD_24		= 184,
	LOLK_WORLD_25		= 185,
	LOLK_WORLD_26		= 186,
	LOLK_WORLD_27		= 187,
	LOLK_WORLD_28		= 188,
	LOLK_WORLD_29		= 189,
	LOLK_WORLD_30		= 190,
	LOLK_WORLD_31		= 191,
	LOLK_WORLD_32		= 192,
	LOLK_WORLD_33		= 193,
	LOLK_WORLD_34		= 194,
	LOLK_WORLD_35		= 195,
	LOLK_WORLD_36		= 196,
	LOLK_WORLD_37		= 197,
	LOLK_WORLD_38		= 198,
	LOLK_WORLD_39		= 199,
	LOLK_WORLD_40		= 200,
	LOLK_WORLD_41		= 201,
	LOLK_WORLD_42		= 202,
	LOLK_WORLD_43		= 203,
	LOLK_WORLD_44		= 204,
	LOLK_WORLD_45		= 205,
	LOLK_WORLD_46		= 206,
	LOLK_WORLD_47		= 207,
	LOLK_WORLD_48		= 208,
	LOLK_WORLD_49		= 209,
	LOLK_WORLD_50		= 210,
	LOLK_WORLD_51		= 211,
	LOLK_WORLD_52		= 212,
	LOLK_WORLD_53		= 213,
	LOLK_WORLD_54		= 214,
	LOLK_WORLD_55		= 215,
	LOLK_WORLD_56		= 216,
	LOLK_WORLD_57		= 217,
	LOLK_WORLD_58		= 218,
	LOLK_WORLD_59		= 219,
	LOLK_WORLD_60		= 220,
	LOLK_WORLD_61		= 221,
	LOLK_WORLD_62		= 222,
	LOLK_WORLD_63		= 223,
	LOLK_WORLD_64		= 224,
	LOLK_WORLD_65		= 225,
	LOLK_WORLD_66		= 226,
	LOLK_WORLD_67		= 227,
	LOLK_WORLD_68		= 228,
	LOLK_WORLD_69		= 229,
	LOLK_WORLD_70		= 230,
	LOLK_WORLD_71		= 231,
	LOLK_WORLD_72		= 232,
	LOLK_WORLD_73		= 233,
	LOLK_WORLD_74		= 234,
	LOLK_WORLD_75		= 235,
	LOLK_WORLD_76		= 236,
	LOLK_WORLD_77		= 237,
	LOLK_WORLD_78		= 238,
	LOLK_WORLD_79		= 239,
	LOLK_WORLD_80		= 240,
	LOLK_WORLD_81		= 241,
	LOLK_WORLD_82		= 242,
	LOLK_WORLD_83		= 243,
	LOLK_WORLD_84		= 244,
	LOLK_WORLD_85		= 245,
	LOLK_WORLD_86		= 246,
	LOLK_WORLD_87		= 247,
	LOLK_WORLD_88		= 248,
	LOLK_WORLD_89		= 249,
	LOLK_WORLD_90		= 250,
	LOLK_WORLD_91		= 251,
	LOLK_WORLD_92		= 252,
	LOLK_WORLD_93		= 253,
	LOLK_WORLD_94		= 254,
	LOLK_WORLD_95		= 255,		/* 0xFF */
        /*@}*/

	/** @name Numeric keypad */
        /*@{*/
	LOLK_KP0		= 256,
	LOLK_KP1		= 257,
	LOLK_KP2		= 258,
	LOLK_KP3		= 259,
	LOLK_KP4		= 260,
	LOLK_KP5		= 261,
	LOLK_KP6		= 262,
	LOLK_KP7		= 263,
	LOLK_KP8		= 264,
	LOLK_KP9		= 265,
	LOLK_KP_PERIOD		= 266,
	LOLK_KP_DIVIDE		= 267,
	LOLK_KP_MULTIPLY	= 268,
	LOLK_KP_MINUS		= 269,
	LOLK_KP_PLUS		= 270,
	LOLK_KP_ENTER		= 271,
	LOLK_KP_EQUALS		= 272,
        /*@}*/

	/** @name Arrows + Home/End pad */
        /*@{*/
	LOLK_UP			= 273,
	LOLK_DOWN		= 274,
	LOLK_RIGHT		= 275,
	LOLK_LEFT		= 276,
	LOLK_INSERT		= 277,
	LOLK_HOME		= 278,
	LOLK_END		= 279,
	LOLK_PAGEUP		= 280,
	LOLK_PAGEDOWN		= 281,
        /*@}*/

	/** @name Function keys */
        /*@{*/
	LOLK_F1			= 282,
	LOLK_F2			= 283,
	LOLK_F3			= 284,
	LOLK_F4			= 285,
	LOLK_F5			= 286,
	LOLK_F6			= 287,
	LOLK_F7			= 288,
	LOLK_F8			= 289,
	LOLK_F9			= 290,
	LOLK_F10		= 291,
	LOLK_F11		= 292,
	LOLK_F12		= 293,
	LOLK_F13		= 294,
	LOLK_F14		= 295,
	LOLK_F15		= 296,
        /*@}*/

	/** @name Key state modifier keys */
        /*@{*/
	LOLK_NUMLOCK		= 300,
	LOLK_CAPSLOCK		= 301,
	LOLK_SCROLLOCK		= 302,
	LOLK_RSHIFT		= 303,
	LOLK_LSHIFT		= 304,
	LOLK_RCTRL		= 305,
	LOLK_LCTRL		= 306,
	LOLK_RALT		= 307,
	LOLK_LALT		= 308,
	LOLK_RMETA		= 309,
	LOLK_LMETA		= 310,
	LOLK_LSUPER		= 311,		/**< Left "Windows" key */
	LOLK_RSUPER		= 312,		/**< Right "Windows" key */
	LOLK_MODE		= 313,		/**< "Alt Gr" key */
	LOLK_COMPOSE		= 314,		/**< Multi-key compose key */
        /*@}*/

	/** @name Miscellaneous function keys */
        /*@{*/
	LOLK_HELP		= 315,
	LOLK_PRINT		= 316,
	LOLK_SYSREQ		= 317,
	LOLK_BREAK		= 318,
	LOLK_MENU		= 319,
	LOLK_POWER		= 320,		/**< Power Macintosh power key */
	LOLK_EURO		= 321,		/**< Some european keyboards */
	LOLK_UNDO		= 322,		/**< Atari keyboard has Undo */
        /*@}*/

	/* Add any other keys here */

	LOLK_LAST
} LOLKey;

/** Enumeration of valid key mods (possibly OR'd together) */
typedef enum {
	LOLKMOD_NONE  = 0x0000,
	LOLKMOD_LSHIFT= 0x0001,
	LOLKMOD_RSHIFT= 0x0002,
	LOLKMOD_LCTRL = 0x0040,
	LOLKMOD_RCTRL = 0x0080,
	LOLKMOD_LALT  = 0x0100,
	LOLKMOD_RALT  = 0x0200,
	LOLKMOD_LMETA = 0x0400,
	LOLKMOD_RMETA = 0x0800,
	LOLKMOD_NUM   = 0x1000,
	LOLKMOD_CAPS  = 0x2000,
	LOLKMOD_MODE  = 0x4000,
	LOLKMOD_RESERVED = 0x8000
} LOLKeyMod;

#define LOLKMOD_CTRL	(LOLKMOD_LCTRL|LOLKMOD_RCTRL)
#define LOLKMOD_SHIFT	(LOLKMOD_LSHIFT|LOLKMOD_RSHIFT)
#define LOLKMOD_ALT		(LOLKMOD_LALT|LOLKMOD_RALT)
#define LOLKMOD_META	(LOLKMOD_LMETA|LOLKMOD_RMETA)

struct ActionSetting
{
	int						ActionId;
	float					BufferingTime;
	float					BufferedSince;

	ActionSetting(int NewActionId)
	{
		memset(this, 0, sizeof(ActionSetting));
		ActionId = NewActionId;
	}
};

struct ButtonSetting
{
	int						m_raw_button_id;
	int						m_current_status;
	int						m_previous_status;
	Array<ActionSetting>	m_associated_action_list;

	ButtonSetting(int NewRawButtonId)
	{
		memset(this, 0, sizeof(ButtonSetting));
		m_raw_button_id = NewRawButtonId;
	}
	int GetActionSettingIdx(int ActionId)
	{
		for (int i = 0; i < m_associated_action_list.Count(); i++)
			if (m_associated_action_list[i].ActionId == ActionId)
				return i;
		return -1;
	}
};

class InputTracker : public Entity
{

	friend class Input;

public:
	InputTracker();

private:
	Array<ButtonSetting>	m_input_assocation_list;

	int GetButtonSettingIdx(int ButtonId)
	{
		for (int i = 0; i < m_input_assocation_list.Count(); i++)
			if (m_input_assocation_list[i].m_raw_button_id == ButtonId)
				return i;
		return -1;
	}

	void UpdateActionStatus(float seconds);

protected:
	virtual void			TickGame(float seconds)
	{
		UpdateActionStatus(seconds);
	}

	void					LinkActionIdToButtonId(int ActionId, int ButtonId);
	void					UnlinkActionId(int ActionId);
	int						GetActionStatus(int ActionId);
	bool					WasActionJustReleased(int ActionId);
};

class Input
{
private:
	static InputTracker*		m_input_tracker;

	static bool					CheckInputTrackerInit()
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
	static void LinkActionIdToButtonId(int ActionId, int ButtonId);
	static void UnlinkActionId(int ActionId);
	static int GetActionStatus(int ActionId);
	static bool WasActionJustReleased(int ActionId);

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

