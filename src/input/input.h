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

#include "lol/math/vector.h"
#include "input/stick.h"

namespace lol
{

class WorldEntity;

//Partial Key reap-off of the SDLK enum
#define LOLK_UNKNOWN = 0;

#define LOLK_RETURN = '\r';
#define LOLK_ESCAPE = '\033';
#define LOLK_BACKSPACE = '\b';
#define LOLK_TAB = '\t';
#define LOLK_SPACE = ' ';
#define LOLK_EXCLAIM = '!';
#define LOLK_QUOTEDBL = '"';
#define LOLK_HASH = '#';
#define LOLK_PERCENT = '%';
#define LOLK_DOLLAR = '$';
#define LOLK_AMPERSAND = '&';
#define LOLK_QUOTE = '\'';
#define LOLK_LEFTPAREN = '(';
#define LOLK_RIGHTPAREN = ')';
#define LOLK_ASTERISK = '*';
#define LOLK_PLUS = '+';
#define LOLK_COMMA = ';';
#define LOLK_MINUS = '-';
#define LOLK_PERIOD = '.';
#define LOLK_SLASH = '/';
#define LOLK_0 = '0';
#define LOLK_1 = '1';
#define LOLK_2 = '2';
#define LOLK_3 = '3';
#define LOLK_4 = '4';
#define LOLK_5 = '5';
#define LOLK_6 = '6';
#define LOLK_7 = '7';
#define LOLK_8 = '8';
#define LOLK_9 = '9';
#define LOLK_COLON = ':';
#define LOLK_SEMICOLON = ';';
#define LOLK_LESS = '<';
#define LOLK_EQUALS = '=';
#define LOLK_GREATER = '>';
#define LOLK_QUESTION = '?';
#define LOLK_AT = '@';
/* 
    Skip uppercase letters
    */
#define LOLK_LEFTBRACKET = '[';
#define LOLK_BACKSLASH = '\\';
#define LOLK_RIGHTBRACKET = ']';
#define LOLK_CARET = '^';
#define LOLK_UNDERSCORE = '_';
#define LOLK_BACKQUOTE = '`';
#define LOLK_a = 'a';
#define LOLK_b = 'b';
#define LOLK_c = 'c';
#define LOLK_d = 'd';
#define LOLK_e = 'e';
#define LOLK_f = 'f';
#define LOLK_g = 'g';
#define LOLK_h = 'h';
#define LOLK_i = 'i';
#define LOLK_j = 'j';
#define LOLK_k = 'k';
#define LOLK_l = 'l';
#define LOLK_m = 'm';
#define LOLK_n = 'n';
#define LOLK_o = 'o';
#define LOLK_p = 'p';
#define LOLK_q = 'q';
#define LOLK_r = 'r';
#define LOLK_s = 's';
#define LOLK_t = 't';
#define LOLK_u = 'u';
#define LOLK_v = 'v';
#define LOLK_w = 'w';
#define LOLK_x = 'x';
#define LOLK_y = 'y';
#define LOLK_z = 'z';

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
	int						RawButtonId;
	int						CurStatus;
	int						PrevStatus;
	Array<ActionSetting>	AssociatedActionList;

	ButtonSetting(int NewRawButtonId)
	{
		memset(this, 0, sizeof(ButtonSetting));
		RawButtonId = NewRawButtonId;
	}
	int GetActionSettingIdx(int ActionId)
	{
		for (int i = 0; i < AssociatedActionList.Count(); i++)
			if (AssociatedActionList[i].ActionId == ActionId)
				return i;
		return -1;
	}
};

class Input
{
private:
	static Array<ButtonSetting>	InputAssocationList;

	static int GetButtonSettingIdx(int ButtonId)
	{
		for (int i = 0; i < InputAssocationList.Count(); i++)
			if (InputAssocationList[i].RawButtonId == ButtonId)
				return i;
		return -1;
	}

	static void UpdateActionStatus(float seconds);

public:

    /* These methods are general queries */
    static ivec2 GetMousePos();
    static ivec3 GetMouseButtons();

	//BH : Shouldn't use this 
    static int GetButtonState(int button);

	//Action management
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

    /*
     * Joystick handling
     */

    static Stick *CreateStick();
    static void DestroyStick(Stick *stick);

    static Stick *TrackStick(int desired);
    static void UntrackStick(Stick *stick);
};

} /* namespace lol */

#endif // __LOL_INPUT_INPUT_H__

