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

namespace lol
{

/*
 * Hardcoded constants for colours
 */
template<typename T> color_t<T> const color_t<T>::black      (0, 0, 0, 1);
template<typename T> color_t<T> const color_t<T>::red        (1, 0, 0, 1);
template<typename T> color_t<T> const color_t<T>::green      (0, 1, 0, 1);
template<typename T> color_t<T> const color_t<T>::yellow     (1, 1, 0, 1);
template<typename T> color_t<T> const color_t<T>::blue       (0, 0, 1, 1);
template<typename T> color_t<T> const color_t<T>::magenta    (1, 0, 1, 1);
template<typename T> color_t<T> const color_t<T>::cyan       (0, 1, 1, 1);
template<typename T> color_t<T> const color_t<T>::white      (1, 1, 1, 1);
template<typename T> color_t<T> const color_t<T>::dark_gray  (.25f, .25f, .25f, 1);
template<typename T> color_t<T> const color_t<T>::gray       (.5f, .5f, .5f, 1);
template<typename T> color_t<T> const color_t<T>::light_gray (.75f, .75f, .75f, 1);

/*
* Convert uint color to vec4.
*/
template<typename T>
vec_t<T,4> color_t<T>::from_rgba32(uint32_t c)
{
    ivec4 v(c >> 24, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff);
    return vec_t<T,4>(v) * (1.f / 255.f);
}
template<typename T>
vec_t<T,4> color_t<T>::from_rgb32(uint32_t c)
{
    return color_t<T>::from_rgba32((c << 8) | 0x000000ffu);
}
template<typename T>
vec_t<T,4> color_t<T>::from_rgba16(uint16_t c)
{
    return color_t<T>::from_rgba32
        ( 0x11000000u * (c >> 12)
        | 0x00110000u * ((c >> 8) & 0xf)
        | 0x00001100u * ((c >> 4) & 0xf)
        | 0x00000011u * (c & 0xf));
}
template<typename T>
vec_t<T,4> color_t<T>::from_rgb16(uint16_t c)
{
    return color_t<T>::from_rgba16((c << 4) | 0xfu);
}

/*
* Convert uint color to vec4.
*/
template<typename T>
uint32_t color_t<T>::to_rgba32(vec_t<T,4> c)
{
    ivec4 v(c * 0xff);
    return (uint32_t)((v.r << 24) | (v.g << 16) | (v.b << 8) | v.a);
}
template<typename T>
uint32_t color_t<T>::to_rgb32(vec_t<T,4> c)
{
    ivec4 v(c * 0xff);
    return (uint32_t)((v.r << 16) | (v.g << 8) | v.b);
}
template<typename T>
uint16_t color_t<T>::to_rgba16(vec_t<T,4> c)
{
    ivec4 v(c * 0xf);
    return (uint16_t)((v.r << 12) | (v.g << 8) | (v.b << 4) | v.a);
}
template<typename T>
uint16_t color_t<T>::to_rgb16(vec_t<T,4> c)
{
    ivec4 v(c * 0xf);
    return (uint16_t)((v.r << 8) | (v.g << 4) | v.b);
}

} /* namespace lol */

