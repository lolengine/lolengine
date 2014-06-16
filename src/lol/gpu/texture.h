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

enum class PixelFormat
{
    /* XXX: make sure to update texture.cpp when this changes */
    Unknown = 0,
    Y_8,
    RGB_8,
    RGBA_8,
    ARGB_8,
    ABGR_8,
    Y_F32,
    RGB_F32,
    RGBA_F32,
};

static inline uint8_t BytesPerPixel(PixelFormat format)
{
    switch (format)
    {
    case PixelFormat::Y_8:
        return 1;
    case PixelFormat::RGB_8:
        return 3;
    case PixelFormat::RGBA_8:
    case PixelFormat::ARGB_8:
    case PixelFormat::ABGR_8:
        return 4;
    case PixelFormat::Y_F32:
        return 4;
    case PixelFormat::RGB_F32:
        return 12;
    case PixelFormat::RGBA_F32:
        return 16;
    default:
        return 1;
    }
};

struct TextureMagFilter
{
    enum Value
    {
        Unknown = 0,
        NEAREST_TEXEL,
        LINEAR_TEXEL,
    }
    m_value;

    inline TextureMagFilter() : m_value(Unknown) {}
    inline TextureMagFilter(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

struct TextureMinFilter
{
    enum Value
    {
        Unknown = 0,
        NEAREST_TEXEL_NO_MIPMAP,
        LINEAR_TEXEL_NO_MIPMAP,
        NEAREST_TEXEL_NEAREST_MIPMAP,
        NEAREST_TEXEL_LINEAR_MIPMAP,
        LINEAR_TEXEL_NEAREST_MIPMAP,
        LINEAR_TEXEL_LINEAR_MIPMAP,
    }
    m_value;

    inline TextureMinFilter() : m_value(Unknown) {}
    inline TextureMinFilter(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

class Texture
{
public:
    Texture(ivec2 size, PixelFormat format);
    ~Texture();

    void Bind();
    void SetData(void *data);
    void SetSubData(ivec2 origin, ivec2 size, void *data);

    void SetMagFiltering(TextureMagFilter filter);
    void SetMinFiltering(TextureMinFilter filter);
    void GenerateMipmaps();

    TextureUniform GetTextureUniform() const;

private:
    class TextureData *m_data;
};

} /* namespace lol */

#endif // __LOL_GPU_TEXTURE_H__

