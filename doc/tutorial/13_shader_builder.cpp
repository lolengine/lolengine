//
//  Lol Engine — Shader builder tutorial
//
//  Copyright © 2012—2015 Sam Hocevar <sam@hocevar.net>
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

#include <lol/engine.h>
#include "loldebug.h"
#include <cstdio>

using namespace lol;

#define Line(x) ((x) + "\n")

class ShaderBuilderDemo : public WorldEntity
{
public:
    ShaderBuilderDemo()
    {
    }

    virtual void TickGame(float seconds)
    {
        WorldEntity::TickGame(seconds);
    }

    virtual void TickDraw(float seconds, Scene &scene)
    {
        WorldEntity::TickDraw(seconds, scene);

        File file;
        file.Open("13_shader_builder_export.txt", FileAccess::Write);
        //file.Open("13_shader_builder_export.txt", FileAccess::Read);
        String code;

        ShaderBuilder builder("red_blue_green", "120");
        ShaderBlock nothing_vertex("NothingVertex");
        ShaderBlock red_pixel("RedPixel");
        ShaderBlock green_pixel("GreenPixel");
        ShaderBlock blue_pixel("BluePixel");

        ShaderVar out_vertex = ShaderVar::GetShaderOut(ShaderProgram::Vertex);
        ShaderVar out_pixel = ShaderVar::GetShaderOut(ShaderProgram::Pixel);

        ShaderVar in_position = ShaderVar(ShaderVariable::Attribute, ShaderVariableType::Vec3, "position");
        ShaderVar in_color = ShaderVar(ShaderVariable::Attribute, ShaderVariableType::Vec4, "color");
        ShaderVar pass_color = ShaderVar(ShaderVariable::Varying, ShaderVariableType::Vec4, "color");

        nothing_vertex << in_position
                        << in_color
                        << pass_color;
        nothing_vertex.AddVar(out_vertex);
        nothing_vertex.SetMainCode(String() +
            Line(pass_color + " = " + in_color + ";") +
            Line(out_vertex + " = vec4(" + in_position + ", 0.f);")
            );

        ShaderVar ambient = ShaderVar(ShaderVariable::InOut, ShaderVariableType::Vec4, "ambient");

        red_pixel.AddVar(pass_color);
        red_pixel.AddVar(out_pixel);
        red_pixel.AddVar(ambient);
        red_pixel.SetMainCode(String() +
            out_pixel + " = " + pass_color + ";\n" +
            out_pixel + ".r = 1.0;\n" +
            "ambient = vec4(1.0);\n"
            );

        green_pixel.AddVar(pass_color);
        green_pixel.AddVar(out_pixel);
        green_pixel.AddVar(ambient);
        green_pixel.SetMainCode(String() +
            out_pixel + " = " + pass_color + ";\n" +
            out_pixel + ".g = 1.0;\n" +
            "ambient.r = 0.0;\n"
            );

        blue_pixel.AddVar(pass_color);
        blue_pixel.AddVar(out_pixel);
        blue_pixel.AddVar(ambient);
        blue_pixel.SetCustomCode(String() +
            "void SetAmbient(inout vec4 ambient)\n" +
            "{\n" +
            "    ambient = vec4(1.0, 1.0, 1.0, 1.0);\n" +
            "}\n");
        blue_pixel.SetMainCode(String() +
            out_pixel + " = " + pass_color + ";\n" +
            out_pixel + ".b = 1.0;\n" +
            "SetAmbient(ambient);\n" +
            out_pixel + " *= ambient;\n"
            );

        builder << ShaderProgram::Vertex
            << nothing_vertex
            << ShaderProgram::Pixel
            << red_pixel
            << green_pixel
            << blue_pixel;

        builder.Build(code);

        file.WriteString(code);
        //code = file.ReadString();
        file.Close();

        Shader* shader = Shader::Create(builder.GetName(), code);
        Shader::Destroy(shader);

        Ticker::Shutdown();
    }
};

int main(int argc, char **argv)
{
    System::Init(argc, argv);

    Application app("Tutorial 13: Shader Builder", ivec2(1280, 720), 60.0f);

    new ShaderBuilderDemo();

    app.Run();
    return EXIT_SUCCESS;
}

