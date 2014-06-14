//
// Lol Engine
//
// Copyright: (c) 2014 Benjamin Huet <huet.benjamin@gmail.com>
//                2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "../../image/image-private.h"

using namespace std;

namespace lol
{

/*
 * Image implementation class
 */

DECLARE_IMAGE_LOADER(ZedImageData, 0)
{
public:
    virtual bool Open(char const *);
    virtual bool Save(char const *);
    virtual bool Close();

    virtual uint8_t *GetData() const;
    virtual bool RetrieveTiles(Array<ivec2, ivec2>& tiles)
    {
        bool result = m_tiles.Count() > 0;
        tiles += m_tiles;
        m_tiles.Empty();
        return result;
    }

private:
    uint8_t *m_pixels;
    //<Pos, Size>
    Array<ivec2, ivec2> m_tiles;
};

/*
 * Public Image class
 */

bool ZedImageData::Open(char const *path)
{
    if (!lol::String(path).EndsWith(".RSC"))
        return false;

    // Compacter definition
    struct CompactSecondary
    {
        CompactSecondary(int32_t size) { m_size = size; }
        int32_t                 m_size;
        Array<int32_t>          m_tiles;
    };
    struct CompactMain
    {
        CompactMain(int32_t size) { m_size = size; m_count = 0; }
        int32_t                 m_size;
        int32_t                 m_count;
        Array<CompactSecondary> m_secondary;
    };
    struct Compacter2d
    {
        void PowSetup(int32_t start, int32_t count)
        {
            for (int i = 0; i < count; i++)
            {
                m_primary << CompactMain(start << i);
                for (int j = 0; j < count; j++)
                    m_primary.Last().m_secondary << CompactSecondary(start << j);
            }
        }
        void StepSetup(int32_t start, int32_t interval, int32_t count)
        {
            for (int i = 0; i < count; i++)
            {
                m_primary << CompactMain(start + interval * i);
                for (int j = 0; j < count; j++)
                    m_primary.Last().m_secondary << CompactSecondary(start + interval * j);
            }
        }
        void CustomSetup(Array<int32_t> custom_list)
        {
            for (int i = 0; i < custom_list.Count(); i++)
            {
                m_primary << CompactMain(custom_list[i]);
                for (int j = 0; j < custom_list.Count(); j++)
                    m_primary.Last().m_secondary << CompactSecondary(custom_list[j]);
            }
        }
        void Store(int32_t tile, ivec2 size)
        {
            for (int i = 0; i < m_primary.Count(); i++)
            {
                if (size.y <= m_primary[i].m_size || i == m_primary.Count() - 1)
                {
                    for (int j = 0; j < m_primary[i].m_secondary.Count(); j++)
                    {
                        if (size.x <= m_primary[i].m_secondary[j].m_size || j == m_primary[i].m_secondary.Count() - 1)
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
        Array<CompactMain>      m_primary;
    };

    File file;
    file.Open(path, FileAccess::Read, true);

    //Put file in memory
    long file_size = file.GetSize();
    Array<uint8_t> file_buffer;
    file_buffer.Resize(file_size);
    file.Read((uint8_t*)&file_buffer[0], file_size);
    file.Close();

    //Get FileCount
    uint32_t file_pos = 0;
    uint16_t file_count = 0;
    file_count = *((uint16_t*)(&file_buffer[file_pos]));
    file_pos += sizeof(uint16_t);

    Array<long int> file_offset;
    file_offset.Resize(file_count);
    //Get all the file offsets
    for (int i = 0; i < file_count; i++)
    {
        file_offset[i] = *((long int*)(&file_buffer[file_pos]));
        file_pos += sizeof(long int);
    }
    file_offset << file_size;

    m_tiles.Reserve(file_count);

    Compacter2d compacter;
    compacter.StepSetup(8, 8, 10);

    uint32_t total_size = 0;
    Array<uint8_t> file_convert;
    file_convert.Reserve(file_size);
    Array<ivec2> available_sizes;
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
        Array<uint8_t> header_data;
        header_data.Resize(header_length);
        memcpy(&header_data[0], &file_buffer[file_offset[i]], header_length);
        Array<uint8_t> footer_data;
        uint32_t footer_length = lol::min((uint32_t)file_buffer.Count(), data_pos + data_length + header_length) - (data_pos + data_length);
        if (footer_length > 0)
        {
            footer_data.Resize(footer_length);
            memcpy(&footer_data[0], &file_buffer[data_pos + data_length], footer_length);
        }
        */

        //Prepare buffer and tiles infos
        int32_t convert_pos = file_convert.Count();
        ivec2 size = ivec2(size_x, size_y);
        compacter.Store(m_tiles.Count(), ivec2(size_x, size_y));
        m_tiles.Push(ivec2(file_convert.Count(), data_length), ivec2(size_x, size_y));
        file_convert.Resize(convert_pos + data_length);

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
            for (; j < available_sizes.Count(); j++)
                if (available_sizes[j] == size_16)
                    break;
            if (j >= available_sizes.Count())
                available_sizes << size_16;
        }
    }

    int32_t tex_sqrt = (int32_t)lol::sqrt((float)total_size);
    int32_t tex_size = 2;
    while (tex_size < tex_sqrt)
        tex_size <<= 1;

    //Prepare final imqge
    m_size = ivec2(tex_size);
    m_format = PixelFormat::Y_8;
    m_pixels = new uint8_t[tex_size * tex_size * 1 * sizeof(*m_pixels)];
    uint8_t *image = m_pixels;

    //Data refactor stage
    ivec2 pos = ivec2(0);
    for (int j = compacter.m_primary.Count() - 1; j >= 0; j--)
    {
        for (int k = compacter.m_primary[j].m_secondary.Count() - 1; k >= 0; k--)
        {
            //Try something smaller
            if (pos.x + compacter.m_primary[j].m_secondary[k].m_size >= tex_size)
                continue;

            while (compacter.m_primary[j].m_secondary[k].m_tiles.Count() > 0)
            {
                //Try something smaller
                if (pos.x + compacter.m_primary[j].m_secondary[k].m_size >= tex_size)
                    break;

                compacter.m_primary[j].m_count--;
                int i = compacter.m_primary[j].m_secondary[k].m_tiles.Pop();
                int32_t file_off = m_tiles[i].m1[0];
                ivec2 t_size = m_tiles[i].m2;

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
                            image[img_pos] = file_convert[file_off++];
                        }
                    }
                }

                //Register new pos and move to next
                m_tiles[i].m1 = pos;
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
    return true;
}

bool ZedImageData::Save(char const *path)
{
    UNUSED(path);
    /* FIXME: do we need to implement this? */
    return true;
}

bool ZedImageData::Close()
{
    delete[] m_pixels;

    return true;
}

uint8_t * ZedImageData::GetData() const
{
    return m_pixels;
}

} /* namespace lol */

