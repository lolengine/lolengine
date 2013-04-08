//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#if USE_SDL
#   if HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#endif

#include "core.h"
#include "sdlinput.h"

/* We force joystick polling because no events are received when
 * there is no SDL display (eg. on the Raspberry Pi). */
#define SDL_FORCE_POLL_JOYSTICK 1

namespace lol
{

/*
 * SDL Input implementation class
 */

class SdlInputData
{
    friend class SdlInput;

private:
    void Tick(float seconds);

    static ivec2 GetMousePos();
#if USE_SDL
    Array<SDL_Joystick *, Stick *> m_joysticks;
#endif
};

/*
 * Public SdlInput class
 */

SdlInput::SdlInput()
  : m_data(new SdlInputData())
{
#if USE_SDL
    /* Enable Unicode translation of keyboard events */
    SDL_EnableUNICODE(1);

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK);

#   if !EMSCRIPTEN
#       if SDL_FORCE_POLL_JOYSTICK
    SDL_JoystickEventState(SDL_QUERY);
#       else
    SDL_JoystickEventState(SDL_ENABLE);
#       endif

    /* Register all the joysticks we can find, and let the input
     * system decide what it wants to track. */
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        SDL_Joystick *sdlstick = SDL_JoystickOpen(i);

        /* Blacklist some devices:
         *  - HDAPS, it's not a real joystick.
         *  - X360 controllers, Xinput handles them better since
         *    it won't think there is only one trigger axis. */
        char const *name = SDL_JoystickName(i);
        if (strstr(name, "HDAPS")
#       if USE_XINPUT
             || strstr(name, "XBOX 360 For Windows")
#       endif
             || false)
        {
            SDL_JoystickClose(sdlstick);
            continue;
        }

        Stick *stick = Input::CreateStick();
        stick->SetAxisCount(SDL_JoystickNumAxes(sdlstick));
        stick->SetButtonCount(SDL_JoystickNumButtons(sdlstick));

        /* It's possible to remap axes */
        //stick->RemapAxis(4, 2);
        //stick->RemapAxis(2, 4);

        m_data->m_joysticks.Push(sdlstick, stick);
    }
#   endif
#endif

    m_gamegroup = GAMEGROUP_BEFORE;
}

SdlInput::~SdlInput()
{
#if USE_SDL && !EMSCRIPTEN
    /* Unregister all the joysticks we added */
    while (m_data->m_joysticks.Count())
    {
        SDL_JoystickClose(m_data->m_joysticks[0].m1);
        Input::DestroyStick(m_data->m_joysticks[0].m2);
        m_data->m_joysticks.Remove(0);
    }
#endif
    delete m_data;
}

void SdlInput::TickGame(float seconds)
{
    Entity::TickGame(seconds);

#if !_WIN32
    m_data->Tick(seconds);
#endif
}

void SdlInput::TickDraw(float seconds)
{
    Entity::TickDraw(seconds);

#if _WIN32
    m_data->Tick(seconds);
#endif
}

