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

#define ACTION_TYPE std::string
#define ACTION_CMP(A, B) (A.compare(B) == 0)

class WorldEntity;

//FULL Key rip-off of the SDLK enum
struct Key
{
	enum Value
	{
		/** @name ASCII mapped keysyms
			*  The keyboard syms have been cleverly chosen to map to ASCII
			*/
		/*@{*/
		K_UNKNOWN		= 0,
		K_FIRST			= 0,
		K_BACKSPACE		= 8,
		K_TAB			= 9,
		K_CLEAR			= 12,
		K_RETURN		= 13,
		K_PAUSE			= 19,
		K_ESCAPE		= 27,
		K_SPACE			= 32,
		K_EXCLAIM		= 33,
		K_QUOTEDBL		= 34,
		K_HASH			= 35,
		K_DOLLAR		= 36,
		K_AMPERSAND		= 38,
		K_QUOTE			= 39,
		K_LEFTPAREN		= 40,
		K_RIGHTPAREN	= 41,
		K_ASTERISK		= 42,
		K_PLUS			= 43,
		K_COMMA			= 44,
		K_MINUS			= 45,
		K_PERIOD		= 46,
		K_SLASH			= 47,
		K_0				= 48,
		K_1				= 49,
		K_2				= 50,
		K_3				= 51,
		K_4				= 52,
		K_5				= 53,
		K_6				= 54,
		K_7				= 55,
		K_8				= 56,
		K_9				= 57,
		K_COLON			= 58,
		K_SEMICOLON		= 59,
		K_LESS			= 60,
		K_EQUALS		= 61,
		K_GREATER		= 62,
		K_QUESTION		= 63,
		K_AT			= 64,
		/* 
		   Skip uppercase letters
		 */
		K_LEFTBRACKET	= 91,
		K_BACKSLASH		= 92,
		K_RIGHTBRACKET	= 93,
		K_CARET			= 94,
		K_UNDERSCORE	= 95,
		K_BACKQUOTE		= 96,
		K_a				= 97,
		K_b				= 98,
		K_c				= 99,
		K_d				= 100,
		K_e				= 101,
		K_f				= 102,
		K_g				= 103,
		K_h				= 104,
		K_i				= 105,
		K_j				= 106,
		K_k				= 107,
		K_l				= 108,
		K_m				= 109,
		K_n				= 110,
		K_o				= 111,
		K_p				= 112,
		K_q				= 113,
		K_r				= 114,
		K_s				= 115,
		K_t				= 116,
		K_u				= 117,
		K_v				= 118,
		K_w				= 119,
		K_x				= 120,
		K_y				= 121,
		K_z				= 122,
		K_DELETE		= 127,
		/* End of ASCII mapped keysyms */
			/*@}*/

