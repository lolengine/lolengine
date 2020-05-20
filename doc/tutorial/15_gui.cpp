//
//  Lol Engine — Dear ImGui tutorial
//
//  Copyright © 2012—2019 Sam Hocevar <sam@hocevar.net>
//            © 2002—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>

#include <cstdio>
#include <string>

#include "loldebug.h"

using namespace lol;

class LolImGuiDemo : public WorldEntity
{
public:
    LolImGuiDemo()
    {
        gui::init();
    }

    ~LolImGuiDemo()
    {
        gui::shutdown();
    }

    virtual void tick_game(float seconds)
    {
        WorldEntity::tick_game(seconds);

        static float f;
        //static vec3 clear_color;
        static char buf[512];
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowFocus();
        ImGui::Begin("testature");
        {
            if (ImGui::IsWindowHovered())
                ImGui::Text("Hovered: true");
            else
                ImGui::Text("Hovered: false");
            if (ImGui::IsWindowFocused())
                ImGui::Text("Focused: true");
            else
                ImGui::Text("Focused: false");
            ImGui::Text("Hello, world!");
            ImGui::Text("prout!");
            ImGui::Text("prout!%i", 100);
            ImGui::Text("MousePos!%.2f/%.2f", io.MousePos.x, io.MousePos.y);
            ImGui::Button("Test Window");
            ImGui::Text("Slider: %.2f", f);
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            //ImGui::ColorEdit3("clear color", (float*)&clear_color);
            ImGui::Text("Left Mouse: %s", io.MouseDown[0] ? "true" : "false");
            ImGui::Text("Scroll: %f", io.MouseWheel);
            ImGui::Text("Maj: %s", io.KeyShift ? "true" : "false");
            ImGui::Text("Ctrl: %s", io.KeyCtrl ? "true" : "false");
            ImGui::Text("Clipboard %s", gui::clipboard().c_str());
            ImGui::InputText("base input", buf, 512);
        }
        ImGui::End();
        ImGui::Begin("SO FUN !!");
        {
            if (ImGui::IsWindowHovered())
                ImGui::Text("Hovered: true");
            else
                ImGui::Text("Hovered: false");
            if (ImGui::IsWindowFocused())
                ImGui::Text("Focused: true");
            else
                ImGui::Text("Focused: false");
            ImGui::Text("poucka!");
            ImGui::Text("    poucka!");
        }
        ImGui::End();
    }

    virtual void tick_draw(float seconds, Scene &scene)
    {
        WorldEntity::tick_draw(seconds, scene);
    }
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    Application app("Tutorial 15: ImGui", ivec2(800, 600), 60.0f);

    new LolImGuiDemo();

    app.Run();
    return EXIT_SUCCESS;
}