void SdlInputData::Tick(float seconds)
{
#if USE_SDL
    /* Handle mouse input */
    ivec2 mouse = SdlInputData::GetMousePos();;
    Input::SetMousePos(mouse);

    /* Pump all joystick events because no event is coming to us. */
#   if SDL_FORCE_POLL_JOYSTICK && !EMSCRIPTEN
    SDL_JoystickUpdate();
    for (int j = 0; j < m_joysticks.Count(); j++)
    {
        for (int i = 0; i < SDL_JoystickNumButtons(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->SetButton(i, SDL_JoystickGetButton(m_joysticks[j].m1, i));
        for (int i = 0; i < SDL_JoystickNumAxes(m_joysticks[j].m1); i++)
            m_joysticks[j].m2->SetAxis(i, (float)SDL_JoystickGetAxis(m_joysticks[j].m1, i) / 32768.f);
    }
#   endif

    /* Handle keyboard and WM events */
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            Ticker::Shutdown();
            break;
#if 0
        case SDL_KEYDOWN:
            if (event.key.keysym.unicode)
                fprintf(stderr, "%c (0x%04X)\n", event.key.keysym.unicode, event.key.keysym.unicode);
            break;
#endif
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            ivec2 newmouse = SdlInputData::GetMousePos();
            if (newmouse != mouse)
                Input::SetMousePos(mouse = newmouse);
            if (event.type == SDL_MOUSEBUTTONDOWN)
                Input::SetMouseButton(event.button.button - 1);
            else
                Input::UnsetMouseButton(event.button.button - 1);
            break;
        }

#   if !SDL_FORCE_POLL_JOYSTICK
        case SDL_JOYAXISMOTION:
            m_joysticks[event.jaxis.which].m2->SetAxis(event.jaxis.axis, (float)event.jaxis.value / 32768.f);
            break;

        case SDL_JOYBUTTONUP:
        case SDL_JOYBUTTONDOWN:
            m_joysticks[event.jbutton.which].m2->SetButton(event.jbutton.button, event.jbutton.state);
            break;
#   endif
        }
    }

    /* Send the whole keyboard state to the input system */
    Array<uint8_t> &lolstate = Input::GetKeyboardState();

#   if SDL_VERSION_ATLEAST(1,3,0)
    Uint8 *sdlstate = SDL_GetKeyboardState(nullptr);
#   else
    Uint8 *sdlstate = SDL_GetKeyState(nullptr);
