//
//  Lol Engine
//
//  Copyright © 2016—2017 Sam Hocevar <sam@hocevar.net>
//  Copyright © 2016—2017 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Resource class
// ---------------
//

#include <lol/math/arraynd.h>
#include <lol/math/vector.h>
#include <lol/math/geometry.h>
#include <lol/image/pixel.h>

namespace lol
{
    //ResourceCodecData -----------------------------------------------------------
    class ResourceCodecData
    {
    public:
        ResourceCodecData() { }
        virtual ~ResourceCodecData() { }
    };

    //ResourceImageData -----------------------------------------------------------
    class ResourceImageData : public ResourceCodecData
    {
    public:
        ResourceImageData(Image* image)
        {
            m_image = image;
        }

        virtual ~ResourceImageData()
        {
            if (m_image)
                delete m_image;
        }

        Image* m_image = nullptr;
    };

    //ResourceImageData -----------------------------------------------------------
    class ResourceTilesetData : public ResourceImageData
    {
    public:
        ResourceTilesetData(Image* image)
            : ResourceImageData(image)
        { }

        array<ivec2, ivec2> m_tiles;
    };

    //ResourceLoader --------------------------------------------------------------
    class ResourceLoader
    {
    public:
        static ResourceCodecData* Load(char const *path);
        static bool Save(char const *path, ResourceCodecData* data);
    };

} /* namespace lol */

