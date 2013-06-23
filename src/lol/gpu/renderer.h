//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Renderer class
// ------------------
//

#if !defined __LOL_RENDERER_H__
#define __LOL_RENDERER_H__

namespace lol
{

class RendererData;

/* A list of bitmasks to clear a render buffer. */
struct ClearMask
{
    enum Value
    {
        /* Note: D3D9 doesn't appear to support the accumulation buffer,
         * and it is a deprecated OpenGL feature. No reason to support it. */
        Color   = 1 << 0,
        Depth   = 1 << 1,
        Stencil = 1 << 2,

        All     = 0xffffffff
    }
    m_value;

    inline ClearMask(Value v) : m_value(v) {}
    inline ClearMask(uint64_t i) : m_value((Value)i) {}
    inline operator Value() { return m_value; }
};

/* A safe enum to indicate the blending factors. */
struct BlendFunc
{
    enum Value
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
    }
    m_value;

    inline BlendFunc() : m_value(Zero) {}
    inline BlendFunc(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

/* A safe enum to indicate the face culling mode. */
struct CullMode
{
    enum Value
    {
        Disabled,
        Clockwise,
        CounterClockwise,
    }
    m_value;

    inline CullMode() : m_value(Disabled) {}
    inline CullMode(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

/* A safe enum to indicate the depth test mode. */
struct DepthFunc
{
    enum Value
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
    }
    m_value;

    inline DepthFunc() : m_value(Disabled) {}
    inline DepthFunc(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

/* A safe enum to indicate the alpha test mode. */
struct AlphaFunc
{
    enum Value
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
    }
    m_value;

    inline AlphaFunc() : m_value(Disabled) {}
    inline AlphaFunc(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

class Renderer
{
private:
    /* Only the Video class can create a renderer for now. */
    friend class Video;

    Renderer(ivec2 size);
    ~Renderer();

    void *GetDevice();

public:
    void Clear(ClearMask mask);

public:
    void SetViewport(ibox2 viewport);
    ibox2 GetViewport() const;

    void SetClearColor(vec4 color);
    vec4 GetClearColor() const;

    void SetClearDepth(float depth);
    float GetClearDepth() const;

    void SetAlphaFunc(AlphaFunc func, float alpha);
    AlphaFunc GetAlphaFunc() const;
    float GetAlphaValue() const;

    void SetBlendFunc(BlendFunc src, BlendFunc dst);
    BlendFunc GetBlendFuncSrc() const;
    BlendFunc GetBlendFuncDst() const;

    void SetDepthFunc(DepthFunc func);
    DepthFunc GetDepthFunc() const;

    void SetFaceCulling(CullMode mode);
    CullMode GetFaceCulling() const;

private:
    RendererData *m_data;
};

extern Renderer *g_renderer;

} /* namespace lol */

#endif // __LOL_RENDERER_H__

