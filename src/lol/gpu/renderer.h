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

/* A safe enum to indicate the blending factors. */
struct BlendFactor
{
    enum Value
    {
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

    inline BlendFactor() : m_value(Zero) {}
    inline BlendFactor(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

/* A safe enum to indicate the face culling mode. */
struct CullMode
{
    enum Value
    {
        None,
        CW,
        CCW,
    }
    m_value;

    inline CullMode() : m_value(None) {}
    inline CullMode(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

class Renderer
{
private:
    /* Only the Video class can create a renderer for now. */
    friend class Video;

    Renderer();
    ~Renderer();

public:
    void SetClearColor(vec4 color);
    vec4 GetClearColor() const;

    void SetClearDepth(float depth);
    float GetClearDepth() const;

    void SetAlphaBlend(bool set);
    bool GetAlphaBlend() const;

    void SetBlendFunc(BlendFactor src, BlendFactor dst);
    BlendFactor GetBlendFuncSrc() const;
    BlendFactor GetBlendFuncDst() const;

    void SetAlphaTest(bool set);
    bool GetAlphaTest() const;

    void SetDepthTest(bool set);
    bool GetDepthTest() const;

    void SetFaceCulling(CullMode mode);
    CullMode GetFaceCulling() const;

private:
    RendererData *m_data;
};

extern Renderer *g_renderer;

} /* namespace lol */

#endif // __LOL_RENDERER_H__

