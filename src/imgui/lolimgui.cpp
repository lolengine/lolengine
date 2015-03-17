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

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include <lol/engine.h>
#include "loldebug.h"

using namespace lol;

//-----------------------------------------------------------------------------
LolImGui::LolImGui()
{
    m_drawgroup = DRAWGROUP_HUD;
}
LolImGui::~LolImGui()
{
    ImGui::GetIO().Fonts->TexID = nullptr;
    Ticker::Unref(m_font);
    m_font = nullptr;
}

//-------------------------------------------------------------------------
LolImGui* g_lolimgui = nullptr;
void LolImGui::Init()
{
    Ticker::Ref(g_lolimgui = new LolImGui());

    ImGuiIO& io = ImGui::GetIO();
    //io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                 // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    //io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    //io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    //io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    //io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    //io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    //io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    //io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    //io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    //io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    //io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    //io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    //io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    //io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    //io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    //io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    //io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    //Func pointer
    io.RenderDrawListsFn = LolImGui::RenderDrawLists;
    io.SetClipboardTextFn = LolImGui::SetClipboard;
    io.GetClipboardTextFn = LolImGui::GetClipboard;

    /* nope
#ifdef _MSC_VER
    io.ImeWindowHandle = glfwGetWin32Window(g_Window);
#endif
    */

    /* Callback not needed but look into these to send IO stuff
    if (install_callbacks)
    {
        glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
        glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
        glfwSetKeyCallback(window, ImGui_ImplGlFw_KeyCallback);
        glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
    }
    */
}

/* CALLBACKS
void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
if (action == GLFW_PRESS && button >= 0 && button < 3)
g_MousePressed[button] = true;
}

void ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
g_MouseWheel += (float)yoffset; // Use fractional mouse wheel, 1.0 unit 5 lines.
}

void ImGui_ImplGlFw_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
ImGuiIO& io = ImGui::GetIO();
if (action == GLFW_PRESS)
io.KeysDown[key] = true;
if (action == GLFW_RELEASE)
io.KeysDown[key] = false;
io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
}

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
        ImGuiIO& io = ImGui::GetIO();

        // Build texture
        unsigned char* pixels;
        ivec2 size;
        io.Fonts->GetTexDataAsAlpha8(&pixels, &size.w, &size.h);

        Image* image = new Image();
        image->Copy(pixels, size, PixelFormat::RGBA_8);
        image->SetSize(size);

        Ticker::Ref(m_font = new TextureImage("", image));

        //// Create texture
        //glGenTextures(1, &g_FontTexture);
        //glBindTexture(GL_TEXTURE_2D, g_FontTexture);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

        //// Store our identifier
        //io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

        //return true;
    }
    //Texture has been created
    if (m_font && m_font->GetTexture())
    {
        io.Fonts->TexID = (void *)(intptr_t)m_font;
    }

    // Setup display size (every frame to accommodate for window resizing)
    vec2 video_size = vec2(0);
    video_size = vec2(Video::GetSize());
    io.DisplaySize = ImVec2(video_size.w, video_size.h);

    // Setup time step
    io.DeltaTime = seconds;

    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    //double mouse_x, mouse_y;
    //glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
    //mouse_x *= (float)display_w / w;                                                    // Convert mouse coordinates to pixels
    //mouse_y *= (float)display_h / h;
    //io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);                               // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)

    /*
    for (int i = 0; i < 3; i++)
    {
        io.MouseDown[i] = g_MousePressed[i] || glfwGetMouseButton(g_Window, i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        g_MousePressed[i] = false;
    }

    io.MouseWheel = g_MouseWheel;
    g_MouseWheel = 0.0f;
    */

    // Start the frame
    ImGui::NewFrame();

}
void LolImGui::TickDraw(float seconds, Scene &scene)
{
    super::TickDraw(seconds, scene);

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
    if (cmd_lists_count == 0)
        return;

    /*
    // We are using the OpenGL fixed pipeline to make the example code simpler to read!
    // A probable faster way to render would be to collate all vertices from all cmd_lists into a single vertex buffer.
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);

    // Setup orthographic projection matrix
    const float width = ImGui::GetIO().DisplaySize.x;
    const float height = ImGui::GetIO().DisplaySize.y;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render command lists
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    for (int n = 0; n < cmd_lists_count; n++)
    {
        const ImDrawList* cmd_list = cmd_lists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->vtx_buffer.front();
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col)));

        int vtx_offset = 0;
        for (size_t cmd_i = 0; cmd_i < cmd_list->commands.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->commands[cmd_i];
            if (pcmd->user_callback)
            {
                pcmd->user_callback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->texture_id);
                glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
                glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
            }
            vtx_offset += pcmd->vtx_count;
        }
    }
#undef OFFSETOF

    // Restore modified state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    */
}

