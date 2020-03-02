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
// The image class
// ---------------
//

#include <lol/math/arraynd.h>
#include <lol/math/vector.h>
#include <lol/math/geometry.h>
#include <lol/image/pixel.h>

#include <string>

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

// image -----------------------------------------------------------------------
class image
{
public:
    image();
    image(ivec2 size);
    /* XXX: use of this ctor should be discouraged, as it will not
     * return information about a possible error. */
    image(std::string const &path);

    /* Rule of three */
    image(image const &other);
    image & operator =(image other);
    ~image();

    void DummyFill();
    void Copy(uint8_t* pixels, ivec2 const& size, PixelFormat fmt);
    void Copy(image const &other);
    bool load(std::string const &path);
    bool save(std::string const &path);

    /* Low level access */
    ivec2 size() const;
    void resize(ivec2);

    PixelFormat format() const;
    void set_format(PixelFormat fmt);

    WrapMode GetWrapX() const;
    WrapMode GetWrapY() const;
    void SetWrap(WrapMode wrap_x, WrapMode wrap_y);

    /* Lock continuous arrays of pixels for writing */
    template<PixelFormat T> [[nodiscard]] typename PixelType<T>::type *lock();
    [[nodiscard]] void *lock();
    void unlock(void const *pixels);

    /* Lock 2D arrays of pixels for writing */
    template<PixelFormat T>
    [[nodiscard]] inline array2d<typename PixelType<T>::type> &lock2d()
    {
        /* Hack: this indirection is needed because of a Visual Studio ICE */
        return *(array2d<typename PixelType<T>::type> *)lock2d_helper(T);
    }

    template<typename T>
    void unlock2d(array2d<T> const &);

    /* Image processing kernels */
    struct kernel
    {
        kernel() = delete;

        static array2d<float> normalize(array2d<float> const &kernel);

        static array2d<float> bayer(ivec2 size);
        static array2d<float> halftone(ivec2 size);
        static array2d<float> blue_noise(ivec2 size,
                                         ivec2 gsize = ivec2(7, 7));
        static array2d<float> ediff(EdiffAlgorithm algorithm);
        static array2d<float> gaussian(vec2 radius,
                                       float angle = 0.f,
                                       vec2 delta = vec2(0.f, 0.f));
    };

    /* Rendering */
    bool RenderRandom(ivec2 size);

    /* Resize and crop */
    image Resize(ivec2 size, ResampleAlgorithm algorithm);
    image Crop(ibox2 box) const;

    /* Image processing */
    image AutoContrast() const;
    image Brightness(float val) const;
    image Contrast(float val) const;
    image Convolution(array2d<float> const &kernel);
    image Dilate();
    image Erode();
    image Invert() const;
    image Median(ivec2 radii) const;
    image Median(array2d<float> const &kernel) const;
    image Sharpen(array2d<float> const &kernel);
    image Threshold(float val) const;
    image Threshold(vec3 val) const;
    image RGBToYUV() const;
    image YUVToRGB() const;

    /* Dithering */
    image dither_random() const;
    image dither_ediff(array2d<float> const &kernel,
                       ScanMode scan = ScanMode::Raster) const;
    image dither_ostromoukhov(ScanMode scan = ScanMode::Raster) const;
    image dither_ordered(array2d<float> const &kernel) const;
    image dither_halftone(float radius, float angle) const;
    image dither_dbs() const;

    /* Combine images */
    static image Merge(image &src1, image &src2, float alpha);
    static image Mean(image &src1, image &src2);
    static image Min(image &src1, image &src2);
    static image Max(image &src1, image &src2);
    static image Overlay(image &src1, image &src2);
    static image Screen(image &src1, image &src2);
    static image Multiply(image &src1, image &src2);
    static image Divide(image &src1, image &src2);
    static image Add(image &src1, image &src2);
    static image Sub(image &src1, image &src2);
    static image Difference(image &src1, image &src2);

private:
    void *lock2d_helper(PixelFormat T);

    class image_data *m_data;
};

} /* namespace lol */

