//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
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

/* Image dimensions and recursion depth. DEPTH = 2 is a reasonable value,
 * DEPTH = 3 gives good quality, and higher values may improve the results
 * even more but at the cost of significantly longer computation times. */
#define WIDTH 240
#define DEPTH 2

/*
 * Image implementation class
 */

class OricImageCodec : public ImageCodec
{
public:
    virtual bool Load(Image *image, char const *path);
    virtual bool Save(Image *image, char const *path);

private:
    static String ReadScreen(char const *name);
    static void WriteScreen(Image &image, Array<uint8_t> &result);
};

DECLARE_IMAGE_CODEC(OricImageCodec, 100)

/*
 * Public Image class
 */

bool OricImageCodec::Load(Image *image, char const *path)
{
    static u8vec4 const pal[8] =
    {
        u8vec4(0x00, 0x00, 0x00, 0xff),
        u8vec4(0x00, 0x00, 0xff, 0xff),
        u8vec4(0x00, 0xff, 0x00, 0xff),
        u8vec4(0x00, 0xff, 0xff, 0xff),
        u8vec4(0xff, 0x00, 0x00, 0xff),
        u8vec4(0xff, 0x00, 0xff, 0xff),
        u8vec4(0xff, 0xff, 0x00, 0xff),
        u8vec4(0xff, 0xff, 0xff, 0xff),
    };

    String screen = ReadScreen(path);
    if (screen.Count() == 0)
        return false;

    image->SetSize(ivec2(WIDTH, screen.Count() * 6 / WIDTH));

    u8vec4 *pixels = image->Lock<PixelFormat::RGBA_8>();

    for (int y = 0; y < image->GetSize().y; y++)
    {
        u8vec2 bgfg(0, 7);

        for (int x = 0; x < 40; x++)
        {
            uint8_t c = screen[y * 40 + x];

            if (c & 0x40)
            {
                for (int i = 0; i < 6; i++)
                {
                    uint8_t id = (c & (1 << (5 - i))) ? bgfg[1] : bgfg[0];
                    int col = (c & 0x80) ? 7 - id : id;
                    pixels[y * WIDTH + x * 6 + i] = pal[col];
                }
            }
            else if ((c & 0x60) == 0x00)
            {
                if (c & 0x10)
                    bgfg[0] = c & 0x7;
                else
                    bgfg[1] = c & 0x7;

                int col = (c & 0x80) ? 7 - bgfg[0] : bgfg[0];

                for (int i = 0; i < 6; i++)
                    pixels[y * WIDTH + x * 6 + i] = pal[col];
            }
            /* else: invalid sequence */
        }
    }

    image->Unlock(pixels);

    return true;
}

bool OricImageCodec::Save(Image *image, char const *path)
{
    int len = strlen(path);
    if (len < 4 || path[len - 4] != '.'
        || toupper(path[len - 3]) != 'T'
        || toupper(path[len - 2]) != 'A'
        || toupper(path[len - 1]) != 'P')
        return false;

    Array<uint8_t> result;

    result << 0x16 << 0x16 << 0x16 << 0x16 << 0x24;
    result << 0 << 0xff << 0x80 << 0 << 0xbf << 0x3f << 0xa0 << 0;

    /* Add filename, except the last 4 characters */
    for (char const *name = path; name[4]; ++name)
        result << (uint8_t)name[0];
    result << 0;

    Image tmp;
    ivec2 size = image->GetSize();
    if (size.x != WIDTH)
    {
        size.y = (int)((float)size.y * WIDTH / size.x);
        size.x = WIDTH;
        tmp = image->Resize(size, ResampleAlgorithm::Bresenham);
        image = &tmp;
    }

    WriteScreen(*image, result);

    File f;
    f.Open(path, FileAccess::Write);
    f.Write(result.Data(), result.Bytes());
    f.Close();

    return true;
}

String OricImageCodec::ReadScreen(char const *name)
{
    File f;
    f.Open(name, FileAccess::Read);
    String data = f.ReadString();
    f.Close();

    /* Skip the sync bytes */
    if (data[0] != 0x16)
        return "";
    int header = 1;
    while (data[header] == 0x16)
        ++header;
    if (data[header] != 0x24)
        return "";
    ++header;

    /* Skip the header, ignoring the last byte’s value */
    if (data.Sub(header, 8) != String("\x00\xff\x80\x00\xbf\x3f\xa0\x00", 8))
        return "";

    /* Skip the file name, including trailing nul char */
    data = data.Sub(header + 8);
    int filename_end = data.IndexOf('\0');
    if (filename_end < 0)
            return "";

    /* Read screen data */
    return data.Sub(filename_end + 1);
}

