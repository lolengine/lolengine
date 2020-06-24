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
// The old image class
// -------------------
//

#include <../legacy/lol/math/arraynd.h>
#include <lol/vector>
#include <../legacy/lol/image/pixel.h>
#include <../legacy/lol/math/geometry.h> // ibox2

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

// old_image -----------------------------------------------------------------------
class old_image
{
public:
    old_image();
    old_image(ivec2 size);
    /* XXX: use of this ctor should be discouraged, as it will not
     * return information about a possible error. */
    old_image(std::string const &path);

    /* Rule of three */
    old_image(old_image const &other);
    old_image & operator =(old_image other);
    ~old_image();

    void DummyFill();
    void Copy(uint8_t* pixels, ivec2 const& size, PixelFormat fmt);
    void Copy(old_image const &other);
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
    [[nodiscard]] inline old_array2d<typename PixelType<T>::type> &lock2d()
    {
        /* Hack: this indirection is needed because of a Visual Studio ICE */
        return *(old_array2d<typename PixelType<T>::type> *)lock2d_helper(T);
    }

    template<typename T>
    void unlock2d(old_array2d<T> const &);

    /* Image processing kernels */
    struct kernel
    {
        kernel() = delete;

        static old_array2d<float> normalize(old_array2d<float> const &kernel);

        static old_array2d<float> bayer(ivec2 size);
        static old_array2d<float> halftone(ivec2 size);
        static old_array2d<float> blue_noise(ivec2 size,
                                         ivec2 gsize = ivec2(7, 7));
        static old_array2d<float> ediff(EdiffAlgorithm algorithm);
        static old_array2d<float> gaussian(vec2 radius,
                                       float angle = 0.f,
                                       vec2 delta = vec2(0.f, 0.f));
    };

    /* Rendering */
    bool RenderRandom(ivec2 size);

    /* Resize and crop */
    old_image Resize(ivec2 size, ResampleAlgorithm algorithm);
    old_image Crop(ibox2 box) const;

    /* Image processing */
    old_image AutoContrast() const;
    old_image Brightness(float val) const;
    old_image Contrast(float val) const;
    old_image Convolution(old_array2d<float> const &kernel);
    old_image Dilate();
    old_image Erode();
    old_image Invert() const;
    old_image Median(ivec2 radii) const;
    old_image Median(old_array2d<float> const &kernel) const;
    old_image Sharpen(old_array2d<float> const &kernel);
    old_image Threshold(float val) const;
    old_image Threshold(vec3 val) const;
    old_image RGBToYUV() const;
    old_image YUVToRGB() const;

    /* Dithering */
    old_image dither_random() const;
    old_image dither_ediff(old_array2d<float> const &kernel,
                           ScanMode scan = ScanMode::Raster) const;
    old_image dither_ostromoukhov(ScanMode scan = ScanMode::Raster) const;
    old_image dither_ordered(old_array2d<float> const &kernel) const;
    old_image dither_halftone(float radius, float angle) const;
    old_image dither_dbs() const;

    /* Combine images */
    static old_image Merge(old_image &src1, old_image &src2, float alpha);
    static old_image Mean(old_image &src1, old_image &src2);
    static old_image Min(old_image &src1, old_image &src2);
    static old_image Max(old_image &src1, old_image &src2);
    static old_image Overlay(old_image &src1, old_image &src2);
    static old_image Screen(old_image &src1, old_image &src2);
    static old_image Multiply(old_image &src1, old_image &src2);
    static old_image Divide(old_image &src1, old_image &src2);
    static old_image Add(old_image &src1, old_image &src2);
    static old_image Sub(old_image &src1, old_image &src2);
    static old_image Difference(old_image &src1, old_image &src2);

private:
    void *lock2d_helper(PixelFormat T);

    class image_data *m_data;
};

} /* namespace lol */

