//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <lol/gpu/renderer.h>

#include <memory>

//
// The RenderContext class
// -----------------------
//

namespace lol
{

class RenderContextData;

class render_context
{
public:
    render_context(std::shared_ptr<Renderer> renderer);
    ~render_context();

    void viewport(ibox2 viewport);
    void clear_color(vec4 color);
    void clear_depth(float depth);
    void alpha_func(AlphaFunc func, float alpha);
    void blend_equation(BlendEquation rgb, BlendEquation alpha);
    void blend_func(BlendFunc src, BlendFunc dst);
    void depth_func(DepthFunc func);
    void depth_mask(DepthMask mask);
    void cull_mode(CullMode mode);
    void polygon_mode(PolygonMode mode);
    void scissor_mode(ScissorMode mode);
    void scissor_rect(vec4 rect);

    ibox2 viewport();
    vec4 clear_color();
    float clear_depth();
    AlphaFunc alpha_func();
    float alpha_value();
    BlendEquation blend_equation_rgb();
    BlendEquation blend_equation_alpha();
    BlendFunc blend_func_src();
    BlendFunc blend_func_dst();
    DepthFunc depth_func();
    DepthMask depth_mask();
    CullMode cull_mode();
    PolygonMode polygon_mode();
    ScissorMode scissor_mode();
    vec4 scissor_rect();

private:
    std::shared_ptr<Renderer> m_renderer;
    std::unique_ptr<RenderContextData> m_data;
};

} /* namespace lol */