/* Error diffusion table, similar to Floyd-Steinberg. I choose not to
 * propagate 100% of the error, because doing so creates awful artifacts
 * (full lines of the same colour, massive colour bleeding) for unclear
 * reasons. Atkinson dithering propagates 3/4 of the error, which is even
 * less than our 31/32. I also choose to propagate slightly more in the
 * X direction to avoid banding effects due to rounding errors.
 * It would be interesting, for future versions of this software, to
 * propagate the error to the second line, too. But right now I find it far
 * too complex to do.
 *
 *             +-------+-------+
 *             | error |FS0/FSX|
 *     +-------+-------+-------+
 *     |FS1/FSX|FS2/FSX|FS3/FSX|
 *     +-------+-------+-------+
 */
#define FS0 15
#define FS1 6
#define FS2 9
#define FS3 1
#define FSX 32

/* The simple Oric RGB palette, made of the 8 Neugebauer primary colours. Each
 * colour is repeated 6 times so that we can point to the palette to paste
 * whole blocks of 6 pixels. It’s also organised so that palette[7-x] is the
 * RGB negative of palette[x], and screen command X uses palette[X & 7]. */
#define V(a,b,c) ivec3(a,b,c),ivec3(a,b,c),ivec3(a,b,c),\
                 ivec3(a,b,c),ivec3(a,b,c),ivec3(a,b,c),
static const ivec3 palette[8][6] =
{
    { V(0x0000, 0x0000, 0x0000) },
    { V(0xffff, 0x0000, 0x0000) },
    { V(0x0000, 0xffff, 0x0000) },
    { V(0xffff, 0xffff, 0x0000) },
    { V(0x0000, 0x0000, 0xffff) },
    { V(0xffff, 0x0000, 0xffff) },
    { V(0x0000, 0xffff, 0xffff) },
    { V(0xffff, 0xffff, 0xffff) },
};
#undef V

/* Set new background and foreground colours according to the given command. */
static inline u8vec2 domove(uint8_t command, u8vec2 bgfg)
{
    if ((command & 0x78) == 0x00)
        bgfg[1] = command & 0x7;
    else if ((command & 0x78) == 0x10)
        bgfg[0] = command & 0x7;
    return bgfg;
}

/* Clamp pixel value to avoid colour bleeding. Deactivated because it
 * does not give satisfactory results. */
static inline ivec3 myclamp(ivec3 p)
{
#if 0
    static int const CLAMP = 0x1000;
    /* FIXME: doesn’t give terribly good results on eg. eatme.png */
    p = lol::clamp(p, -CLAMP, 0xffff + CLAMP);
#endif
    return p;
}

/* Compute the perceptual error caused by replacing the input pixels "in"
 * with the output pixels "out". "indelta" is the diffused error that should
 * be applied to "in"’s first pixel. "outdelta" will hold the diffused error
 * to apply after "in"’s last pixel upon next call. The return value does
 * not mean much physically; it is one part of the algorithm where you need
 * to play a bit in order to get appealing results. That’s how image
 * processing works, folks. */
static inline int geterror(ivec3 const *in, ivec3 indelta,
                           ivec3 const *out, ivec3 *outdelta)
{
    /* 9 cells: 1 for the end of line, 8 for the errors below */
    ivec3 tmpdelta[9] = { indelta, ivec3(0) };

    int ret = 0;

    /* Experiment shows that this is important at small depths */
    for (int i = 0; i < 6; i++)
    {
        ivec3 a = myclamp(in[i] + tmpdelta[0]);
        ivec3 b = out[i];

        tmpdelta[0] = (a - b) * FS0 / FSX;
        tmpdelta[i + 1] += (a - b) * FS1 / FSX;
        tmpdelta[i + 2] += (a - b) * FS2 / FSX;
        tmpdelta[i + 3] += (a - b) * FS3 / FSX;

        ret += dot((a - b) / 256, (a - b) / 256);
    }

    /* Experiment shows that this is important at large depths */
    for (int i = 0; i < 4; i++)
    {
        ivec3 a = (in[i] + in[i + 1] + in[i + 2]) / 3;
        ivec3 b = (out[i] + out[i + 1] + out[i + 2]) / 3;

        ret += dot((a - b) / 256, (a - b) / 256);
    }

    /* Using the diffused error as a perceptual error component is stupid,
     * because that’s not what it is at all, but I found that it helped a
     * bit in some cases. */
    ret += dot(tmpdelta[0] / 256, tmpdelta[0] / 256);

    *outdelta = tmpdelta[0];

    return ret;
}

