//
//  Lol Engine
//
//  Copyright © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2017—2018 Sam Hocevar <sam@hocevar.net>
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

//
// The Imgui integration
//

#define IM_VEC2_CLASS_EXTRA ImVec2(const lol::vec2 &v) { x = v.x; y = v.y; } \
                            ImVec2(const lol::ivec2 &v) : ImVec2(lol::vec2(v)) { } \
                            operator lol::vec2() const { return lol::vec2(x, y); } \
                            operator lol::ivec2() const { return lol::ivec2(lol::vec2(x, y)); }

#define IM_VEC4_CLASS_EXTRA ImVec4(const lol::vec4 &v) { x = v.x; y = v.y; z = v.z; w = v.w; } \
                            ImVec4(const lol::ivec4 &v) : ImVec4(lol::vec4(v)) { } \
                            operator lol::vec4() const { return lol::vec4(x, y, z, w); } \
                            operator lol::ivec4() const { return lol::ivec4(lol::vec4(x, y, z, w)); }

#include "imgui.h"

#undef IM_VEC2_CLASS_EXTRA
#undef IM_VEC4_CLASS_EXTRA

//LolImGui ----------------------------------------------------------------------------------------
namespace lol
{

class LolImGui : public Entity
{
    typedef Entity super;

    //ImGuiKeyBase ------------------------------------------------------------
    struct LolImGuiKeyBase : public StructSafeEnum
    {
        enum Type
        {
            KEY_START,

            Tab = KEY_START,
            LeftArrow,
            RightArrow,
            UpArrow,
            DownArrow,
            Home,
            End,
            Delete,
            Backspace,
            Enter,
            Escape,

            A,
            C,
            V,
            X,
            Y,
            Z,

            LShift,
            RShift,
            LCtrl,
            RCtrl,

            KEY_END,

            MOUSE_KEY_START = KEY_END,

            LeftClick = MOUSE_KEY_START,
            RightClick,
            MiddleClick,
            Focus,

            MOUSE_KEY_END,

            MAX = MOUSE_KEY_END,
        };
    protected:
        virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
        {
            enum_map[Tab] = g_name_key_Tab;
            enum_map[LeftArrow] = g_name_key_Left;
            enum_map[RightArrow] = g_name_key_Right;
            enum_map[UpArrow] = g_name_key_Up;
            enum_map[DownArrow] = g_name_key_Down;
            enum_map[Home] = g_name_key_Home;
            enum_map[End] = g_name_key_End;
            enum_map[Delete] = g_name_key_Delete;
            enum_map[Backspace] = g_name_key_Backspace;
            enum_map[Enter] = g_name_key_Return;
            enum_map[Escape] = g_name_key_Escape;

            enum_map[A] = g_name_key_A;
            enum_map[C] = g_name_key_C;
            enum_map[V] = g_name_key_V;
            enum_map[X] = g_name_key_X;
            enum_map[Y] = g_name_key_Y;
            enum_map[Z] = g_name_key_Z;

            enum_map[LShift] = g_name_key_LShift;
            enum_map[RShift] = g_name_key_RShift;
            enum_map[LCtrl] = g_name_key_LCtrl;
            enum_map[RCtrl] = g_name_key_RCtrl;

            enum_map[LeftClick] = g_name_mouse_key_left;
            enum_map[RightClick] = g_name_mouse_key_right;
            enum_map[MiddleClick] = g_name_mouse_key_middle;
            enum_map[Focus] = g_name_mouse_key_in_screen;

            return true;
        }
    };
    typedef SafeEnum<LolImGuiKeyBase> LolImGuiKey;

    //ImGuiKeyBase ------------------------------------------------------------
    struct LolImGuiAxisBase : public StructSafeEnum
    {
        enum Type
        {
            MOUSE_AXIS_START = 0,

            Scroll = MOUSE_AXIS_START,

            MOUSE_AXIS_END,

            MAX = MOUSE_AXIS_END,
        };
    protected:
        virtual bool BuildEnumMap(std::map<int64_t, std::string>& enum_map)
        {
            enum_map[Scroll] = g_name_mouse_axis_scroll;

            return true;
        }
    };
    typedef SafeEnum<LolImGuiAxisBase> LolImGuiAxis;

public:
    //-------------------------------------------------------------------------
    LolImGui(ImFontAtlas *shared_font_atlas);
    ~LolImGui();
    std::string GetName() const { return "<LolImGui>"; }

    //-------------------------------------------------------------------------
    static void Init(ImFontAtlas *shared_font_atlas = nullptr);
    static void Shutdown();

    //-------------------------------------------------------------------------
    static std::string GetClipboard();

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

    static void SetClipboardCallback(void *data, const char* text);
    static const char* GetClipboardCallback(void *data);

    static void RenderDrawLists(ImDrawData* draw_data);
    void RenderDrawListsMethod(ImDrawData* draw_data);

    struct Uniform
    {
        Uniform() { }
        Uniform(ShaderVar var) { m_var = var; }
        std::string tostring() const { return m_var.tostring(); }
        operator ShaderVar() const { return m_var; }
        operator ShaderUniform() const { return m_uniform; }
        //--
        ShaderVar m_var;
        ShaderUniform m_uniform;
    };
    //-------------------------------------------------------------------------
    TextureImage* m_font = nullptr;
    ShaderBuilder m_builder = ShaderBuilder("imgui_shader", "120");
    Shader* m_shader = nullptr;
    Uniform m_ortho;
    Uniform m_texture;
    array<ShaderAttrib> m_attribs;
    VertexDeclaration* m_vdecl = nullptr;
    IndexBuffer* m_ibuff = nullptr;
    Controller* m_controller = nullptr;
    InputDevice* m_mouse = nullptr;
    InputDevice* m_keyboard = nullptr;
    InputProfile m_profile;
    //std::map<ImGuiKey_, LolImGuiKey> m_keys;
    std::string m_clipboard;
};

//-----------------------------------------------------------------------------
class PrimitiveLolImGui : public PrimitiveRenderer
{
public:
    PrimitiveLolImGui() { }
    virtual void Render(Scene& scene, PrimitiveSource* primitive);
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
