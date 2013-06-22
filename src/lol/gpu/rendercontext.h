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

    void SetClearColor(vec4 color);
    void SetClearDepth(float depth);
    void SetAlphaBlend(bool set);
    void SetBlendFunc(BlendFunc src, BlendFunc dst);
    void SetAlphaTest(bool set);
    void SetDepthFunc(DepthFunc func);
    void SetFaceCulling(CullMode mode);

private:
    RenderContextData *m_data;
};

} /* namespace lol */

#endif // __LOL_RENDERCONTEXT_H__