static uint8_t bestmove(ivec3 const *in, u8vec2 bgfg,
                        ivec3 delta, int depth, int maxerror,
                        int *error, ivec3 *out)
{
    ivec3 tmprgb[6], bestrgb[6];
    ivec3 nop_rgb_delta, inop_rgb_delta;
    ivec3 const *rgb, *nop_rgb, *inop_rgb;
    int suberror, statice, nop_error, inop_error;

    /* Precompute error for the case where we change the foreground colour
     * and hence only print the background colour or its negative */
    nop_rgb = palette[bgfg[0]];
    nop_error = geterror(in, delta, nop_rgb, &nop_rgb_delta);
    inop_rgb = palette[7 - bgfg[0]];
    inop_error = geterror(in, delta, inop_rgb, &inop_rgb_delta);

    /* Precompute sub-error for the case where we print pixels (and hence
     * don’t change the palette). It’s not the exact error because we should
     * be propagating the error to the first pixel here. */
    if (depth > 0)
    {
        bestmove(in + 6, bgfg, ivec3(0), depth - 1,
                 maxerror, &statice, nullptr);
    }

    /* Check every likely command:
     * 0-7: change foreground to 0-7
     * 8-15: change foreground to 0-7, print negative background
     * 16-23: change background to 0-7
     * 24-31: change background to 0-7, print negative background
     * 32: normal stuff
     * 33: inverse video stuff */
    static uint8_t const command_list[] =
    {
        0x00, 0x04, 0x01, 0x05, 0x02, 0x06, 0x03, 0x07,
        0x80, 0x84, 0x81, 0x85, 0x82, 0x86, 0x83, 0x87,
        0x10, 0x14, 0x11, 0x15, 0x12, 0x16, 0x13, 0x17,
        0x90, 0x94, 0x91, 0x95, 0x92, 0x96, 0x93, 0x97,
        0x40, 0xc0
    };

    int besterror = 0x7ffffff;
    uint8_t bestcommand = 0x10;
    memcpy(bestrgb, nop_rgb, sizeof(bestrgb));

    for (uint8_t command : command_list)
    {
        ivec3 nexterr = ivec3(0);
        int curerror = 0;

        u8vec2 newbgfg = domove(command, bgfg);

        /* Keeping bg and fg is useless, because we could use standard
         * pixel printing instead */
        if ((command & 0x40) == 0x00 && newbgfg == bgfg)
            continue;

        /* I *think* having newfg == newbg is useless, too, but I don’t
         * want to miss some corner case where swapping bg and fg may be
         * interesting, so we continue anyway. */

#if 0
        /* Bit 6 off and bit 5 on seems illegal */
        if ((command & 0x60) == 0x20)
            continue;

        /* Bits 6 and 5 off and bit 3 on seems illegal */
        if ((command & 0x68) == 0x08)
            continue;
#endif

        if ((command & 0xf8) == 0x00)
        {
            curerror = nop_error;
            rgb = nop_rgb;
            nexterr = nop_rgb_delta;
        }
        else if ((command & 0xf8) == 0x80)
        {
            curerror = inop_error;
            rgb = inop_rgb;
            nexterr = inop_rgb_delta;
        }
        else if ((command & 0xf8) == 0x10)
        {
            rgb = palette[newbgfg[0]];
            curerror = geterror(in, delta, rgb, &nexterr);
        }
        else if ((command & 0xf8) == 0x90)
        {
            rgb = palette[7 - newbgfg[0]];
            curerror = geterror(in, delta, rgb, &nexterr);
        }
        else
        {
            ivec3 bgcolor, fgcolor;

            if ((command & 0x80) == 0x00)
            {
                bgcolor = palette[bgfg[0]][0];
                fgcolor = palette[bgfg[1]][0];
            }
            else
            {
                bgcolor = palette[7 - bgfg[0]][0];
                fgcolor = palette[7 - bgfg[1]][0];
            }

            ivec3 tmpvec = delta;

            for (int i = 0; i < 6; i++)
            {
                ivec3 delta1 = myclamp(in[i] + tmpvec) - bgcolor;
                ivec3 delta2 = myclamp(in[i] + tmpvec) - fgcolor;

                if (dot(delta1 / 256, delta1) < dot(delta2 / 256, delta2))
                {
                    tmpvec = delta1 * FS0 / FSX;
                    tmprgb[i] = bgcolor;
                }
                else
                {
                    tmpvec = delta2 * FS0 / FSX;
                    tmprgb[i] = fgcolor;
                    command |= (1 << (5 - i));
                }
            }

            /* Recompute full error */
            curerror += geterror(in, delta, tmprgb, &nexterr);

            rgb = tmprgb;
        }

        if (curerror > besterror)
            continue;

        /* Try to avoid bad decisions now that will have a high cost
         * later in the line by making the next error more important than
         * the current error. */
        curerror = curerror * 3 / 4;

        if (depth == 0)
            suberror = 0; /* It’s the end of the tree */
        else if ((command & 0x68) == 0x00)
        {
            bestmove(in + 6, newbgfg, nexterr, depth - 1,
                     besterror - curerror, &suberror, nullptr);

#if 0
            /* Slight penalty for colour changes; they're hard to revert. The
             * value of 2 was determined empirically. 1.5 is not enough and
             * 3 is too much. */
            if (newbgfg[0] != bgfg[0])
                suberror = suberror * 10 / 8;
            else if (newbgfg[1] != bgfg[1])
                suberror = suberror * 9 / 8;
#endif
        }
        else
            suberror = statice;

        if (curerror + suberror < besterror)
        {
            besterror = curerror + suberror;
            bestcommand = command;
            memcpy(bestrgb, rgb, sizeof(bestrgb));
        }
    }

    *error = besterror;
    if (out)
        memcpy(out, bestrgb, sizeof(bestrgb));

    return bestcommand;
}