		/** @name International keyboard syms */
			/*@{*/
		K_WORLD_0		= 160,		/* 0xA0 */
		K_WORLD_1		= 161,
		K_WORLD_2		= 162,
		K_WORLD_3		= 163,
		K_WORLD_4		= 164,
		K_WORLD_5		= 165,
		K_WORLD_6		= 166,
		K_WORLD_7		= 167,
		K_WORLD_8		= 168,
		K_WORLD_9		= 169,
		K_WORLD_10		= 170,
		K_WORLD_11		= 171,
		K_WORLD_12		= 172,
		K_WORLD_13		= 173,
		K_WORLD_14		= 174,
		K_WORLD_15		= 175,
		K_WORLD_16		= 176,
		K_WORLD_17		= 177,
		K_WORLD_18		= 178,
		K_WORLD_19		= 179,
		K_WORLD_20		= 180,
		K_WORLD_21		= 181,
		K_WORLD_22		= 182,
		K_WORLD_23		= 183,
		K_WORLD_24		= 184,
		K_WORLD_25		= 185,
		K_WORLD_26		= 186,
		K_WORLD_27		= 187,
		K_WORLD_28		= 188,
		K_WORLD_29		= 189,
		K_WORLD_30		= 190,
		K_WORLD_31		= 191,
		K_WORLD_32		= 192,
		K_WORLD_33		= 193,
		K_WORLD_34		= 194,
		K_WORLD_35		= 195,
		K_WORLD_36		= 196,
		K_WORLD_37		= 197,
		K_WORLD_38		= 198,
		K_WORLD_39		= 199,
		K_WORLD_40		= 200,
		K_WORLD_41		= 201,
		K_WORLD_42		= 202,
		K_WORLD_43		= 203,
		K_WORLD_44		= 204,
		K_WORLD_45		= 205,
		K_WORLD_46		= 206,
		K_WORLD_47		= 207,
		K_WORLD_48		= 208,
		K_WORLD_49		= 209,
		K_WORLD_50		= 210,
		K_WORLD_51		= 211,
		K_WORLD_52		= 212,
		K_WORLD_53		= 213,
		K_WORLD_54		= 214,
		K_WORLD_55		= 215,
		K_WORLD_56		= 216,
		K_WORLD_57		= 217,
		K_WORLD_58		= 218,
		K_WORLD_59		= 219,
		K_WORLD_60		= 220,
		K_WORLD_61		= 221,
		K_WORLD_62		= 222,
		K_WORLD_63		= 223,
		K_WORLD_64		= 224,
		K_WORLD_65		= 225,
		K_WORLD_66		= 226,
		K_WORLD_67		= 227,
		K_WORLD_68		= 228,
		K_WORLD_69		= 229,
		K_WORLD_70		= 230,
		K_WORLD_71		= 231,
		K_WORLD_72		= 232,
		K_WORLD_73		= 233,
		K_WORLD_74		= 234,
		K_WORLD_75		= 235,
		K_WORLD_76		= 236,
		K_WORLD_77		= 237,
		K_WORLD_78		= 238,
		K_WORLD_79		= 239,
		K_WORLD_80		= 240,
		K_WORLD_81		= 241,
		K_WORLD_82		= 242,
		K_WORLD_83		= 243,
		K_WORLD_84		= 244,
		K_WORLD_85		= 245,
		K_WORLD_86		= 246,
		K_WORLD_87		= 247,
		K_WORLD_88		= 248,
		K_WORLD_89		= 249,
		K_WORLD_90		= 250,
		K_WORLD_91		= 251,
		K_WORLD_92		= 252,
		K_WORLD_93		= 253,
		K_WORLD_94		= 254,
		K_WORLD_95		= 255,		/* 0xFF */
			/*@}*/

		/** @name Numeric keypad */
			/*@{*/
		K_KP0			= 256,
		K_KP1			= 257,
		K_KP2			= 258,
		K_KP3			= 259,
		K_KP4			= 260,
		K_KP5			= 261,
		K_KP6			= 262,
		K_KP7			= 263,
		K_KP8			= 264,
		K_KP9			= 265,
		K_KP_PERIOD		= 266,
		K_KP_DIVIDE		= 267,
		K_KP_MULTIPLY	= 268,
		K_KP_MINUS		= 269,
		K_KP_PLUS		= 270,
		K_KP_ENTER		= 271,
		K_KP_EQUALS		= 272,
			/*@}*/

		/** @name Arrows + Home/End pad */
			/*@{*/
		K_UP			= 273,
		K_DOWN			= 274,
		K_RIGHT			= 275,
		K_LEFT			= 276,
		K_INSERT		= 277,
		K_HOME			= 278,
		K_END			= 279,
		K_PAGEUP		= 280,
		K_PAGEDOWN		= 281,
			/*@}*/

		/** @name Function keys */
			/*@{*/
		K_F1			= 282,
		K_F2			= 283,
		K_F3			= 284,
		K_F4			= 285,
		K_F5			= 286,
		K_F6			= 287,
		K_F7			= 288,
		K_F8			= 289,
		K_F9			= 290,
		K_F10			= 291,
		K_F11			= 292,
		K_F12			= 293,
		K_F13			= 294,
		K_F14			= 295,
		K_F15			= 296,
			/*@}*/

