//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//


#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

#include "core.h"

#ifndef LOL_INPUT_V2

namespace lol
{

/*
 * Input implementation class
 */

InputTracker* Input::m_input_tracker = nullptr;

static class InputData
{
    friend class Input;

public:
    InputData()
      : mouse(-1),
        buttons(0),
        nentities(0),
        lastfocus(0)
    {
        m_keystate.Resize(Key::Last);
        memset(&m_keystate[0], 0, m_keystate.Bytes());
    }

private:
    ivec2 mouse;
    uint32_t buttons;

    Array<uint8_t> m_keystate;

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

int ButtonSetting::GetActionSettingIdx(Action a)
{
    for (int i = 0; i < m_associated_action_list.Count(); i++)
        if (ACTION_CMP(m_associated_action_list[i].m_action, a))
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
int InputTracker::GetButtonSettingIdx(Key k)
{
    for (int i = 0; i < m_input_assocation_list.Count(); i++)
        if (m_input_assocation_list[i].m_raw_button == k)
            return i;
    return -1;
}

//-----
int InputTracker::GetCurrentButtonStatus(Key k)
{
    if (k < m_input_status.Count())
        return m_input_status[k];
    return 0;
}

//-----
int InputTracker::GetPreviousButtonStatus(Key k)
{
    if (k < m_input_status.Count())
        return m_input_status[(int)k + (int)Key::Last];
    return 0;
}

//Internal : Updates the action status & timers
void InputTracker::UpdateActionStatus(float seconds)
{
    Array<uint8_t> &keystate = Input::GetKeyboardState();

    //SOOOoooo ugly.
    for (int i = 0; i < Key::Last; ++i)
    {
        m_input_status[i + Key::Last] = m_input_status[i];
        m_input_status[i] = keystate[i];
    }

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
void InputTracker::LinkActionToKey(Action a, Key k)
{
    int ITIdx = GetButtonSettingIdx(k);
    if (ITIdx == -1)
    {
        ITIdx = m_input_assocation_list.Count();
        m_input_assocation_list << ButtonSetting(k);
    }

    ButtonSetting &CurIT = m_input_assocation_list[ITIdx];

    int ASIdx = CurIT.GetActionSettingIdx(a);
    if (ASIdx == -1)
    {
        ASIdx = CurIT.m_associated_action_list.Count();
        CurIT.m_associated_action_list << ActionSetting(a);
    }
}

//Helps unlink a software input Action-Id to an hardware input k-Id.
void InputTracker::UnlinkAction(Action a)
{
    for (int i = 0; i < m_input_assocation_list.Count(); i++)
    {
        ButtonSetting &CurIT = m_input_assocation_list[i];
        int ASIdx = CurIT.GetActionSettingIdx(a);
        if (ASIdx != -1)
            CurIT.m_associated_action_list.Remove(ASIdx);
    }
}

//Returns the current status of a given action
int InputTracker::GetStatus(Action a)
{
    for (int i = 0; i < m_input_assocation_list.Count(); i++)
    {
        ButtonSetting &CurIT = m_input_assocation_list[i];
        int ASIdx = CurIT.GetActionSettingIdx(a);
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
bool InputTracker::WasReleased(Action a)
{
    for (int i = 0; i < m_input_assocation_list.Count(); i++)
    {
        ButtonSetting &CurIT = m_input_assocation_list[i];
        int ASIdx = CurIT.GetActionSettingIdx(a);
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
bool InputTracker::WasPressed(Action a)
{
    for (int i = 0; i < m_input_assocation_list.Count(); i++)
    {
        ButtonSetting &CurIT = m_input_assocation_list[i];
        int ASIdx = CurIT.GetActionSettingIdx(a);
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
int InputTracker::GetStatus(Key k)
{
    return GetCurrentButtonStatus(k);
}

//Returns TRUE if action status went from Active to Inactive this frame
bool InputTracker::WasReleased(Key k)
{
    if (GetPreviousButtonStatus(k) &&
        !GetCurrentButtonStatus(k))
        return true;
    return false;
}

//Returns TRUE if action status went from Inactive to Active this frame
bool InputTracker::WasPressed(Key k)
{
    if (!GetPreviousButtonStatus(k) &&
        GetCurrentButtonStatus(k))
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

    /* Simulate a joystick using the keyboard. */
    int left = GetKeyState(Key::D) - (GetKeyState(Key::A) | GetKeyState(Key::Q));
    int up = (GetKeyState(Key::W) | GetKeyState(Key::Z)) - GetKeyState(Key::S);
    ret.x += left;
    ret.y += up;
    if (left && up)
        ret = ret * sqrtf(0.5f);

    return ret;
}
#endif

ivec2 Input::GetMousePos()
{
    return data->mouse;
}

uint32_t Input::GetMouseButtons()
{
    return data->buttons;
}

Array<uint8_t> &Input::GetKeyboardState()
{
    return data->m_keystate;
}

int Input::GetKeyState(int key)
{
    return data->m_keystate[key];
}

//Helps link a software input Action-Id to an hardware input Button-Id.
void Input::LinkActionToKey(Action a, Key k)
{
    if (CheckInputTrackerInit())
        Input::m_input_tracker->LinkActionToKey(a, k);
}

//Helps unlink a software input Action-Id to an hardware input Button-Id.
void Input::UnlinkAction(Action a)
{
    if (CheckInputTrackerInit())
        Input::m_input_tracker->UnlinkAction(a);
}

//Returns the current status of a given action
int Input::GetStatus(Action a)
{
    if (CheckInputTrackerInit())
        return Input::m_input_tracker->GetStatus(a);
    return 0;
}

//Returns TRUE if action status when from Active to Inactive this frame
bool Input::WasPressed(Action a)
{
    if (CheckInputTrackerInit())
        return Input::m_input_tracker->WasPressed(a);
    return false;
}

//Returns TRUE if action status when from Active to Inactive this frame
bool Input::WasReleased(Action a)
{
    if (CheckInputTrackerInit())
        return Input::m_input_tracker->WasReleased(a);
    return false;
}

//Returns the current status of a given action
int Input::GetStatus(Key k)
{
    if (CheckInputTrackerInit())
        return Input::m_input_tracker->GetStatus(k);
    return 0;
}

//Returns TRUE if action status when from Active to Inactive this frame
bool Input::WasPressed(Key k)
{
    if (CheckInputTrackerInit())
        return Input::m_input_tracker->WasPressed(k);
    return false;
}

//Returns TRUE if action status when from Active to Inactive this frame
bool Input::WasReleased(Key k)
{
    if (CheckInputTrackerInit())
        return Input::m_input_tracker->WasReleased(k);
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

    WorldEntity *top = nullptr;

    /* Find the top “widget” amongst all entities that match the
     * mouse coordinates */
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
                data->entities[n]->m_clicked = 0;
        }
        else
        {
            data->entities[n]->m_mousepos = ivec2(-1);
            /* FIXME */
            data->entities[n]->m_released = 0;
            data->entities[n]->m_pressed = 0;
            data->entities[n]->m_clicked = 0;
        }
    }

    data->lastfocus = top;
}

void Input::SetMouseButton(int index)
{
    uint32_t flag = 1 << index;
    data->buttons |= flag;

    if (data->lastfocus)
    {
        if (!(data->lastfocus->m_pressed & flag))
            data->lastfocus->m_clicked |= flag;
        data->lastfocus->m_pressed |= flag;
        data->lastfocus->m_released &= ~flag;
    }
}

void Input::UnsetMouseButton(int index)
{
    uint32_t flag = 1 << index;
    data->buttons &= ~flag;

    if (data->lastfocus)
    {
        if (!(data->lastfocus->m_pressed & flag))
            data->lastfocus->m_released |= flag;
        data->lastfocus->m_pressed &= ~flag;
        data->lastfocus->m_clicked &= ~flag;
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
        return nullptr;
    Ticker::Ref(data->m_sticks[desired]);
    return data->m_sticks[desired];
}

void Input::UntrackStick(Stick *stick)
{
    Ticker::Unref(stick);
}

} /* namespace lol */

#endif // !LOL_INPUT_V2
