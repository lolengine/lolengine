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
        Array2D<typename PixelType<T>::type> &Lock2D();
    template<typename T>
        void Unlock2D(Array2D<T> const &);

    bool RetrieveTiles(Array<ivec2, ivec2>& tiles) const;

    /* Image processing kernels */
    static Array2D<float> BayerKernel(ivec2 size);
    static Array2D<float> HalftoneKernel(ivec2 size);
    static Array2D<float> EdiffKernel(EdiffAlgorithm algorithm);
    static Array2D<float> NormalizeKernel(Array2D<float> const &kernel);
    static Array2D<float> GaussianKernel(vec2 radius,
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
    Image Convolution(Array2D<float> const &kernel);
    Image Dilate();
    Image Erode();
    Image Invert() const;
    Image Median(ivec2 radii) const;
    Image Median(Array2D<float> const &kernel) const;
    Image Sharpen(Array2D<float> const &kernel);
    Image Threshold(float val) const;
    Image Threshold(vec3 val) const;
    Image RGBToYUV() const;
    Image YUVToRGB() const;

    /* Dithering */
    Image DitherRandom() const;
    Image DitherEdiff(Array2D<float> const &kernel,
                      ScanMode scan = ScanMode::Raster) const;
    Image DitherOstromoukhov(ScanMode scan = ScanMode::Raster) const;
    Image DitherOrdered(Array2D<float> const &kernel) const;
    Image DitherHalftone(float radius, float angle) const;
    Image DitherDbs() const;

private:
    class ImageData *m_data;
};

} /* namespace lol */

#endif // __LOL_IMAGE_IMAGE_H__

