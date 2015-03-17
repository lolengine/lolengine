//
//  imGui integration in lolengine
//
//  Copyright © 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include "imgui.h"

namespace lol
{

//LolImGui --------------------------------------------------------------------
class LolImGui : public Entity
{
    typedef Entity super;

    //-------------------------------------------------------------------------
    LolImGui();
    ~LolImGui();
    char const *GetName() { return "<LolImGui>"; }

    //-------------------------------------------------------------------------
    static void Init();
    static void Shutdown();

    //-------------------------------------------------------------------------
    static void SetClipboard(const char* text);
    static const char* GetClipboard();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, Scene &scene);

    static void RenderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count);

    TextureImage* m_font = nullptr;
};

//bool        ImGui_ImplGlfw_Init(GLFWwindow* window, bool install_callbacks);
//void        ImGui_ImplGlfw_Shutdown();
//void        ImGui_ImplGlfw_NewFrame();
//
//// Use if you want to reset your rendering device without losing ImGui state.
//void        ImGui_ImplGlfw_InvalidateDeviceObjects();
//bool        ImGui_ImplGlfw_CreateDeviceObjects();
//
//// GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
//// Provided here if you want to chain callbacks.
//// You can also handle inputs yourself and use those as a reference.
//void        ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
//void        ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
//void        ImGui_ImplGlFw_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
//void        ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c);


} /* namespace lol */
