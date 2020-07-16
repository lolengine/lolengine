//
//  Lol Engine — Shader builder tutorial
//
//  Copyright © 2012—2020 Sam Hocevar <sam@hocevar.net>
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
#include "loldebug.h"

#include <lol/file> // lol::file::write
#include <cstdio>
#include <string>

using namespace lol;

class ShaderBuilderDemo : public WorldEntity
{
public:
    ShaderBuilderDemo()
    {
    }

    virtual void tick_game(float seconds)
    {
        WorldEntity::tick_game(seconds);
    }

    virtual void tick_draw(float seconds, Scene &scene)
    {
        WorldEntity::tick_draw(seconds, scene);

        std::string code;

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
        nothing_vertex.SetMainCode(
            pass_color.tostring() + " = " + in_color.tostring() + ";\n" +
            out_vertex.tostring() + " = vec4(" + in_position.tostring() + ", 0.f);\n"
            );

        ShaderVar ambient = ShaderVar(ShaderVariable::InOut, ShaderVariableType::Vec4, "ambient");

        red_pixel.AddVar(pass_color);
        red_pixel.AddVar(out_pixel);
        red_pixel.AddVar(ambient);
        red_pixel.SetMainCode(
            out_pixel.tostring() + " = " + pass_color.tostring() + ";\n" +
            out_pixel.tostring() + ".r = 1.0;\n"
            "ambient = vec4(1.0);\n"
            );

        green_pixel.AddVar(pass_color);
        green_pixel.AddVar(out_pixel);
        green_pixel.AddVar(ambient);
        green_pixel.SetMainCode(
            out_pixel.tostring() + " = " + pass_color.tostring() + ";\n" +
            out_pixel.tostring() + ".g = 1.0;\n"
            "ambient.r = 0.0;\n"
            );

        blue_pixel.AddVar(pass_color);
        blue_pixel.AddVar(out_pixel);
        blue_pixel.AddVar(ambient);
        blue_pixel.SetCustomCode(
            "void SetAmbient(inout vec4 ambient)\n"
            "{\n"
            "    ambient = vec4(1.0, 1.0, 1.0, 1.0);\n"
            "}\n");
        blue_pixel.SetMainCode(
            out_pixel.tostring() + " = " + pass_color.tostring() + ";\n" +
            out_pixel.tostring() + ".b = 1.0;\n"
            "SetAmbient(ambient);\n" +
            out_pixel.tostring() + " *= ambient;\n"
            );

        builder << ShaderProgram::Vertex
            << nothing_vertex
            << ShaderProgram::Pixel
            << red_pixel
            << green_pixel
            << blue_pixel;

        code = builder.Build();

        file::write("13_shader_builder_export.txt", code);

        std::shared_ptr<Shader> shader = Shader::Create(builder.GetName(), code);
        shader = nullptr;

        Ticker::Shutdown();
    }
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    auto app = app::init("Tutorial 13: Shader Builder", ivec2(1280, 720), 60.0f);

    new ShaderBuilderDemo();

    app->run();
    return EXIT_SUCCESS;
}