#   endif

    lolstate[Key::Unknown] = sdlstate[SDLK_UNKNOWN];
    lolstate[Key::Backspace] = sdlstate[SDLK_BACKSPACE];
    lolstate[Key::Tab] = sdlstate[SDLK_TAB];
    lolstate[Key::Clear] = sdlstate[SDLK_CLEAR];
    lolstate[Key::Return] = sdlstate[SDLK_RETURN];
    lolstate[Key::Pause] = sdlstate[SDLK_PAUSE];
    lolstate[Key::Escape] = sdlstate[SDLK_ESCAPE];
    lolstate[Key::Space] = sdlstate[SDLK_SPACE];
    lolstate[Key::Exclaim] = sdlstate[SDLK_EXCLAIM];
    lolstate[Key::DoubleQuote] = sdlstate[SDLK_QUOTEDBL];
    lolstate[Key::Hash] = sdlstate[SDLK_HASH];
    lolstate[Key::Dollar] = sdlstate[SDLK_DOLLAR];
    lolstate[Key::Ampersand] = sdlstate[SDLK_AMPERSAND];
    lolstate[Key::Quote] = sdlstate[SDLK_QUOTE];
    lolstate[Key::LeftParen] = sdlstate[SDLK_LEFTPAREN];
    lolstate[Key::RightParen] = sdlstate[SDLK_RIGHTPAREN];
    lolstate[Key::Asterisk] = sdlstate[SDLK_ASTERISK];
    lolstate[Key::Plus] = sdlstate[SDLK_PLUS];
    lolstate[Key::Comma] = sdlstate[SDLK_COMMA];
    lolstate[Key::Minus] = sdlstate[SDLK_MINUS];
    lolstate[Key::Period] = sdlstate[SDLK_PERIOD];
    lolstate[Key::Slash] = sdlstate[SDLK_SLASH];
    lolstate[Key::K0] = sdlstate[SDLK_0];
    lolstate[Key::K1] = sdlstate[SDLK_1];
    lolstate[Key::K2] = sdlstate[SDLK_2];
    lolstate[Key::K3] = sdlstate[SDLK_3];
    lolstate[Key::K4] = sdlstate[SDLK_4];
    lolstate[Key::K5] = sdlstate[SDLK_5];
    lolstate[Key::K6] = sdlstate[SDLK_6];
    lolstate[Key::K7] = sdlstate[SDLK_7];
    lolstate[Key::K8] = sdlstate[SDLK_8];
    lolstate[Key::K9] = sdlstate[SDLK_9];
    lolstate[Key::Colon] = sdlstate[SDLK_COLON];
    lolstate[Key::Semicolon] = sdlstate[SDLK_SEMICOLON];
    lolstate[Key::Less] = sdlstate[SDLK_LESS];
    lolstate[Key::Equals] = sdlstate[SDLK_EQUALS];
    lolstate[Key::Greater] = sdlstate[SDLK_GREATER];
    lolstate[Key::Question] = sdlstate[SDLK_QUESTION];
    lolstate[Key::At] = sdlstate[SDLK_AT];

    lolstate[Key::LeftBracket] = sdlstate[SDLK_LEFTBRACKET];
    lolstate[Key::BackSlash] = sdlstate[SDLK_BACKSLASH];
    lolstate[Key::RightBracket] = sdlstate[SDLK_RIGHTBRACKET];
    lolstate[Key::Caret] = sdlstate[SDLK_CARET];
    lolstate[Key::Underscore] = sdlstate[SDLK_UNDERSCORE];
    lolstate[Key::Backquote] = sdlstate[SDLK_BACKQUOTE];
    lolstate[Key::A] = sdlstate[SDLK_a];
    lolstate[Key::B] = sdlstate[SDLK_b];
    lolstate[Key::C] = sdlstate[SDLK_c];
    lolstate[Key::D] = sdlstate[SDLK_d];
    lolstate[Key::E] = sdlstate[SDLK_e];
    lolstate[Key::F] = sdlstate[SDLK_f];
    lolstate[Key::G] = sdlstate[SDLK_g];
    lolstate[Key::H] = sdlstate[SDLK_h];
    lolstate[Key::I] = sdlstate[SDLK_i];
    lolstate[Key::J] = sdlstate[SDLK_j];
    lolstate[Key::K] = sdlstate[SDLK_k];
    lolstate[Key::L] = sdlstate[SDLK_l];
    lolstate[Key::M] = sdlstate[SDLK_m];
    lolstate[Key::N] = sdlstate[SDLK_n];
    lolstate[Key::O] = sdlstate[SDLK_o];
    lolstate[Key::P] = sdlstate[SDLK_p];
    lolstate[Key::Q] = sdlstate[SDLK_q];
    lolstate[Key::R] = sdlstate[SDLK_r];
    lolstate[Key::S] = sdlstate[SDLK_s];
    lolstate[Key::T] = sdlstate[SDLK_t];
    lolstate[Key::U] = sdlstate[SDLK_u];
    lolstate[Key::V] = sdlstate[SDLK_v];
    lolstate[Key::W] = sdlstate[SDLK_w];
    lolstate[Key::X] = sdlstate[SDLK_x];
    lolstate[Key::Y] = sdlstate[SDLK_y];
    lolstate[Key::Z] = sdlstate[SDLK_z];
    lolstate[Key::Delete] = sdlstate[SDLK_DELETE];

