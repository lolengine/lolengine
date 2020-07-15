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

#include <lol/engine-internal.h>

#include <cassert>
#include <cstdio>
#include <string>

//
// The Imgui integration
//

using namespace lol;

namespace
{
    enum key_enum
    {
        LeftClick = 300,
        RightClick,
        MiddleClick,
        Focus,
    };
}

static gui* g_gui = nullptr;

//-----------------------------------------------------------------------------
gui::gui(ImFontAtlas *shared_font_atlas)
{
    ImGui::CreateContext(shared_font_atlas);

    m_gamegroup = tickable::group::game::gui;
    m_drawgroup = tickable::group::draw::gui;

    // Build shader code -------------------------------------------------------
    ShaderVar out_vertex = ShaderVar::GetShaderOut(ShaderProgram::Vertex);
    ShaderVar out_pixel = ShaderVar::GetShaderOut(ShaderProgram::Pixel);

    ShaderVar pass_texcoord = ShaderVar(ShaderVariable::Varying, ShaderVariableType::Vec2, "texcoord");
    ShaderVar pass_color = ShaderVar(ShaderVariable::Varying, ShaderVariableType::Vec4, "color");

    ShaderVar in_position = ShaderVar(ShaderVariable::Attribute, ShaderVariableType::Vec2, "position");
    ShaderVar in_texcoord = ShaderVar(ShaderVariable::Attribute, ShaderVariableType::Vec2, "texcoord");
    ShaderVar in_color = ShaderVar(ShaderVariable::Attribute, ShaderVariableType::Vec4, "color");

    m_ortho.m_var = ShaderVar(ShaderVariable::Uniform, ShaderVariableType::Mat4, "ortho");
    m_texture.m_var = ShaderVar(ShaderVariable::Uniform, ShaderVariableType::sampler2D, "texture");

    ShaderBlock imgui_vertex("imgui_vertex");
    imgui_vertex
        << out_vertex << m_ortho << in_position
        << pass_texcoord << in_texcoord
        << pass_color << in_color;
    imgui_vertex.SetMainCode(lol::format(
        "%s = .5 * %s * vec4(%s, -1.0, 1.0);\n" "%s = %s;\n" "%s = %s;\n",
        out_vertex.tostring().c_str(),
        m_ortho.tostring().c_str(),
        in_position.tostring().c_str(),
        pass_texcoord.tostring().c_str(),
        in_texcoord.tostring().c_str(),
        pass_color.tostring().c_str(),
        in_color.tostring().c_str()));

    ShaderBlock imgui_pixel("imgui_pixel");
    imgui_pixel << m_texture << pass_texcoord << pass_color << out_pixel;
    imgui_pixel.SetMainCode(lol::format(
        "vec4 col = %s * texture2D(%s, %s);\n" "if (col.a == 0.0) discard;\n" "%s = col;\n",
        pass_color.tostring().c_str(),
        m_texture.tostring().c_str(),
        pass_texcoord.tostring().c_str(),
        out_pixel.tostring().c_str()));

    m_builder << ShaderProgram::Vertex << imgui_vertex
              << ShaderProgram::Pixel << imgui_pixel;
}

gui::~gui()
{
    ImGui::GetIO().Fonts->TexID = nullptr;
    Ticker::Unref(m_font);
    m_font = nullptr;

    ImGui::DestroyContext();
}

void gui::init(ImFontAtlas *shared_font_atlas)
{
    Ticker::Ref(g_gui = new gui(shared_font_atlas));
}

void gui::shutdown()
{
    Ticker::Unref(g_gui);
    g_gui = nullptr;
}

#if 0 // CALLBACKS
void ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c)
{
ImGuiIO& io = ImGui::GetIO();
if (c > 0 && c < 0x10000)
io.AddInputCharacter((unsigned short)c);
}
#endif

std::string gui::clipboard()
{
    return g_gui ? g_gui->m_clipboard : "";
}

void gui::static_set_clipboard(void *data, const char* text)
{
    std::string *clipboard = (std::string *)data;
    *clipboard = text;
}

