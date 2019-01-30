//
//  Lol Engine
//
//  Copyright © 2017—2019 Sam Hocevar <sam@hocevar.net>
//            © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
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
#include <memory>

//
// The Imgui integration
//

#define IM_VEC2_CLASS_EXTRA \
    ImVec2(const lol::vec2 &v) { x = v.x; y = v.y; } \
    ImVec2(const lol::ivec2 &v) : ImVec2(lol::vec2(v)) { } \
    operator lol::vec2() const { return lol::vec2(x, y); } \
    operator lol::ivec2() const { return lol::ivec2(lol::vec2(x, y)); }

#define IM_VEC4_CLASS_EXTRA \
    ImVec4(const lol::vec4 &v) { x = v.x; y = v.y; z = v.z; w = v.w; } \
    ImVec4(const lol::ivec4 &v) : ImVec4(lol::vec4(v)) { } \
    operator lol::vec4() const { return lol::vec4(x, y, z, w); } \
    operator lol::ivec4() const { return lol::ivec4(lol::vec4(x, y, z, w)); }

#include "imgui.h"

#undef IM_VEC2_CLASS_EXTRA
#undef IM_VEC4_CLASS_EXTRA

namespace lol
{

class gui : public Entity
{
public:
    gui(ImFontAtlas *shared_font_atlas);
    ~gui();
    std::string GetName() const { return "<gui>"; }

    static void init(ImFontAtlas *shared_font_atlas = nullptr);
    static void shutdown();
    static std::string clipboard();
    static void refresh_fonts();

private:
    typedef Entity super;

    //ImGuiKeyBase ------------------------------------------------------------
    struct key_base : public StructSafeEnum
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

            A, C, V, X, Y, Z,

            LShift, RShift, LCtrl, RCtrl,

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
    typedef SafeEnum<key_base> key_enum;

    //ImGuiKeyBase ------------------------------------------------------------
    struct axis_base : public StructSafeEnum
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
    typedef SafeEnum<axis_base> axis_enum;

protected:
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, Scene &scene);

    static void static_set_clipboard(void *data, const char* text);
    static const char* static_get_clipboard(void *data);

    static void static_render_draw_lists(ImDrawData* draw_data);
    void render_draw_lists(ImDrawData* draw_data);

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
    std::shared_ptr<Shader> m_shader = nullptr;
    Uniform m_ortho;
    Uniform m_texture;
    array<ShaderAttrib> m_attribs;
    std::shared_ptr<VertexDeclaration> m_vdecl;
    Controller* m_controller = nullptr;
    InputDevice* m_mouse = nullptr;
    InputDevice* m_keyboard = nullptr;
    InputProfile m_profile;
    //std::map<ImGuiKey_, key_enum> m_keys;
    std::string m_clipboard;

    class primitive : public PrimitiveRenderer
    {
    public:
        primitive() { }
        virtual void Render(Scene& scene, std::shared_ptr<PrimitiveSource> primitive);
    };
};

} /* namespace lol */
