//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Framebuffer class
// ---------------------
//

#include <cstring>

namespace lol
{

struct FramebufferFormat
{
    enum Value
    {
        R_8,
        R_8_I,
        R_8_UI,
        R_8_F,

        R_16,
        R_16_I,
        R_16_UI,
        R_16_F,

        R_32,
        R_32_I,
        R_32_UI,
        R_32_F,

        RG_8,
        RG_8_I,
        RG_8_UI,
        RG_8_F,

        RG_16,
        RG_16_I,
        RG_16_UI,
        RG_16_F,

        RG_32,
        RG_32_I,
        RG_32_UI,
        RG_32_F,

        RGB_8,
        RGB_8_I,
        RGB_8_UI,
        RGB_8_F,

        RGB_16,
        RGB_16_I,
        RGB_16_UI,
        RGB_16_F,

        RGB_32,
        RGB_32_I,
        RGB_32_UI,
        RGB_32_F,

        RGBA_8,
        RGBA_8_I,
        RGBA_8_UI,
        RGBA_8_F,

        RGBA_16,
        RGBA_16_I,
        RGBA_16_UI,
        RGBA_16_F,

        RGBA_32,
        RGBA_32_I,
        RGBA_32_UI,
        RGBA_32_F,
    }
    m_format;
    bool m_invert_rgb;

    inline FramebufferFormat(Value format=RGBA_8, bool invert_rgb=true)
            : m_format(format), m_invert_rgb(invert_rgb) {}
    inline uint32_t GetFormat();
    inline uint32_t GetFormatOrder();
    inline operator Value() { return m_format; }
};

class Framebuffer
{
public:
    Framebuffer(ivec2 size, FramebufferFormat fbo_format = FramebufferFormat());
    ~Framebuffer();

    TextureUniform GetTextureUniform() const;
    ivec2 GetSize() const;

    class old_image GetImage() const;

    void Bind();
    void Unbind();

private:
    class FramebufferData *m_data;
};

} /* namespace lol */