const char* gui::static_get_clipboard(void *data)
{
    std::string *clipboard = (std::string *)data;
    return clipboard->c_str();
}

void gui::refresh_fonts()
{
    if (g_gui->m_font)
        Ticker::Unref(g_gui->m_font);

    // Build texture
    unsigned char* pixels;
    ivec2 size;
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->GetTexDataAsRGBA32(&pixels, &size.x, &size.y);

    old_image* img = new old_image();
    img->Copy(pixels, size, PixelFormat::RGBA_8);

    Ticker::Ref(g_gui->m_font = new TextureImage("", img));
}

bool gui::init_game()
{
    ImGuiIO& io = ImGui::GetIO();
    //ImFont* font0 = io.Fonts->AddFontDefault();

    // Keyboard mapping; these are the only ones ImGui cares about, the
    // rest is just handled by the application.
    io.KeyMap[ImGuiKey_Tab]         = (int)input::key::SC_Tab;
    io.KeyMap[ImGuiKey_LeftArrow]   = (int)input::key::SC_Left;
    io.KeyMap[ImGuiKey_RightArrow]  = (int)input::key::SC_Right;
    io.KeyMap[ImGuiKey_UpArrow]     = (int)input::key::SC_Up;
    io.KeyMap[ImGuiKey_DownArrow]   = (int)input::key::SC_Down;
    io.KeyMap[ImGuiKey_Home]        = (int)input::key::SC_Home;
    io.KeyMap[ImGuiKey_End]         = (int)input::key::SC_End;
    io.KeyMap[ImGuiKey_Delete]      = (int)input::key::SC_Delete;
    io.KeyMap[ImGuiKey_Backspace]   = (int)input::key::SC_Backspace;
    io.KeyMap[ImGuiKey_Enter]       = (int)input::key::SC_Return;
    io.KeyMap[ImGuiKey_Escape]      = (int)input::key::SC_Escape;
    io.KeyMap[ImGuiKey_A]           = (int)input::key::SC_A;
    io.KeyMap[ImGuiKey_C]           = (int)input::key::SC_C;
    io.KeyMap[ImGuiKey_V]           = (int)input::key::SC_V;
    io.KeyMap[ImGuiKey_X]           = (int)input::key::SC_X;
    io.KeyMap[ImGuiKey_Y]           = (int)input::key::SC_Y;
    io.KeyMap[ImGuiKey_Z]           = (int)input::key::SC_Z;

    io.RenderDrawListsFn = static_render_draw_lists;
    io.SetClipboardTextFn = static_set_clipboard;
    io.GetClipboardTextFn = static_get_clipboard;
    io.ClipboardUserData = &m_clipboard;

    return true;
}

void gui::tick_game(float seconds)
{
    super::tick_game(seconds);

    auto keyboard = input::keyboard();
    auto mouse = input::mouse();

    ImGuiIO& io = ImGui::GetIO();

    // Disable the ini file for now
    io.IniFilename = nullptr;

    // Init Texture
    if (!m_font)
    {
        refresh_fonts();
    }

    // Texture has been created
    if (m_font && m_font->GetTexture())
    {
        io.Fonts->TexID = (void *)m_font->GetTexture();
    }

    // Setup display size (every frame to accommodate for window resizing)
    auto video_size = vec2(Video::GetSize());
    io.DisplaySize = video_size;

    // Setup time step
    io.DeltaTime = seconds;
    io.MouseDrawCursor = true;

    // Update Keyboard
    io.KeyCtrl = keyboard->key(input::key::SC_LCtrl)
              || keyboard->key(input::key::SC_RCtrl);
    io.KeyShift = keyboard->key(input::key::SC_LShift)
               || keyboard->key(input::key::SC_RShift);

    for (input::key k : input::all_keys())
        io.KeysDown[(int)k] = keyboard->key(k);

    // Update text input
    if (io.WantTextInput)
    {
        std::string text = keyboard->text();
        //text.case_change(io.KeyShift);
        for (auto ch : text)
            io.AddInputCharacter(ch);
    }
    keyboard->capture_text(io.WantTextInput);

    // Update mouse
    vec2 cursor(mouse->axis(input::axis::X),
                1.f - mouse->axis(input::axis::Y));

    io.MousePos = cursor * video_size;
    //msg::debug("%.2f/%.2f\n", io.MousePos.x, io.MousePos.y);
    io.MouseWheel = mouse->axis(input::axis::Wheel);

    io.MouseDown[0] = mouse->button(input::button::BTN_Left);
    io.MouseDown[1] = mouse->button(input::button::BTN_Right);
    io.MouseDown[2] = mouse->button(input::button::BTN_Middle);
    // FIXME: handle key_enum::Focus?

    // Start the frame
    ImGui::NewFrame();
}

