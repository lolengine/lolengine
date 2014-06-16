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
// The Image class
// ---------------
//

#if !defined __LOL_IMAGE_IMAGE_H__
#define __LOL_IMAGE_IMAGE_H__

#include <lol/math/vector.h>

namespace lol
{

/* FIXME: is it possible to avoid the cast to int here? */
template <PixelFormat T> struct PixelType { typedef void type; };
template<> struct PixelType<PixelFormat::Y_8> { typedef uint8_t type; };
template<> struct PixelType<PixelFormat::RGB_8> { typedef u8vec3 type; };
template<> struct PixelType<PixelFormat::RGBA_8> { typedef u8vec4 type; };

class Image
{
    friend class ImageBank;
    friend class ImageCodec;

public:
    Image();
    ~Image();

    static Image *Create(char const *path);

    bool Save(char const *path);
    void Destroy();

    PixelFormat GetFormat() const;
    void *LockGeneric();
    template<PixelFormat T> typename PixelType<T>::type *Lock();
    void Unlock();

    ivec2 GetSize() const;
    void SetSize(ivec2);

    bool RetrieveTiles(Array<ivec2, ivec2>& tiles) const;

private:
    class ImageData *m_data;
    String m_path;
};

} /* namespace lol */

#endif // __LOL_IMAGE_IMAGE_H__

