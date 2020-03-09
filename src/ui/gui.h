//
//  Lol Engine
//
//  Copyright © 2017—2020 Sam Hocevar <sam@hocevar.net>
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

class gui : public entity
{
public:
    gui(ImFontAtlas *shared_font_atlas);
    ~gui();
    std::string GetName() const override { return "<gui>"; }

    static void init(ImFontAtlas *shared_font_atlas = nullptr);
    static void shutdown();
    static std::string clipboard();
    static void refresh_fonts();

private:
    typedef entity super;

protected:
    virtual bool init_game() override;
    virtual bool init_draw() override;
    virtual void tick_game(float seconds) override;
    virtual void tick_draw(float seconds, Scene &scene) override;
    virtual bool release_game() override;
    virtual bool release_draw() override;

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
    std::vector<ShaderAttrib> m_attribs;
    std::shared_ptr<VertexDeclaration> m_vdecl;
    std::string m_clipboard;

    class primitive : public PrimitiveRenderer
    {
    public:
        primitive() { }
        virtual void Render(Scene& scene, std::shared_ptr<PrimitiveSource> primitive);
    };
};

} /* namespace lol */