bool gui::release_game()
{
    ImGui::EndFrame();
    return true;
}

bool gui::init_draw()
{
    // Create shader
    m_shader = Shader::Create(m_builder.GetName(), m_builder.Build());
    assert(m_shader);

    m_ortho.m_uniform = m_shader->GetUniformLocation(m_ortho.m_var.tostring());
    m_texture.m_uniform = m_shader->GetUniformLocation(m_texture.m_var.tostring());

    m_attribs.push_back(m_shader->GetAttribLocation(VertexUsage::Position, 0));
    m_attribs.push_back(m_shader->GetAttribLocation(VertexUsage::TexCoord, 0));
    m_attribs.push_back(m_shader->GetAttribLocation(VertexUsage::Color, 0));

    m_vdecl = std::make_shared<VertexDeclaration>(
        VertexStream<vec2, vec2, u8vec4>(
            VertexUsage::Position,
            VertexUsage::TexCoord,
            VertexUsage::Color));

    return true;
}

void gui::tick_draw(float seconds, Scene &scene)
{
    super::tick_draw(seconds, scene);

    scene.AddPrimitiveRenderer(this, std::make_shared<primitive>());
}

bool gui::release_draw()
{
    m_vdecl.reset();
    return true;
}

void gui::primitive::Render(Scene& scene)
{
    (void)scene;

    ImGui::Render();
    ImGui::EndFrame();
}

//// Data
//static GLFWwindow*  g_Window = NULL;
//static double       g_Time = 0.0f;
//static bool         g_MousePressed[3] = { false, false, false };
//static float        g_MouseWheel = 0.0f;
//static GLuint       g_FontTexture = 0;

//-------------------------------------------------------------------------
// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
//-------------------------------------------------------------------------
void gui::static_render_draw_lists(ImDrawData* draw_data)
{
    g_gui->render_draw_lists(draw_data);
}

