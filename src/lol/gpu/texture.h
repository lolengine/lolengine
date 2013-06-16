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
// The Texture class
// -----------------
//

#if !defined __LOL_GPU_TEXTURE_H__
#define __LOL_GPU_TEXTURE_H__

namespace lol
{

struct PixelFormat
{
    /* XXX: make sure to update texture.cpp when this changes */
    enum Value
    {
        Unknown = 0,
        RGB_8,
        RGBA_8,
        ARGB_8,
        ABGR_8,
        Y_8,
    }
    m_value;

    inline PixelFormat() : m_value(Unknown) {}
    inline PixelFormat(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }

    inline uint8_t BytesPerPixel()
    {
        switch (m_value)
        {
        case Y_8:
            return 1;
        case RGB_8:
            return 3;
        case RGBA_8:
        case ARGB_8:
        case ABGR_8:
        default:
            return 4;
        }
    }
};

class Texture
{
public:
    Texture(ivec2 size, PixelFormat format);
    ~Texture();

    void Bind();
    void SetData(void *data);
    void SetSubData(ivec2 origin, ivec2 size, void *data);

    ShaderTexture GetTexture() const;

private:
    class TextureData *m_data;
};

} /* namespace lol */

#endif // __LOL_GPU_TEXTURE_H__