#if SDLK_WORLD_0
    lolstate[Key::World0] = sdlstate[SDLK_WORLD_0];
    lolstate[Key::World1] = sdlstate[SDLK_WORLD_1];
    lolstate[Key::World2] = sdlstate[SDLK_WORLD_2];
    lolstate[Key::World3] = sdlstate[SDLK_WORLD_3];
    lolstate[Key::World4] = sdlstate[SDLK_WORLD_4];
    lolstate[Key::World5] = sdlstate[SDLK_WORLD_5];
    lolstate[Key::World6] = sdlstate[SDLK_WORLD_6];
    lolstate[Key::World7] = sdlstate[SDLK_WORLD_7];
    lolstate[Key::World8] = sdlstate[SDLK_WORLD_8];
    lolstate[Key::World9] = sdlstate[SDLK_WORLD_9];
    lolstate[Key::World10] = sdlstate[SDLK_WORLD_10];
    lolstate[Key::World11] = sdlstate[SDLK_WORLD_11];
    lolstate[Key::World12] = sdlstate[SDLK_WORLD_12];
    lolstate[Key::World13] = sdlstate[SDLK_WORLD_13];
    lolstate[Key::World14] = sdlstate[SDLK_WORLD_14];
    lolstate[Key::World15] = sdlstate[SDLK_WORLD_15];
    lolstate[Key::World16] = sdlstate[SDLK_WORLD_16];
    lolstate[Key::World17] = sdlstate[SDLK_WORLD_17];
    lolstate[Key::World18] = sdlstate[SDLK_WORLD_18];
    lolstate[Key::World19] = sdlstate[SDLK_WORLD_19];
    lolstate[Key::World20] = sdlstate[SDLK_WORLD_20];
    lolstate[Key::World21] = sdlstate[SDLK_WORLD_21];
    lolstate[Key::World22] = sdlstate[SDLK_WORLD_22];
    lolstate[Key::World23] = sdlstate[SDLK_WORLD_23];
    lolstate[Key::World24] = sdlstate[SDLK_WORLD_24];
    lolstate[Key::World25] = sdlstate[SDLK_WORLD_25];
    lolstate[Key::World26] = sdlstate[SDLK_WORLD_26];
    lolstate[Key::World27] = sdlstate[SDLK_WORLD_27];
    lolstate[Key::World28] = sdlstate[SDLK_WORLD_28];
    lolstate[Key::World29] = sdlstate[SDLK_WORLD_29];
    lolstate[Key::World30] = sdlstate[SDLK_WORLD_30];
    lolstate[Key::World31] = sdlstate[SDLK_WORLD_31];
    lolstate[Key::World32] = sdlstate[SDLK_WORLD_32];
    lolstate[Key::World33] = sdlstate[SDLK_WORLD_33];
    lolstate[Key::World34] = sdlstate[SDLK_WORLD_34];
    lolstate[Key::World35] = sdlstate[SDLK_WORLD_35];
    lolstate[Key::World36] = sdlstate[SDLK_WORLD_36];
    lolstate[Key::World37] = sdlstate[SDLK_WORLD_37];
    lolstate[Key::World38] = sdlstate[SDLK_WORLD_38];
    lolstate[Key::World39] = sdlstate[SDLK_WORLD_39];
    lolstate[Key::World40] = sdlstate[SDLK_WORLD_40];
    lolstate[Key::World41] = sdlstate[SDLK_WORLD_41];
    lolstate[Key::World42] = sdlstate[SDLK_WORLD_42];
    lolstate[Key::World43] = sdlstate[SDLK_WORLD_43];
    lolstate[Key::World44] = sdlstate[SDLK_WORLD_44];
    lolstate[Key::World45] = sdlstate[SDLK_WORLD_45];
    lolstate[Key::World46] = sdlstate[SDLK_WORLD_46];
    lolstate[Key::World47] = sdlstate[SDLK_WORLD_47];
    lolstate[Key::World48] = sdlstate[SDLK_WORLD_48];
    lolstate[Key::World49] = sdlstate[SDLK_WORLD_49];
    lolstate[Key::World50] = sdlstate[SDLK_WORLD_50];
    lolstate[Key::World51] = sdlstate[SDLK_WORLD_51];
    lolstate[Key::World52] = sdlstate[SDLK_WORLD_52];
    lolstate[Key::World53] = sdlstate[SDLK_WORLD_53];
    lolstate[Key::World54] = sdlstate[SDLK_WORLD_54];
    lolstate[Key::World55] = sdlstate[SDLK_WORLD_55];
    lolstate[Key::World56] = sdlstate[SDLK_WORLD_56];
    lolstate[Key::World57] = sdlstate[SDLK_WORLD_57];
    lolstate[Key::World58] = sdlstate[SDLK_WORLD_58];
    lolstate[Key::World59] = sdlstate[SDLK_WORLD_59];
    lolstate[Key::World60] = sdlstate[SDLK_WORLD_60];
    lolstate[Key::World61] = sdlstate[SDLK_WORLD_61];
    lolstate[Key::World62] = sdlstate[SDLK_WORLD_62];
    lolstate[Key::World63] = sdlstate[SDLK_WORLD_63];
    lolstate[Key::World64] = sdlstate[SDLK_WORLD_64];
    lolstate[Key::World65] = sdlstate[SDLK_WORLD_65];
    lolstate[Key::World66] = sdlstate[SDLK_WORLD_66];
    lolstate[Key::World67] = sdlstate[SDLK_WORLD_67];
    lolstate[Key::World68] = sdlstate[SDLK_WORLD_68];
    lolstate[Key::World69] = sdlstate[SDLK_WORLD_69];
    lolstate[Key::World70] = sdlstate[SDLK_WORLD_70];
    lolstate[Key::World71] = sdlstate[SDLK_WORLD_71];
    lolstate[Key::World72] = sdlstate[SDLK_WORLD_72];
    lolstate[Key::World73] = sdlstate[SDLK_WORLD_73];
    lolstate[Key::World74] = sdlstate[SDLK_WORLD_74];
    lolstate[Key::World75] = sdlstate[SDLK_WORLD_75];
    lolstate[Key::World76] = sdlstate[SDLK_WORLD_76];
    lolstate[Key::World77] = sdlstate[SDLK_WORLD_77];
    lolstate[Key::World78] = sdlstate[SDLK_WORLD_78];
    lolstate[Key::World79] = sdlstate[SDLK_WORLD_79];
    lolstate[Key::World80] = sdlstate[SDLK_WORLD_80];
    lolstate[Key::World81] = sdlstate[SDLK_WORLD_81];
    lolstate[Key::World82] = sdlstate[SDLK_WORLD_82];
    lolstate[Key::World83] = sdlstate[SDLK_WORLD_83];
    lolstate[Key::World84] = sdlstate[SDLK_WORLD_84];
    lolstate[Key::World85] = sdlstate[SDLK_WORLD_85];
    lolstate[Key::World86] = sdlstate[SDLK_WORLD_86];
    lolstate[Key::World87] = sdlstate[SDLK_WORLD_87];
    lolstate[Key::World88] = sdlstate[SDLK_WORLD_88];
    lolstate[Key::World89] = sdlstate[SDLK_WORLD_89];
    lolstate[Key::World90] = sdlstate[SDLK_WORLD_90];
    lolstate[Key::World91] = sdlstate[SDLK_WORLD_91];
    lolstate[Key::World92] = sdlstate[SDLK_WORLD_92];
    lolstate[Key::World93] = sdlstate[SDLK_WORLD_93];
    lolstate[Key::World94] = sdlstate[SDLK_WORLD_94];
    lolstate[Key::World95] = sdlstate[SDLK_WORLD_95];
