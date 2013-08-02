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
// The RenderContext class
// -----------------------
//

#if !defined __LOL_RENDERCONTEXT_H__
#define __LOL_RENDERCONTEXT_H__

namespace lol
{

class RenderContextData;

class RenderContext
{
public:
    RenderContext();
    ~RenderContext();

    void SetViewport(ibox2 viewport);
    void SetClearColor(vec4 color);
    void SetClearDepth(float depth);
    void SetAlphaFunc(AlphaFunc func, float alpha);
    void SetBlendFunc(BlendFunc src, BlendFunc dst);
    void SetDepthFunc(DepthFunc func);
    void SetDepthMask(DepthMask mask);
    void SetCullMode(CullMode mode);
    void SetPolygonMode(PolygonMode mode);

    ibox2 GetViewport();
    vec4 GetClearColor();
    float GetClearDepth();
    AlphaFunc GetAlphaFunc();
    float GetAlphaValue();
    BlendFunc GetBlendFuncSrc();
    BlendFunc GetBlendFuncDst();
    DepthFunc GetDepthFunc();
    DepthMask GetDepthMask();
    CullMode GetCullMode();
    PolygonMode GetPolygonMode();

private:
    RenderContextData *m_data;
};

} /* namespace lol */

#endif // __LOL_RENDERCONTEXT_H__

