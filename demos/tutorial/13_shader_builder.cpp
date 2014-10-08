//
// Lol Engine - Graphing tutorial
//
// Copyright: (c) 2012-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>
#include "loldebug.h"
#include <cstdio>

using namespace lol;

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

        String vertex_out = Shader::GetProgramOutVariableLocal(ShaderProgram::Vertex);
        String pixel_out = Shader::GetProgramOutVariableLocal(ShaderProgram::Pixel);

        String in_position = Shader::GetVariablePrefix(ShaderVariable::Attribute) + "position";
        String in_color = Shader::GetVariablePrefix(ShaderVariable::Attribute) + "color";
        String pass_color = Shader::GetVariablePrefix(ShaderVariable::Varying) + "color";

        nothing_vertex.Add(ShaderVariable::Attribute, "vec3", "position");
        nothing_vertex.Add(ShaderVariable::Attribute, "vec4", "color");
        nothing_vertex.Add(ShaderVariable::Varying, "vec4", "color");
        nothing_vertex.Add(ShaderVariable::InOut, "vec4", vertex_out);
        nothing_vertex.SetMainCode(
            String("    ") + pass_color + " = " + in_color + ";\n" +
            String("    ") + vertex_out + " = vec4(" + in_position + ", 0.f);\n"
            );

        red_pixel.Add(ShaderVariable::Varying, "vec4", "color");
        red_pixel.Add(ShaderVariable::InOut, "vec4", pixel_out);
        red_pixel.Add(ShaderVariable::InOut, "vec4", "ambient");
        red_pixel.SetMainCode(
            String("    ") + pixel_out + " = " + pass_color + ";\n" +
            String("    ") + pixel_out + ".r = 1.0;\n" +
            String("    ") + "ambient = vec4(1.0);\n"
            );

        green_pixel.Add(ShaderVariable::Varying, "vec4", "color");
        green_pixel.Add(ShaderVariable::InOut, "vec4", pixel_out);
        green_pixel.Add(ShaderVariable::InOut, "vec4", "ambient");
        green_pixel.SetMainCode(
            String("    ") + pixel_out + " = " + pass_color + ";\n" +
            String("    ") + pixel_out + ".g = 1.0;\n" +
            String("    ") + "ambient.r = 0.0;\n"
            );

        blue_pixel.Add(ShaderVariable::Varying, "vec4", "color");
        blue_pixel.Add(ShaderVariable::InOut, "vec4", pixel_out);
        blue_pixel.Add(ShaderVariable::InOut, "vec4", "ambient");
        blue_pixel.SetCustomCode(
            String("void SetAmbient(inout vec4 ambient)\n{\n    ambient = vec4(1.0, 1.0, 1.0, 1.0);\n}"));
        blue_pixel.SetMainCode(
            String("    ") + pixel_out + " = " + pass_color + ";\n" +
            String("    ") + pixel_out + ".b = 1.0;\n" +
            String("    ") + "SetAmbient(ambient);\n" +
            String("    ") + pixel_out + " *= ambient;\n"
            );

        builder << ShaderProgram::Vertex
            << &nothing_vertex
            << ShaderProgram::Pixel
            << &red_pixel
            << &green_pixel
            << &blue_pixel;

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