void gui::render_draw_lists(ImDrawData* draw_data)
{
    if (draw_data == nullptr)
        return;

    vec2 size = vec2(Video::GetSize());
    float alpha = 1.f;
    mat4 ortho = mat4::ortho(size.x * alpha, size.y * alpha, -1000.f, 1000.f)
        * mat4::lookat(vec3::axis_z, vec3::zero, vec3::axis_y)
        * mat4::scale(vec3::axis_x - vec3::axis_y - vec3::axis_z)
        * mat4::translate(-size.x * .5f * alpha, -size.y * .5f * alpha, 0.f);

    // Do not render without shader
    if (!m_shader)
        return;

    render_context rc(Scene::GetScene(0).get_renderer());
    rc.cull_mode(CullMode::Disabled);
    rc.depth_func(DepthFunc::Disabled);
    rc.scissor_mode(ScissorMode::Enabled);

    m_shader->Bind();

    // Register uniforms
    m_shader->SetUniform(m_ortho, ortho);

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        auto const &command_list = *draw_data->CmdLists[n];
        /*const unsigned char* vtx_buffer = (const unsigned char*)&command_list.VtxBuffer.front();*/

        struct Vertex
        {
            vec2 pos, tex;
            u8vec4 color;
        };

        auto vbo = std::make_shared<VertexBuffer>(command_list.VtxBuffer.Size * sizeof(ImDrawVert));
        vbo->set_data(command_list.VtxBuffer.Data, command_list.VtxBuffer.Size * sizeof(ImDrawVert));

        auto ibo = std::make_shared<IndexBuffer>(command_list.IdxBuffer.Size * sizeof(ImDrawIdx));
        ibo->set_data(command_list.IdxBuffer.Data, command_list.IdxBuffer.Size * sizeof(ImDrawIdx));

        m_vdecl->Bind();
        ibo->Bind();
        m_vdecl->SetStream(vbo, m_attribs[0], m_attribs[1], m_attribs[2]);

        const ImDrawIdx* idx_buffer_offset = 0;
        for (int cmd_i = 0; cmd_i < command_list.CmdBuffer.Size; cmd_i++)
        {
            auto const &command = command_list.CmdBuffer[cmd_i];
            Texture* texture = (Texture*)command.TextureId;
            if (texture)
            {
                texture->Bind();
                m_shader->SetUniform(m_texture, texture->GetTextureUniform(), 0);
            }

            rc.scissor_rect(command.ClipRect);

#ifdef SHOW_IMGUI_DEBUG
            //-----------------------------------------------------------------
            //<Debug render> --------------------------------------------------
            //-----------------------------------------------------------------
            //Doesn't work anymore ......
            static uint32_t idx_buffer_offset_i = 0;
            if (cmd_i == 0)
                idx_buffer_offset_i = 0;

            float mod = -200.f;
            vec3 off = vec3(vec2(-size.x, -size.y), 0.f);
            vec3 pos[4] = {
                (1.f / mod) * (off + vec3(0.f)),
                (1.f / mod) * (off + size.x * vec3::axis_x),
                (1.f / mod) * (off + size.x * vec3::axis_x + size.y * vec3::axis_y),
                (1.f / mod) * (off + size.y * vec3::axis_y)
            };
            for (int i = 0; i < 4; ++i)
                Debug::DrawLine(pos[i], pos[(i + 1) % 4], Color::white);
            ImDrawVert* buf = vert;
            for (uint16_t i = 0; i < command.ElemCount; i += 3)
            {
                uint16_t ib = indices[idx_buffer_offset_i + i];
                vec2 pos[3];
                pos[0] = vec2(buf[ib + 0].pos.x, buf[ib + 0].pos.y);
                pos[1] = vec2(buf[ib + 1].pos.x, buf[ib + 1].pos.y);
                pos[2] = vec2(buf[ib + 2].pos.x, buf[ib + 2].pos.y);
                vec4 col[3];
                col[0] = vec4(Color::FromRGBA32(buf[ib + 0].col).arg, 1.f);
                col[1] = vec4(Color::FromRGBA32(buf[ib + 1].col).arg, 1.f);
                col[2] = vec4(Color::FromRGBA32(buf[ib + 2].col).arg, 1.f);
                Debug::DrawLine((off + vec3(pos[0], 0.f)) / mod, (off + vec3(pos[1], 0.f)) / mod, col[0]);
                Debug::DrawLine((off + vec3(pos[1], 0.f)) / mod, (off + vec3(pos[2], 0.f)) / mod, col[1]);
                Debug::DrawLine((off + vec3(pos[2], 0.f)) / mod, (off + vec3(pos[0], 0.f)) / mod, col[2]);
            }
            idx_buffer_offset_i += command.ElemCount;

            //-----------------------------------------------------------------
            //<\Debug render> -------------------------------------------------
            //-----------------------------------------------------------------
#endif //SHOW_IMGUI_DEBUG
            //Debug::DrawLine(vec2::zero, vec2::axis_x /*, Color::green*/);

            m_vdecl->DrawIndexedElements(MeshPrimitive::Triangles, command.ElemCount, (const short*)idx_buffer_offset);

            idx_buffer_offset += command.ElemCount;
        }

        m_vdecl->Unbind();
        ibo->Unbind();
    }

    m_shader->Unbind();
}