void OricImageCodec::WriteScreen(Image &image, Array<uint8_t> &result)
{
    ivec2 size = image.GetSize();
    vec4 *pixels = image.Lock<PixelFormat::RGBA_F32>();

    int stride = (size.x + 1);

    array2d<ivec3> src, dst;
    src.SetSize(size + ivec2(1));
    dst.SetSize(size + ivec2(1));

    memset(src.Data(), 0, src.Bytes());
    memset(dst.Data(), 0, dst.Bytes());

    /* Import pixels into our custom format */
    for (int y = 0; y < size.y; y++)
        for (int x = 0; x < size.x; x++)
            for (int c = 0; c < 3; c++)
                src[x][y][c] = 0xffff * pixels[y * size.x + x][2 - c];

    /* Let the fun begin */
    for (int y = 0; y < size.y; y++)
    {
        u8vec2 bgfg(0, 7);

        //fprintf(stderr, "\rProcessing... %i%%", (y * 100 + 99) / size.y);

        for (int x = 0; x < size.x; x += 6)
        {
            int depth = (x + DEPTH < size.x) ? DEPTH : (size.x - x) / 6 - 1;
            ivec3 *srcl = &src[x][y];
            ivec3 *dstl = &dst[x][y];

            /* Recursively compute and apply best command */
            int dummy;
            uint8_t command = bestmove(srcl, bgfg, vec3(0), depth,
                                       0x7fffff, &dummy, dstl);
            /* Propagate error */
            for (int i = 0; i < 6; i++)
            {
                ivec3 delta = srcl[i] - dstl[i];
                srcl[i + 1] = myclamp(srcl[i + 1] + delta * FS0 / FSX);
                srcl[i + stride - 1] += delta * FS1 / FSX;
                srcl[i + stride] += delta * FS2 / FSX;
                srcl[i + stride + 1] += delta * FS3 / FSX;
            }

            for (int i = -1; i < 7; i++)
                srcl[i + stride] = myclamp(srcl[i + stride]);

            /* Iterate */
            bgfg = domove(command, bgfg);
            /* Write byte to file */
            result << command;
        }
    }

    image.Unlock(pixels);

    //fprintf(stderr, " done.\n");
}

} /* namespace lol */