		/** @name Key state modifier keys */
			/*@{*/
		K_NUMLOCK		= 300,
		K_CAPSLOCK		= 301,
		K_SCROLLOCK		= 302,
		K_RSHIFT		= 303,
		K_LSHIFT		= 304,
		K_RCTRL			= 305,
		K_LCTRL			= 306,
		K_RALT			= 307,
		K_LALT			= 308,
		K_RMETA			= 309,
		K_LMETA			= 310,
		K_LSUPER		= 311,		/**< Left "Windows" key */
		K_RSUPER		= 312,		/**< Right "Windows" key */
		K_MODE			= 313,		/**< "Alt Gr" key */
		K_COMPOSE		= 314,		/**< Multi-key compose key */
			/*@}*/

		/** @name Miscellaneous function keys */
			/*@{*/
		K_HELP			= 315,
		K_PRINT			= 316,
		K_SYSREQ		= 317,
		K_BREAK			= 318,
		K_MENU			= 319,
		K_POWER			= 320,		/**< Power Macintosh power key */
		K_EURO			= 321,		/**< Some european keyboards */
		K_UNDO			= 322,		/**< Atari keyboard has Undo */
			/*@}*/

		/* Add any other keys here */

		K_LAST

	}
	m_value;

	//BH : Removed KMod from main enum, because I don't have any idea about handling them correctly for now.
	/*
		//Enumeration of valid key mods (possibly OR'd together)
		KM_NONE  		= 0x0000,
		KM_LSHIFT		= 0x0001,
		KM_RSHIFT		= 0x0002,
		KM_LCTRL 		= 0x0040,
		KM_RCTRL 		= 0x0080,
		KM_LALT  		= 0x0100,
		KM_RALT  		= 0x0200,
		KM_LMETA 		= 0x0400,
		KM_RMETA 		= 0x0800,
		KM_NUM   		= 0x1000,
		KM_CAPS  		= 0x2000,
		KM_MODE  		= 0x4000,

		KM_RESERVED		= 0x8000,

		//Left/Right independent key mods definition
		KM_CTRL			= (KM_LCTRL|KM_RCTRL),
		KM_SHIFT		= (KM_LSHIFT|KM_RSHIFT),
		KM_ALT			= (KM_LALT|KM_RALT),
		KM_META			= (KM_LMETA|KM_RMETA),
	*/

	inline Key(Value v) { m_value = v; }
	inline operator Value() { return m_value; }
	inline bool operator==(const Key& CompareButton) { return m_value == CompareButton.m_value; }
};

struct ActionSetting
{
	ACTION_TYPE				m_action;
	float					m_buffering_time;
	float					m_buffered_since;

	ActionSetting(ACTION_TYPE NewAction) :
		m_action(NewAction),
		m_buffering_time(.0f),
		m_buffered_since(.0f)
	{ }
};

struct ButtonSetting
{
	Key						m_raw_button;
	Array<ActionSetting>	m_associated_action_list;

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
	Array<Uint8>			m_input_status;
	Array<ButtonSetting>	m_input_assocation_list;

	int						GetButtonSettingIdx(struct Key Button);
	int						GetCurrentButtonStatus(struct Key Button);
	int						GetPreviousButtonStatus(struct Key Button);
	void					UpdateActionStatus(float seconds);

protected:
	virtual void			TickGame(float seconds)
	{
		UpdateActionStatus(seconds);
	}

	void					LinkActionToKey(ACTION_TYPE Action, struct Key Button);
	void					UnlinkAction(ACTION_TYPE Action);
	int						GetStatus(ACTION_TYPE Action);
	bool					WasPressed(ACTION_TYPE Action);
	bool					WasReleased(ACTION_TYPE Action);

	//You should probably use the Action System
	int						GetStatus(Key Button);
	bool					WasPressed(Key Button);
	bool					WasReleased(Key Button);
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
	static void LinkActionToKey(ACTION_TYPE Action, struct Key Button);
	static void UnlinkAction(ACTION_TYPE Action);
	static int GetStatus(ACTION_TYPE Action);
	static bool	WasPressed(ACTION_TYPE Action);
	static bool WasReleased(ACTION_TYPE Action);

	/* Raw Button management. You should use actions. */
	static int GetStatus(Key Button);
	static bool	WasPressed(Key Button);
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

