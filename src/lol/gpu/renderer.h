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

//
// The Renderer class
// ------------------
//

namespace lol
{

class RendererData;

/* A list of bitmasks to clear a render buffer. */
enum class ClearMask : uint8_t
{
    /* Note: D3D9 doesn't appear to support the accumulation buffer,
     * and it is a deprecated OpenGL feature. No reason to support it. */
    Color   = 1 << 0,
    Depth   = 1 << 1,
    Stencil = 1 << 2,

    All     = 0xff,
};

static inline ClearMask operator |(ClearMask const &a, ClearMask const &b)
{
    return ClearMask((uint8_t)a | (uint8_t)b);
}

static inline bool operator &(ClearMask const &a, ClearMask const &b)
{
    return !!((uint8_t)a & (uint8_t)b);
}

/* A safe enum to indicate the blend equation. */
enum class BlendEquation : uint8_t
{
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max,
};

/* A safe enum to indicate the blending factors. */
enum class BlendFunc : uint8_t
{
    Disabled,
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
};

/* A safe enum to indicate the face culling mode. */
enum class CullMode : uint8_t
{
    Disabled,
    Clockwise,
    CounterClockwise,
};

/* A safe enum to indicate the polygon mode. */
enum class PolygonMode : uint8_t
{
    Fill,
    Line,
    Point,
};

/* A safe enum to indicate the depth test mode. */
enum class DepthFunc : uint8_t
{
    Disabled,
    Never,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always,
};

/* A safe enum to indicate the depth mask. */
enum class DepthMask : uint8_t
{
    Disabled,
    Enabled,
};

/* A safe enum to indicate the alpha test mode. */
enum class AlphaFunc : uint8_t
{
    Disabled,
    Never,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always,
};

/* A safe enum to indicate the depth mask. */
enum class ScissorMode : uint8_t
{
    Disabled,
    Enabled,
};

class Renderer
{
public:
    // FIXME: only the Scene class should be allowed to create a renderer
    Renderer(ivec2 size);
    ~Renderer();

    void Clear(ClearMask mask);

    void SetViewport(ibox2 viewport);
    ibox2 GetViewport() const;
    float GetXYRatio() const;
    float GetYXRatio() const;

    void SetClearColor(vec4 color);
    vec4 GetClearColor() const;

    void SetClearDepth(float depth);
    float GetClearDepth() const;

    void SetAlphaFunc(AlphaFunc func, float alpha);
    AlphaFunc GetAlphaFunc() const;
    float GetAlphaValue() const;

    void SetBlendEquation(BlendEquation rgb, BlendEquation alpha);
    BlendEquation GetBlendEquationRgb() const;
    BlendEquation GetBlendEquationAlpha() const;

    void SetBlendFunc(BlendFunc src, BlendFunc dst);
    BlendFunc GetBlendFuncSrc() const;
    BlendFunc GetBlendFuncDst() const;

    void SetDepthFunc(DepthFunc func);
    DepthFunc GetDepthFunc() const;

    void SetDepthMask(DepthMask mask);
    DepthMask GetDepthMask() const;

    void SetCullMode(CullMode mode);
    CullMode GetCullMode() const;

    void SetPolygonMode(PolygonMode mode);
    PolygonMode GetPolygonMode() const;

    void SetScissorMode(ScissorMode mode);
    void SetScissorRect(vec4 rect);
    ScissorMode GetScissorMode() const;
    vec4 GetScissorRect() const;

private:
    std::unique_ptr<RendererData> m_data;
};

} /* namespace lol */

