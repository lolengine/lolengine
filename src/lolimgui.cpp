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

#include <lol/engine-internal.h>

#include "imgui.cpp"

#include <cstdio>

using namespace lol;

//-----------------------------------------------------------------------------
LolImGui::LolImGui()
{
    m_gamegroup = GAMEGROUP_IMGUI;
    m_drawgroup = DRAWGROUP_IMGUI;

    //Build shader code -------------------------------------------------------
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
    imgui_vertex.SetMainCode(String() +
        Line(out_vertex + " = .5 *" + m_ortho + " * vec4(" + in_position + ", -1.0, 1.0);")
        + Line(pass_texcoord + " = " + in_texcoord + ";")
        + Line(pass_color + " = " + in_color + ";")
        );

    ShaderBlock imgui_pixel("imgui_pixel");
    imgui_pixel << m_texture << pass_texcoord << pass_color << out_pixel;
    imgui_pixel.SetMainCode(String() +
        Line(String()
        + "vec4 col = " + pass_color + " * texture2D(" + m_texture + ", " + pass_texcoord + ");")
        + Line("if (col.a == 0.0) discard; ")
        + Line(out_pixel + " = col;")
        );

    m_builder
        << ShaderProgram::Vertex << imgui_vertex
        << ShaderProgram::Pixel << imgui_pixel;

    //Input Setup -------------------------------------------------------------
    for (int i = LolImGuiKey::KEY_START; i < LolImGuiKey::KEY_END; ++i)
        m_profile << InputProfile::Keyboard(i, LolImGuiKey(i).ToString());
    for (int i = LolImGuiKey::MOUSE_KEY_START; i < LolImGuiKey::MOUSE_KEY_END; ++i)
        m_profile << InputProfile::MouseKey(i, LolImGuiKey(i).ToString());

    for (int i = LolImGuiAxis::MOUSE_AXIS_START; i < LolImGuiAxis::MOUSE_AXIS_END; ++i)
        m_profile << InputProfile::MouseAxis(i, LolImGuiAxis(i).ToString());

    Ticker::Ref(m_controller = new Controller("ImGui_Controller"));
    m_controller->Init(m_profile);
    //InputDevice::CaptureMouse(true);
    m_mouse = InputDevice::GetMouse();
    m_keyboard = InputDevice::GetKeyboard();

    //m_controller->Get
    //#   define KB InputProfile::Keyboard
//    m_profile
//        << InputProfile::Keyboard(idx, g_name_key_Left);
//#   undef KB

}
LolImGui::~LolImGui()
{
    ImGui::GetIO().Fonts->TexID = nullptr;
    Ticker::Unref(m_font);
    m_font = nullptr;

    Shader::Destroy(m_shader);
    delete m_vdecl;
}

//-------------------------------------------------------------------------
LolImGui* g_lolimgui = nullptr;
void LolImGui::Init()
{
    Ticker::Ref(g_lolimgui = new LolImGui());

    ImGuiIO& io = ImGui::GetIO();
    //ImFont* font0 = io.Fonts->AddFontDefault();

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_Tab]         = LolImGuiKey::Tab;
    io.KeyMap[ImGuiKey_LeftArrow]   = LolImGuiKey::LeftArrow;
    io.KeyMap[ImGuiKey_RightArrow]  = LolImGuiKey::RightArrow;
    io.KeyMap[ImGuiKey_UpArrow]     = LolImGuiKey::UpArrow;
    io.KeyMap[ImGuiKey_DownArrow]   = LolImGuiKey::DownArrow;
    io.KeyMap[ImGuiKey_Home]        = LolImGuiKey::Home;
    io.KeyMap[ImGuiKey_End]         = LolImGuiKey::End;
    io.KeyMap[ImGuiKey_Delete]      = LolImGuiKey::Delete;
    io.KeyMap[ImGuiKey_Backspace]   = LolImGuiKey::Backspace;
    io.KeyMap[ImGuiKey_Enter]       = LolImGuiKey::Enter;
    io.KeyMap[ImGuiKey_Escape]      = LolImGuiKey::Escape;
    io.KeyMap[ImGuiKey_A]           = LolImGuiKey::A;
    io.KeyMap[ImGuiKey_C]           = LolImGuiKey::C;
    io.KeyMap[ImGuiKey_V]           = LolImGuiKey::V;
    io.KeyMap[ImGuiKey_X]           = LolImGuiKey::X;
    io.KeyMap[ImGuiKey_Y]           = LolImGuiKey::Y;
    io.KeyMap[ImGuiKey_Z]           = LolImGuiKey::Z;

    //Func pointer
    io.RenderDrawListsFn = LolImGui::RenderDrawLists;
    io.SetClipboardTextFn = LolImGui::SetClipboard;
    io.GetClipboardTextFn = LolImGui::GetClipboard;
}

