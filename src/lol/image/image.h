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

enum class ResampleAlgorithm : uint8_t
{
    Bicubic,
    Bresenham,
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

    template<PixelFormat T>
        array2d<typename PixelType<T>::type> &Lock2D();
    template<typename T>
        void Unlock2D(array2d<T> const &);

    bool RetrieveTiles(array<ivec2, ivec2>& tiles) const;

    /* Image processing kernels */
    static array2d<float> BayerKernel(ivec2 size);
    static array2d<float> HalftoneKernel(ivec2 size);
    static array2d<float> BlueNoiseKernel(ivec2 size,
                                          ivec2 gsize = ivec2(7, 7));
    static array2d<float> EdiffKernel(EdiffAlgorithm algorithm);
    static array2d<float> NormalizeKernel(array2d<float> const &kernel);
    static array2d<float> GaussianKernel(vec2 radius,
                                         float angle = 0.f,
                                         vec2 delta = vec2(0.f, 0.f));

    /* Rendering */
    bool RenderRandom(ivec2 size);

    /* Resize and crop */
    Image Resize(ivec2 size, ResampleAlgorithm algorithm);
    Image Crop(ibox2 box) const;

    /* Image processing */
    Image AutoContrast() const;
    Image Brightness(float val) const;
    Image Contrast(float val) const;
    Image Convolution(array2d<float> const &kernel);
    Image Dilate();
    Image Erode();
    Image Invert() const;
    Image Median(ivec2 radii) const;
    Image Median(array2d<float> const &kernel) const;
    Image Sharpen(array2d<float> const &kernel);
    Image Threshold(float val) const;
    Image Threshold(vec3 val) const;
    Image RGBToYUV() const;
    Image YUVToRGB() const;

    /* Dithering */
    Image DitherRandom() const;
    Image DitherEdiff(array2d<float> const &kernel,
                      ScanMode scan = ScanMode::Raster) const;
    Image DitherOstromoukhov(ScanMode scan = ScanMode::Raster) const;
    Image DitherOrdered(array2d<float> const &kernel) const;
    Image DitherHalftone(float radius, float angle) const;
    Image DitherDbs() const;

    /* Combine images */
    static Image Merge(Image &src1, Image &src2, float alpha);
    static Image Mean(Image &src1, Image &src2);
    static Image Min(Image &src1, Image &src2);
    static Image Max(Image &src1, Image &src2);
    static Image Overlay(Image &src1, Image &src2);
    static Image Screen(Image &src1, Image &src2);
    static Image Multiply(Image &src1, Image &src2);
    static Image Divide(Image &src1, Image &src2);
    static Image Add(Image &src1, Image &src2);
    static Image Sub(Image &src1, Image &src2);
    static Image Difference(Image &src1, Image &src2);

private:
    class ImageData *m_data;
};

} /* namespace lol */

#endif // __LOL_IMAGE_IMAGE_H__