#endif

    lolstate[Key::KP0] = sdlstate[SDLK_KP0];
    lolstate[Key::KP1] = sdlstate[SDLK_KP1];
    lolstate[Key::KP2] = sdlstate[SDLK_KP2];
    lolstate[Key::KP3] = sdlstate[SDLK_KP3];
    lolstate[Key::KP4] = sdlstate[SDLK_KP4];
    lolstate[Key::KP5] = sdlstate[SDLK_KP5];
    lolstate[Key::KP6] = sdlstate[SDLK_KP6];
    lolstate[Key::KP7] = sdlstate[SDLK_KP7];
    lolstate[Key::KP8] = sdlstate[SDLK_KP8];
    lolstate[Key::KP9] = sdlstate[SDLK_KP9];
    lolstate[Key::KPPeriod] = sdlstate[SDLK_KP_PERIOD];
    lolstate[Key::KPDivide] = sdlstate[SDLK_KP_DIVIDE];
    lolstate[Key::KPMultiply] = sdlstate[SDLK_KP_MULTIPLY];
    lolstate[Key::KPMinus] = sdlstate[SDLK_KP_MINUS];
    lolstate[Key::KPPlus] = sdlstate[SDLK_KP_PLUS];
    lolstate[Key::KPEnter] = sdlstate[SDLK_KP_ENTER];
    lolstate[Key::KPEquals] = sdlstate[SDLK_KP_EQUALS];

    lolstate[Key::Up] = sdlstate[SDLK_UP];
    lolstate[Key::Down] = sdlstate[SDLK_DOWN];
    lolstate[Key::Right] = sdlstate[SDLK_RIGHT];
    lolstate[Key::Left] = sdlstate[SDLK_LEFT];
    lolstate[Key::Insert] = sdlstate[SDLK_INSERT];
    lolstate[Key::Home] = sdlstate[SDLK_HOME];
    lolstate[Key::End] = sdlstate[SDLK_END];
    lolstate[Key::PageUp] = sdlstate[SDLK_PAGEUP];
    lolstate[Key::PageDown] = sdlstate[SDLK_PAGEDOWN];

    lolstate[Key::F1] = sdlstate[SDLK_F1];
    lolstate[Key::F2] = sdlstate[SDLK_F2];
    lolstate[Key::F3] = sdlstate[SDLK_F3];
    lolstate[Key::F4] = sdlstate[SDLK_F4];
    lolstate[Key::F5] = sdlstate[SDLK_F5];
    lolstate[Key::F6] = sdlstate[SDLK_F6];
    lolstate[Key::F7] = sdlstate[SDLK_F7];
    lolstate[Key::F8] = sdlstate[SDLK_F8];
    lolstate[Key::F9] = sdlstate[SDLK_F9];
    lolstate[Key::F10] = sdlstate[SDLK_F10];
    lolstate[Key::F11] = sdlstate[SDLK_F11];
    lolstate[Key::F12] = sdlstate[SDLK_F12];
    lolstate[Key::F13] = sdlstate[SDLK_F13];
    lolstate[Key::F14] = sdlstate[SDLK_F14];
    lolstate[Key::F15] = sdlstate[SDLK_F15];

    lolstate[Key::NumLock] = sdlstate[SDLK_NUMLOCK];
    lolstate[Key::CapsLock] = sdlstate[SDLK_CAPSLOCK];
    lolstate[Key::ScrollLock] = sdlstate[SDLK_SCROLLOCK];
    lolstate[Key::RightShift] = sdlstate[SDLK_RSHIFT];
    lolstate[Key::LeftShift] = sdlstate[SDLK_LSHIFT];
    lolstate[Key::RightCtrl] = sdlstate[SDLK_RCTRL];
    lolstate[Key::LeftCtrl] = sdlstate[SDLK_LCTRL];
    lolstate[Key::RightAlt] = sdlstate[SDLK_RALT];
    lolstate[Key::LeftAlt] = sdlstate[SDLK_LALT];
    lolstate[Key::RightMeta] = sdlstate[SDLK_RMETA];
    lolstate[Key::LeftMeta] = sdlstate[SDLK_LMETA];
    lolstate[Key::LeftSuper] = sdlstate[SDLK_LSUPER];
    lolstate[Key::RightSuper] = sdlstate[SDLK_RSUPER];
    lolstate[Key::Mode] = sdlstate[SDLK_MODE];
    lolstate[Key::Compose] = sdlstate[SDLK_COMPOSE];

    lolstate[Key::Help] = sdlstate[SDLK_HELP];
    lolstate[Key::Print] = sdlstate[SDLK_PRINT];
    lolstate[Key::SysReq] = sdlstate[SDLK_SYSREQ];
    lolstate[Key::Break] = sdlstate[SDLK_BREAK];
    lolstate[Key::Menu] = sdlstate[SDLK_MENU];
    lolstate[Key::Power] = sdlstate[SDLK_POWER];
    lolstate[Key::Euro] = sdlstate[SDLK_EURO];
    lolstate[Key::Undo] = sdlstate[SDLK_UNDO];

    UNUSED(seconds);
#endif
}

ivec2 SdlInputData::GetMousePos()
{
    ivec2 ret(-1, -1);

#if USE_SDL
#   if !EMSCRIPTEN
    if (SDL_GetAppState() & SDL_APPMOUSEFOCUS)
#   endif
    {
        SDL_GetMouseState(&ret.x, &ret.y);
        ret.y = Video::GetSize().y - 1 - ret.y;
    }
#endif
    return ret;
}

} /* namespace lol */