/* CALLBACKS
void ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c)
{
ImGuiIO& io = ImGui::GetIO();
if (c > 0 && c < 0x10000)
io.AddInputCharacter((unsigned short)c);
}

*/

void LolImGui::Shutdown()
{
    Ticker::Unref(g_lolimgui);

    ImGui::Shutdown();
}

//-------------------------------------------------------------------------
static String g_clipboard;
void LolImGui::SetClipboard(const char* text)
{
    g_clipboard = text;
}
const char* LolImGui::GetClipboard()
{
    return g_clipboard.C();
}

//-------------------------------------------------------------------------
void LolImGui::TickGame(float seconds)
{
    super::TickGame(seconds);

    ImGuiIO& io = ImGui::GetIO();

    //Init Texture
    if (!m_font)
    {
        // Build texture
        unsigned char* pixels;
        ivec2 size;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &size.x, &size.y);

        Image* image = new Image();
        image->Copy(pixels, size, PixelFormat::RGBA_8);

        Ticker::Ref(m_font = new TextureImage("", image));
    }
    //Texture has been created
    if (m_font && m_font->GetTexture())
    {
        io.Fonts->TexID = (void *)(intptr_t)m_font;
    }

    // Setup display size (every frame to accommodate for window resizing)
    vec2 video_size = vec2(0);
    video_size = vec2(Video::GetSize());
    io.DisplaySize = ImVec2(video_size.x, video_size.y);

    //Setup time step
    io.DeltaTime = seconds;
    io.MouseDrawCursor = true;

    //Update Keyboard
    io.KeyCtrl = false;
    io.KeyShift = false;
    for (int i = LolImGuiKey::KEY_START; i < LolImGuiKey::KEY_END; ++i)
    {
        switch (i)
        {
        default:
            io.KeysDown[i] = m_controller->IsKeyPressed(i);
            break;
        case LolImGuiKey::LShift:
        case LolImGuiKey::RShift:
            io.KeyShift = (io.KeyShift || m_controller->IsKeyPressed(i));
            break;
        case LolImGuiKey::LCtrl:
        case LolImGuiKey::RCtrl:
            io.KeyCtrl = (io.KeyCtrl || m_controller->IsKeyPressed(i));
            break;
        }
    }

    //Update text input
    String text = m_keyboard->GetText();
    //text.case_change(io.KeyShift);
    for (int i = 0; i < text.count(); ++i)
        io.AddInputCharacter(text[i]);

    //Update mouse
    if (m_mouse)
    {
        vec2 cursor = m_mouse->GetCursor(0);
        cursor.y = 1.f - cursor.y;
        cursor *= video_size;
        io.MousePos = ImVec2(cursor.x, cursor.y);
        io.MouseWheel = m_controller->GetAxisValue(LolImGuiAxis::Scroll);

        for (int i = LolImGuiKey::MOUSE_KEY_START; i < LolImGuiKey::MOUSE_KEY_END; ++i)
        {
            switch (i)
            {
            default:
                io.MouseDown[i - LolImGuiKey::MOUSE_KEY_START] = m_controller->IsKeyPressed(i);
                break;
            case LolImGuiKey::Focus:
                if (m_controller->IsKeyPressed(i))
                    io.MousePos = ImVec2(-1.f, -1.f);
                break;
            }
        }
    }

    // Start the frame
    ImGui::NewFrame();
}
void LolImGui::TickDraw(float seconds, Scene &scene)
{
    super::TickDraw(seconds, scene);

    ImGuiIO& io = ImGui::GetIO();

    if (io.Fonts->TexID)
        ImGui::Render();
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
void LolImGui::RenderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
{
    g_lolimgui->RenderDrawListsMethod(cmd_lists, cmd_lists_count);
}
void LolImGui::RenderDrawListsMethod(ImDrawList** const cmd_lists, int cmd_lists_count)
{
    if (cmd_lists_count == 0)
        return;

    vec2 size = vec2(Video::GetSize());
    float alpha = 1.f;
    mat4 ortho = mat4::ortho(size.x * alpha, size.y * alpha, -1000.f, 1000.f)
        * mat4::lookat(vec3::axis_z, vec3::zero, vec3::axis_y)
        * mat4::scale(vec3::axis_x - vec3::axis_y - vec3::axis_z)
        * mat4::translate(-size.x * .5f * alpha, -size.y * .5f * alpha, 0.f);

    //Create shader
    if (!m_shader)
    {
        String code;
        m_builder.Build(code);

        m_shader = Shader::Create(m_builder.GetName(), code);
        ASSERT(m_shader);

        m_ortho.m_uniform = m_shader->GetUniformLocation(m_ortho.m_var);
        m_texture.m_uniform = m_shader->GetUniformLocation(m_texture.m_var);

        m_attribs
            << m_shader->GetAttribLocation(VertexUsage::Position, 0)
            << m_shader->GetAttribLocation(VertexUsage::TexCoord, 0)
            << m_shader->GetAttribLocation(VertexUsage::Color, 0);

        m_vdecl = new VertexDeclaration(
            VertexStream<vec2, vec2, u8vec4>(
                VertexUsage::Position,
                VertexUsage::TexCoord,
                VertexUsage::Color));
    }

    //Do not render without shader
    if (!m_shader)
        return;

    RenderContext rc;
    rc.SetCullMode(CullMode::Disabled);
    rc.SetDepthFunc(DepthFunc::Disabled);

    m_shader->Bind();
    for (int n = 0; n < cmd_lists_count; n++)
    {
        const ImDrawList* cmd_list = cmd_lists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->vtx_buffer.front();

        //Register uniforms
        m_shader->SetUniform(m_ortho, ortho);
        m_shader->SetUniform(m_texture, m_font->GetTexture()->GetTextureUniform(), 0);

        int vtx_offset = 0;
        for (size_t cmd_i = 0; cmd_i < cmd_list->commands.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->commands[cmd_i];

            struct Vertex
            {
                vec2 pos, tex;
                u8vec4 color;
            };

            VertexBuffer* vbo = new VertexBuffer(pcmd->vtx_count * sizeof(Vertex));
            Vertex *vert = (Vertex *)vbo->Lock(0, 0);

#ifdef SHOW_IMGUI_DEBUG
            //-----------------------------------------------------------------
            //<Debug render> --------------------------------------------------
            //-----------------------------------------------------------------
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
            ImDrawVert* buf = (ImDrawVert*)(vtx_buffer + vtx_offset);
            for (uint16_t i = 0; i < pcmd->vtx_count; i += 3)
            {
                vec2 pos[3];
                pos[0] = vec2(buf[i + 0].pos.x, buf[i + 0].pos.y);
                pos[1] = vec2(buf[i + 1].pos.x, buf[i + 1].pos.y);
                pos[2] = vec2(buf[i + 2].pos.x, buf[i + 2].pos.y);
                vec4 col[3];
                col[0] = vec4(Color::FromRGBA32(buf[i + 0].col).arg, 1.f);
                col[1] = vec4(Color::FromRGBA32(buf[i + 1].col).arg, 1.f);
                col[2] = vec4(Color::FromRGBA32(buf[i + 2].col).arg, 1.f);
                Debug::DrawLine((off + vec3(pos[0], 0.f)) / mod, (off + vec3(pos[1], 0.f)) / mod, col[0]);
                Debug::DrawLine((off + vec3(pos[1], 0.f)) / mod, (off + vec3(pos[2], 0.f)) / mod, col[1]);
                Debug::DrawLine((off + vec3(pos[2], 0.f)) / mod, (off + vec3(pos[0], 0.f)) / mod, col[2]);
            }
            //-----------------------------------------------------------------
            //<\Debug render> -------------------------------------------------
            //-----------------------------------------------------------------
#endif //SHOW_IMGUI_DEBUG

            memcpy(vert, vtx_buffer + vtx_offset, pcmd->vtx_count * sizeof(Vertex));
            vbo->Unlock();

            m_font->Bind();
            m_vdecl->Bind();
            m_vdecl->SetStream(vbo, m_attribs[0], m_attribs[1], m_attribs[2]);
            m_vdecl->DrawElements(MeshPrimitive::Triangles, 0, pcmd->vtx_count);
            m_vdecl->Unbind();
            m_font->Unbind();

            vtx_offset += pcmd->vtx_count * sizeof(Vertex);

            delete vbo;
        }
    }
    m_shader->Unbind();
}

