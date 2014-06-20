//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
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

template <PixelFormat T> struct PixelType { typedef void type; };
template<> struct PixelType<PixelFormat::Y_8> { typedef uint8_t type; };
template<> struct PixelType<PixelFormat::RGB_8> { typedef u8vec3 type; };
template<> struct PixelType<PixelFormat::RGBA_8> { typedef u8vec4 type; };
template<> struct PixelType<PixelFormat::Y_F32> { typedef float type; };
template<> struct PixelType<PixelFormat::RGB_F32> { typedef vec3 type; };
template<> struct PixelType<PixelFormat::RGBA_F32> { typedef vec4 type; };

enum class WrapMode : uint8_t
{
    Clamp,
    Repeat,
};

enum class ScanMode : uint8_t
{
    Raster,
    Serpentine,
};

enum class EdiffAlgorithm : uint8_t
{
    FloydSteinberg,
    JaJuNi,
    Atkinson,
    Fan,
    ShiauFan,
    ShiauFan2,
    Stucki,
    Burkes,
    Sierra,
    Sierra2,
    Lite,
};

class Image
{
public:
    Image();
    Image(ivec2 size);
    /* XXX: use of this ctor should be discouraged, as it will not
     * return information about a possible error. */
    Image(char const *path);

    /* Rule of three */
    Image(Image const &other);
    Image & operator =(Image other);
    ~Image();

    bool Load(char const *path);
    bool Save(char const *path);

    /* Low level access */
    ivec2 GetSize() const;
    void SetSize(ivec2);

    PixelFormat GetFormat() const;
    void SetFormat(PixelFormat fmt);

    WrapMode GetWrapX() const;
    WrapMode GetWrapY() const;
    void SetWrap(WrapMode wrap_x, WrapMode wrap_y);

    template<PixelFormat T> typename PixelType<T>::type *Lock();
    void *Lock();
    void Unlock(void const *pixels);

    bool RetrieveTiles(Array<ivec2, ivec2>& tiles) const;

    /* Image processing kernels */
    static Array2D<float> BayerKernel(ivec2 size);
    static Array2D<float> HalftoneKernel(ivec2 size);
    static Array2D<float> EdiffKernel(EdiffAlgorithm algorithm);

    /* Rendering */
    bool Stock(char const *desc);
    bool RenderRandom(ivec2 size);

    /* Image processing */
    Image AutoContrast() const;
    Image Convolution(Array2D<float> const &kernel);

    Image DitherRandom() const;
    Image DitherEdiff(Array2D<float> const &kernel,
                      ScanMode scan = ScanMode::Raster) const;
    Image DitherOstromoukhov(ScanMode scan = ScanMode::Raster) const;

private:
    class ImageData *m_data;
};

} /* namespace lol */

#endif // __LOL_IMAGE_IMAGE_H__

