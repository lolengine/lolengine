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

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void SetClearColor(vec4 color);
    vec4 GetClearColor() const;

    void SetClearDepth(float depth);
    float GetClearDepth() const;

    void SetAlphaBlend(bool set);
    bool GetAlphaBlend() const;

    void SetDepthTest(bool set);
    bool GetDepthTest() const;

    void SetFaceCulling(bool set);
    bool GetFaceCulling() const;

private:
    RendererData *m_data;
};

extern Renderer *g_renderer;

} /* namespace lol */

#endif // __LOL_RENDERER_H__

