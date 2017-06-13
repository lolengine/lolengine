//
//  Lol Engine
//
//  Copyright © 2010—2017 Sam Hocevar <sam@hocevar.net>
//              2014 Benjamin Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "../../image/resource-private.h"

namespace lol
{

/*
 * Image implementation class
 */

class ZedImageCodec : public ResourceCodec
{
public:
    virtual char const *GetName() { return "<ZedImageCodec>"; }
    virtual ResourceCodecData* Load(char const *path);
    virtual bool Save(char const *path, ResourceCodecData* data);
};

DECLARE_IMAGE_CODEC(ZedImageCodec, 10)

/*
 * Public Image class
 */

ResourceCodecData* ZedImageCodec::Load(char const *path)
{
    if (!lol::String(path).ends_with(".RSC"))
        return nullptr;

    // Compacter definition
    struct CompactSecondary
    {
        CompactSecondary(int32_t size) { m_size = size; }
        int32_t m_size;
        array<int32_t> m_tiles;
    };
    struct CompactMain
    {
        CompactMain(int32_t size) { m_size = size; m_count = 0; }
        int32_t                 m_size;
        int32_t                 m_count;
        array<CompactSecondary> m_secondary;
    };
    struct Compacter2d
    {
        void PowSetup(int32_t start, int32_t count)
        {
            for (int i = 0; i < count; i++)
            {
                m_primary << CompactMain(start << i);
                for (int j = 0; j < count; j++)
                    m_primary.last().m_secondary << CompactSecondary(start << j);
            }
        }
        void StepSetup(int32_t start, int32_t interval, int32_t count)
        {
            for (int i = 0; i < count; i++)
            {
                m_primary << CompactMain(start + interval * i);
                for (int j = 0; j < count; j++)
                    m_primary.last().m_secondary << CompactSecondary(start + interval * j);
            }
        }
        void CustomSetup(array<int32_t> custom_list)
        {
            for (int i = 0; i < custom_list.count(); i++)
            {
                m_primary << CompactMain(custom_list[i]);
                for (int j = 0; j < custom_list.count(); j++)
                    m_primary.last().m_secondary << CompactSecondary(custom_list[j]);
            }
        }
        void Store(int32_t tile, ivec2 size)
        {
            for (int i = 0; i < m_primary.count(); i++)
            {
                if (size.y <= m_primary[i].m_size || i == m_primary.count() - 1)
                {
                    for (int j = 0; j < m_primary[i].m_secondary.count(); j++)
                    {
                        if (size.x <= m_primary[i].m_secondary[j].m_size || j == m_primary[i].m_secondary.count() - 1)
                        {
                            m_primary[i].m_secondary[j].m_tiles << tile;
                            m_primary[i].m_count++;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        array<CompactMain>      m_primary;
    };

    File file;
    file.Open(path, FileAccess::Read, true);

    //Put file in memory
    long file_size = file.size();
    array<uint8_t> file_buffer;
    file_buffer.resize(file_size);
    file.Read((uint8_t*)&file_buffer[0], file_size);
    file.Close();

    //Get FileCount
    uint32_t file_pos = 0;
    uint16_t file_count = 0;
    file_count = *((uint16_t*)(&file_buffer[file_pos]));
    file_pos += sizeof(uint16_t);

    array<uint32_t> file_offset;
    file_offset.resize(file_count);
    //Get all the file offsets
    for (int i = 0; i < file_count; i++)
    {
        file_offset[i] = *((uint32_t*)(&file_buffer[file_pos]));
        file_pos += sizeof(uint32_t);
    }
    file_offset << file_size;

    //<Pos, Size>
    array<ivec2, ivec2> tiles;
    tiles.reserve(file_count);

    Compacter2d compacter;
    compacter.StepSetup(8, 8, 10);

    uint32_t total_size = 0;
    array<uint8_t> file_convert;
    file_convert.reserve(file_size);
    array<ivec2> available_sizes;
    //got through all the files and store them
    for (int i = 0; i < file_count; i++)
    {
        file_pos = file_offset[i];

        //Get image size
        uint8_t size_x = 0;
        uint8_t size_y = 0;
        size_y = file_buffer[file_pos++];
        size_x = file_buffer[file_pos++];

        //special tweak
        size_x *= 8;
        total_size += (size_x * size_y);

        //Prepare read
        uint32_t header_length = (size_y + 5) & 0xFC;
        uint32_t data_length = (file_offset[i+1] - file_offset[i]) - header_length;
        uint32_t data_pos = file_offset[i] + header_length;

        /* Seems useless in the end
        //Retrieve Header & footer
        array<uint8_t> header_data;
        header_data.resize(header_length);
        memcpy(&header_data[0], &file_buffer[file_offset[i]], header_length);
        array<uint8_t> footer_data;
        uint32_t footer_length = lol::min((uint32_t)file_buffer.count(), data_pos + data_length + header_length) - (data_pos + data_length);
        if (footer_length > 0)
        {
            footer_data.resize(footer_length);
            memcpy(&footer_data[0], &file_buffer[data_pos + data_length], footer_length);
        }
        */

        //Prepare buffer and tiles infos
        int32_t convert_pos = file_convert.count();
        ivec2 size = ivec2(size_x, size_y);
        //store tile in compacter
        compacter.Store(tiles.count(), ivec2(size_x, size_y));
        //push tile on the stack
        tiles.push(ivec2(file_convert.count(), data_length), ivec2(size_x, size_y));
        file_convert.resize(convert_pos + data_length);

        //Retrieve actual datas
        file_pos = data_pos;
        memcpy(&file_convert[convert_pos], &file_buffer[file_pos], data_length);
        file_pos += data_length;

        //Store size type
        {
            ivec2 size_16 = size;
            int32_t s_16 = 8;
            while (1)
            {
                if (size_16.x < s_16)
                {
                    size_16.x = s_16;
                    break;
                }
                s_16 <<= 1;
            }
            s_16 = 8;
            while (1)
            {
                if (size_16.y < s_16)
                {
                    size_16.y = s_16;
                    break;
                }
                s_16 <<= 1;
            }
            int j = 0;
            for (; j < available_sizes.count(); j++)
                if (available_sizes[j] == size_16)
                    break;
            if (j >= available_sizes.count())
                available_sizes << size_16;
        }
    }

    int32_t tex_sqrt = (int32_t)lol::sqrt((float)total_size);
    int32_t tex_size = 2;
    while (tex_size < tex_sqrt)
        tex_size <<= 1;

    //Prepare final image
    auto data = new ResourceTilesetData(new image(ivec2(tex_size)));
    auto image = data->m_image;
    uint8_t *pixels = image->lock<PixelFormat::Y_8>();

    //Data refactor stage
    ivec2 pos = ivec2(0);
    for (int j = compacter.m_primary.count() - 1; j >= 0; j--)
    {
        for (int k = compacter.m_primary[j].m_secondary.count() - 1; k >= 0; k--)
        {
            //Try something smaller
            if (pos.x + compacter.m_primary[j].m_secondary[k].m_size >= tex_size)
                continue;

            while (compacter.m_primary[j].m_secondary[k].m_tiles.count() > 0)
            {
                //Try something smaller
                if (pos.x + compacter.m_primary[j].m_secondary[k].m_size >= tex_size)
                    break;

                compacter.m_primary[j].m_count--;
                int i = compacter.m_primary[j].m_secondary[k].m_tiles.pop();
                int32_t file_off = tiles[i].m1[0];
                ivec2 t_size = tiles[i].m2;

                ASSERT(pos.y + t_size.y < tex_size);

                //Move image to texture
                int32_t img_off = pos.x + pos.y * tex_size;

                //At this stage image data consists of 4 vertical interlaced blocks
                for (int pass = 0; pass < 4; pass++)
                {
                    for (int y_cur = 0; y_cur < t_size.y; y_cur++)
                    {
                        for (int x_cur = 0; x_cur < t_size.x / 4; x_cur++)
                        {
                            int32_t img_pos = img_off + pass + 4 * x_cur + y_cur * (int32_t)tex_size;
                            pixels[img_pos] = file_convert[file_off++];
                        }
                    }
                }

                //Register new pos and move to next
                tiles[i].m1 = pos;
                pos.x += t_size.x;
            }
        }

        //Do another loop
        if (compacter.m_primary[j].m_count > 0)
        {
            pos.x = 0;
            pos.y += compacter.m_primary[j].m_size;
            j++;
        }
    }
    image->unlock(pixels);

    data->m_tiles = tiles;

    return data;
}

bool ZedImageCodec::Save(char const *path, ResourceCodecData* data)
{
    UNUSED(path, data);
    /* FIXME: do we need to implement this? */
    return true;
}

} /* namespace lol */

