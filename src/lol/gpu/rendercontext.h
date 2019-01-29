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

class RenderContext
{
public:
    RenderContext(std::shared_ptr<Renderer> renderer);
    ~RenderContext();

    void SetViewport(ibox2 viewport);
    void SetClearColor(vec4 color);
    void SetClearDepth(float depth);
    void SetAlphaFunc(AlphaFunc func, float alpha);
    void SetBlendEquation(BlendEquation rgb, BlendEquation alpha);
    void SetBlendFunc(BlendFunc src, BlendFunc dst);
    void SetDepthFunc(DepthFunc func);
    void SetDepthMask(DepthMask mask);
    void SetCullMode(CullMode mode);
    void SetPolygonMode(PolygonMode mode);
    void SetScissorMode(ScissorMode mode);
    void SetScissorRect(vec4 rect);

    ibox2 GetViewport();
    vec4 GetClearColor();
    float GetClearDepth();
    AlphaFunc GetAlphaFunc();
    float GetAlphaValue();
    BlendEquation GetBlendEquationRgb();
    BlendEquation GetBlendEquationAlpha();
    BlendFunc GetBlendFuncSrc();
    BlendFunc GetBlendFuncDst();
    DepthFunc GetDepthFunc();
    DepthMask GetDepthMask();
    CullMode GetCullMode();
    PolygonMode GetPolygonMode();
    ScissorMode GetScissorMode();
    vec4 GetScissorRect();

private:
    std::shared_ptr<Renderer> m_renderer;
    std::unique_ptr<RenderContextData> m_data;
};

} /* namespace lol */

