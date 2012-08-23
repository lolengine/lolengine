//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

#if defined USE_SDL
#   if defined HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#endif

#include "core.h"

namespace lol
{

/*
 * Input implementation class
 */

InputTracker*			Input::m_input_tracker = NULL;

static class InputData
{
    friend class Input;

public:
    InputData()
      : mouse(-1),
        buttons(0),
        nentities(0),
        lastfocus(0)
    { }

private:
    ivec2 mouse;
    ivec3 buttons;

    static int const MAX_ENTITIES = 100;
    WorldEntity *entities[MAX_ENTITIES];
    int nentities;
    WorldEntity *lastfocus;

    Array<Stick *> m_sticks;
}
inputdata;

static InputData * const data = &inputdata;

/*
 * ButtonSetting class
 */

int ButtonSetting::GetActionSettingIdx(ACTION_TYPE SearchAction)
{
	for (int i = 0; i < m_associated_action_list.Count(); i++)
		if (ACTION_CMP(m_associated_action_list[i].m_action, SearchAction))
			return i;
	return -1;
}

/*
 * InputTracker class
 */

InputTracker::InputTracker()
{
	m_gamegroup = GAMEGROUP_BEFORE;

	for (int i = 0; i < Key::Last * 2; ++i)
		m_input_status << 0;

	Ticker::Ref(this);
}

//Internal
int InputTracker::GetButtonSettingIdx(Key Button)
{
	for (int i = 0; i < m_input_assocation_list.Count(); i++)
		if (m_input_assocation_list[i].m_raw_button == Button)
			return i;
	return -1;
}

//-----
int InputTracker::GetCurrentButtonStatus(Key Button)
{
	if (Button < m_input_status.Count())
		return m_input_status[Button];
	return 0;
}

//-----
int InputTracker::GetPreviousButtonStatus(Key Button)
{
	if (Button < m_input_status.Count())
		return m_input_status[Button + Key::Last];
	return 0;
}

//Internal : Updates the action status & timers
void InputTracker::UpdateActionStatus(float seconds)
{
#if defined USE_SDL
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION >= 3
	Uint8 *keystate = SDL_GetKeyboardState(NULL);
#else
	Uint8 *keystate = SDL_GetKeyState(NULL);
#endif
	//SOOOoooo ugly.
	for (int i = 0; i < Key::Last; ++i)
	{
		m_input_status[i + Key::Last] = m_input_status[i];
		m_input_status[i] = keystate[i];
	}
#endif

	for (int i = 0; i < m_input_assocation_list.Count(); i++)
	{
		ButtonSetting &CurIT = m_input_assocation_list[i];

		for (int j = 0; j < CurIT.m_associated_action_list.Count(); j++)
		{
			ActionSetting &CurAS = CurIT.m_associated_action_list[j];

			if (CurAS.m_buffered_since <= CurAS.m_buffering_time)
				CurAS.m_buffered_since += seconds;

			if (GetCurrentButtonStatus(CurIT.m_raw_button) &&
				CurAS.m_buffering_time >= .0f)
				CurAS.m_buffered_since = .0f;
		}
	}
}

//Helps link a software input Action-Id to an hardware input Button-Id.
void InputTracker::LinkActionToKey(ACTION_TYPE Action, Key Button)
{
	int ITIdx = GetButtonSettingIdx(Button);
	if (ITIdx == -1)
	{
		ITIdx = m_input_assocation_list.Count();
		m_input_assocation_list << ButtonSetting(Button);
	}

	ButtonSetting &CurIT = m_input_assocation_list[ITIdx];

	int ASIdx = CurIT.GetActionSettingIdx(Action);
	if (ASIdx == -1)
	{
		ASIdx = CurIT.m_associated_action_list.Count();
		CurIT.m_associated_action_list << ActionSetting(Action);
	}
}

//Helps unlink a software input Action-Id to an hardware input Button-Id.
void InputTracker::UnlinkAction(ACTION_TYPE Action)
{
	for (int i = 0; i < m_input_assocation_list.Count(); i++)
	{
		ButtonSetting &CurIT = m_input_assocation_list[i];
		int ASIdx = CurIT.GetActionSettingIdx(Action);
		if (ASIdx != -1)
			CurIT.m_associated_action_list.Remove(ASIdx);
	}
}

//Returns the current status of a given action
int InputTracker::GetStatus(ACTION_TYPE Action)
{
	for (int i = 0; i < m_input_assocation_list.Count(); i++)
	{
		ButtonSetting &CurIT = m_input_assocation_list[i];
		int ASIdx = CurIT.GetActionSettingIdx(Action);
		if (ASIdx != -1)
		{
			ActionSetting &CurAS = CurIT.m_associated_action_list[ASIdx];

			if (CurAS.m_buffering_time >= .0f && CurAS.m_buffered_since <= CurAS.m_buffering_time)
				return 1;
			return 0;
		}
	}
	return 0;
}

//Returns TRUE if action status went from Active to Inactive this frame
bool InputTracker::WasReleased(ACTION_TYPE Action)
{
	for (int i = 0; i < m_input_assocation_list.Count(); i++)
	{
		ButtonSetting &CurIT = m_input_assocation_list[i];
		int ASIdx = CurIT.GetActionSettingIdx(Action);
		if (ASIdx != -1)
		{
			
			if (GetPreviousButtonStatus(CurIT.m_raw_button) &&
				!GetCurrentButtonStatus(CurIT.m_raw_button))
				return true;
			return false;
		}
	}
	return false;
}

//Returns TRUE if action status went from Inactive to Active this frame
bool InputTracker::WasPressed(ACTION_TYPE Action)
{
	for (int i = 0; i < m_input_assocation_list.Count(); i++)
	{
		ButtonSetting &CurIT = m_input_assocation_list[i];
		int ASIdx = CurIT.GetActionSettingIdx(Action);
		if (ASIdx != -1)
		{
			if (!GetPreviousButtonStatus(CurIT.m_raw_button) &&
				GetCurrentButtonStatus(CurIT.m_raw_button))
				return true;
			return false;
		}
	}
	return false;
}

//Returns the current status of a given action
int InputTracker::GetStatus(Key Button)
{
	return GetCurrentButtonStatus(Button);
}

//Returns TRUE if action status went from Active to Inactive this frame
bool InputTracker::WasReleased(Key Button)
{
	if (GetPreviousButtonStatus(Button) &&
		!GetCurrentButtonStatus(Button))
		return true;
	return false;
}

//Returns TRUE if action status went from Inactive to Active this frame
bool InputTracker::WasPressed(Key Button)
{
	if (!GetPreviousButtonStatus(Button) &&
		GetCurrentButtonStatus(Button))
		return true;
	return false;
}

/*
 * Public Input class
 */

#if 0
vec2 Input::GetAxis(int axis)
{
    vec2 ret;

#if defined USE_SDL
    /* Simulate a joystick using the keyboard. This SDL call is free. */
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION >= 3
    Uint8 *keystate = SDL_GetKeyboardState(NULL);
#else
    Uint8 *keystate = SDL_GetKeyState(NULL);
#endif
    int left = keystate[SDLK_d] - (keystate[SDLK_a] | keystate[SDLK_q]);
    int up = (keystate[SDLK_w] | keystate[SDLK_z]) - keystate[SDLK_s] ;
    ret.x += left;
    ret.y += up;
    if (left && up)
        ret = ret * sqrtf(0.5f);
#else
    ret = vec2(0, 0);
#endif

    return ret;
}
#endif

ivec2 Input::GetMousePos()
{
    return data->mouse;
}

ivec3 Input::GetMouseButtons()
{
    return data->buttons;
}

//BH : Added this, is a v0.1 Alpha version.
int Input::GetButtonState(int button)
{
#if defined USE_SDL
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION >= 3
    Uint8 *keystate = SDL_GetKeyboardState(NULL);
#else
    Uint8 *keystate = SDL_GetKeyState(NULL);
#endif
    return keystate[button];
#else
    return 0;
#endif
}

//Helps link a software input Action-Id to an hardware input Button-Id.
void Input::LinkActionToKey(ACTION_TYPE Action, struct Key Button)
{
	if (CheckInputTrackerInit())
		Input::m_input_tracker->LinkActionToKey(Action, Button);
}

//Helps unlink a software input Action-Id to an hardware input Button-Id.
void Input::UnlinkAction(ACTION_TYPE Action)
{
	if (CheckInputTrackerInit())
		Input::m_input_tracker->UnlinkAction(Action);
}

//Returns the current status of a given action
int Input::GetStatus(ACTION_TYPE Action)
{
	if (CheckInputTrackerInit())
		return Input::m_input_tracker->GetStatus(Action);
	return 0;
}

//Returns TRUE if action status when from Active to Inactive this frame
bool Input::WasPressed(ACTION_TYPE Action)
{
	if (CheckInputTrackerInit())
		return Input::m_input_tracker->WasPressed(Action);
	return false;
}

//Returns TRUE if action status when from Active to Inactive this frame
bool Input::WasReleased(ACTION_TYPE Action)
{
	if (CheckInputTrackerInit())
		return Input::m_input_tracker->WasReleased(Action);
	return false;
}

//Returns the current status of a given action
int Input::GetStatus(Key Button)
{
	if (CheckInputTrackerInit())
		return Input::m_input_tracker->GetStatus(Button);
	return 0;
}

//Returns TRUE if action status when from Active to Inactive this frame
bool Input::WasPressed(Key Button)
{
	if (CheckInputTrackerInit())
		return Input::m_input_tracker->WasPressed(Button);
	return false;
}

//Returns TRUE if action status when from Active to Inactive this frame
bool Input::WasReleased(Key Button)
{
	if (CheckInputTrackerInit())
		return Input::m_input_tracker->WasReleased(Button);
	return false;
}

//--
void Input::TrackMouse(WorldEntity *e)
{
    if (data->nentities >= InputData::MAX_ENTITIES)
        return;
    data->entities[data->nentities] = e;
    data->nentities++;
}

void Input::UntrackMouse(WorldEntity *e)
{
    for (int n = 0; n < data->nentities; n++)
    {
        if (data->entities[n] != e)
            continue;

        data->entities[n] = data->entities[data->nentities - 1];
        data->nentities--;
        n--;
    }
}

void Input::SetMousePos(ivec2 coord)
{
    data->mouse = coord;

    WorldEntity *top = NULL;

    for (int n = 0; n < data->nentities; n++)
    {
        if (coord.x < data->entities[n]->m_bbox[0].x
             || coord.x >= data->entities[n]->m_bbox[1].x
             || coord.y < data->entities[n]->m_bbox[0].y
             || coord.y >= data->entities[n]->m_bbox[1].y)
            continue;

        if (!top || top->m_bbox[1].z < data->entities[n]->m_bbox[1].z)
            top = data->entities[n];
    }

    for (int n = 0; n < data->nentities; n++)
    {
        if (data->entities[n] == top)
        {
            data->entities[n]->m_mousepos = coord - (ivec2)top->m_bbox[0].xy;
            if (top != data->lastfocus)
                data->entities[n]->m_pressed = data->buttons;
            else
                data->entities[n]->m_clicked = ivec3(0);
        }
        else
        {
            data->entities[n]->m_mousepos = ivec2(-1);
            /* FIXME */
            data->entities[n]->m_released = ivec3(0);
            data->entities[n]->m_pressed = ivec3(0);
            data->entities[n]->m_clicked = ivec3(0);
        }
    }

    data->lastfocus = top;
}

void Input::SetMouseButton(int index)
{
    data->buttons[index] = 1;

    if (data->lastfocus)
    {
        if (!data->lastfocus->m_pressed[index])
            data->lastfocus->m_clicked[index] = 1;
        data->lastfocus->m_pressed[index] = 1;
        data->lastfocus->m_released[index] = 0;
    }
}

void Input::UnsetMouseButton(int index)
{
    data->buttons[index] = 0;

    if (data->lastfocus)
    {
        if (data->lastfocus->m_pressed[index])
            data->lastfocus->m_released[index] = 1;
        data->lastfocus->m_pressed[index] = 0;
        data->lastfocus->m_clicked[index] = 0;
    }
}

Stick *Input::CreateStick()
{
    Stick *stick = new Stick();
    Ticker::Ref(stick);
    data->m_sticks.Push(stick);
    return stick;
}

void Input::DestroyStick(Stick *stick)
{
    for (int i = 0; i < data->m_sticks.Count(); i++)
        if (data->m_sticks[i] == stick)
            data->m_sticks.Remove(i);
    Ticker::Unref(stick);
}

Stick *Input::TrackStick(int desired)
{
    /* FIXME: add the possibility to choose amongst sticks */
    if (desired >= data->m_sticks.Count())
        return NULL;
    Ticker::Ref(data->m_sticks[desired]);
    return data->m_sticks[desired];
}

void Input::UntrackStick(Stick *stick)
{
    Ticker::Unref(stick);
}

} /* namespace lol */

