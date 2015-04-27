//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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

class Renderer
{
private:
    /* Only the Video class can create a renderer for now. */
    friend class Video;

    Renderer(ivec2 size);
    ~Renderer();

public:
    void Clear(ClearMask mask);

    void *GetDevice();

    static void AddNew(ivec2 size);
    static int GetCount();
    static Renderer* Get(int index = 0);
    static void DestroyAll();

public:
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

private:
    RendererData *m_data;
};

//extern Renderer *g_renderer;

} /* namespace lol */

