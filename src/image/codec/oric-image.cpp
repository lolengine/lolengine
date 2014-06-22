//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
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
#define HEIGHT 200
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
        int bg = 0, fg = 7;

        for (int x = 0; x < 40; x++)
        {
            int col;
            uint8_t c = screen[y * 40 + x];

            if (c & 0x40)
            {
                for (int i = 0; i < 6; i++)
                {
                    col = (c & (1 << (5 - i))) ? (c & 0x80) ? 7 - fg : fg
                                               : (c & 0x80) ? 7 - bg : bg;
                    pixels[y * WIDTH + x * 6 + i] = pal[col];
                }
            }
            else if ((c & 0x60) == 0x00)
            {
                if (c & 0x10)
                    bg = c & 0x7;
                else
                    fg = c & 0x7;

                col = (c & 0x80) ? 7 - bg : bg;

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
    return false;
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

#if 0

pipi_image_t *pipi_load_oric(char const *name)
{
}

int pipi_save_oric(pipi_image_t *img, char const *name)
{
    pipi_image_t *tmp = NULL;
    pipi_pixels_t *p;
    float *data;
    uint8_t *screen;
    FILE *fp;
    size_t len;

    len = strlen(name);
    if (len < 4 || name[len - 4] != '.'
        || toupper(name[len - 3]) != 'T'
        || toupper(name[len - 2]) != 'A'
        || toupper(name[len - 1]) != 'P')
        return -1;

    fp = fopen(name, "w");
    if (!fp)
        return -1;

    fwrite("\x16\x16\x16\x16\x24", 1, 5, fp);
    fwrite("\x00\xff\x80\x00\xbf\x3f\xa0\x00\x00", 1, 9, fp);
    fwrite(name, 1, len - 4, fp);
    fwrite("\x00", 1, 1, fp);

    if (img->w != WIDTH || img->h != HEIGHT)
        tmp = pipi_resize_bicubic(img, WIDTH, HEIGHT);
    else
        tmp = img;
    p = pipi_get_pixels(tmp, PIPI_PIXELS_RGBA_F32);
    data = p->pixels;
    screen = malloc(WIDTH * HEIGHT / 6);
    write_screen(data, screen);
    pipi_release_pixels(tmp, p);
    if (tmp != img)
        pipi_free(tmp);

    fwrite(screen, 1, WIDTH * HEIGHT / 6, fp);
    fclose(fp);

    free(screen);

    return 0;
}

/*
 * XXX: The following functions are local.
 */

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
#define o 0x0000
#define X 0xffff
static const int palette[8][6 * 3] =
{
    { o, o, o,   o, o, o,   o, o, o,   o, o, o,   o, o, o,   o, o, o },
    { X, o, o,   X, o, o,   X, o, o,   X, o, o,   X, o, o,   X, o, o },
    { o, X, o,   o, X, o,   o, X, o,   o, X, o,   o, X, o,   o, X, o },
    { X, X, o,   X, X, o,   X, X, o,   X, X, o,   X, X, o,   X, X, o },
    { o, o, X,   o, o, X,   o, o, X,   o, o, X,   o, o, X,   o, o, X },
    { X, o, X,   X, o, X,   X, o, X,   X, o, X,   X, o, X,   X, o, X },
    { o, X, X,   o, X, X,   o, X, X,   o, X, X,   o, X, X,   o, X, X },
    { X, X, X,   X, X, X,   X, X, X,   X, X, X,   X, X, X,   X, X, X },
};

/* Set new background and foreground colours according to the given command. */
static inline void domove(uint8_t command, uint8_t *bg, uint8_t *fg)
{
    if ((command & 0x78) == 0x00)
        *fg = command & 0x7;
    else if ((command & 0x78) == 0x10)
        *bg = command & 0x7;
}

/* Clamp pixel value to avoid colour bleeding. Deactivated because it
 * does not give satisfactory results. */
#define CLAMP 0x1000
static inline int clamp(int p)
{
#if 0
    /* FIXME: doesn’t give terribly good results on eg. eatme.png */
    if (p < - CLAMP) return - CLAMP;
    if (p > 0xffff + CLAMP) return 0xffff + CLAMP;
#endif
    return p;
}

/* Compute the perceptual error caused by replacing the input pixels "in"
 * with the output pixels "out". "inerr" is the diffused error that should
 * be applied to "in"’s first pixel. "outerr" will hold the diffused error
 * to apply after "in"’s last pixel upon next call. The return value does
 * not mean much physically; it is one part of the algorithm where you need
 * to play a bit in order to get appealing results. That’s how image
 * processing works, dude. */
static inline int geterror(int const *in, int const *inerr,
                           int const *out, int *outerr)
{
    int tmperr[9 * 3];
    int i, c, ret = 0;

    /* 9 cells: 1 for the end of line, 8 for the errors below */
    memcpy(tmperr, inerr, 3 * sizeof(int));
    memset(tmperr + 3, 0, 8 * 3 * sizeof(int));

    for (i = 0; i < 6; i++)
    {
        for (c = 0; c < 3; c++)
        {
            /* Experiment shows that this is important at small depths */
            int a = clamp(in[i * 3 + c] + tmperr[c]);
            int b = out[i * 3 + c];
            tmperr[c] = (a - b) * FS0 / FSX;
            tmperr[c + (i * 3 + 3)] += (a - b) * FS1 / FSX;
            tmperr[c + (i * 3 + 6)] += (a - b) * FS2 / FSX;
            tmperr[c + (i * 3 + 9)] += (a - b) * FS3 / FSX;
            ret += (a - b) / 256 * (a - b) / 256;
        }
    }

    for (i = 0; i < 4; i++)
    {
        for (c = 0; c < 3; c++)
        {
            /* Experiment shows that this is important at large depths */
            int a = ((in[i * 3 + c] + in[i * 3 + 3 + c]
                                    + in[i * 3 + 6 + c]) / 3);
            int b = ((out[i * 3 + c] + out[i * 3 + 3 + c]
                                     + out[i * 3 + 6 + c]) / 3);
            ret += (a - b) / 256 * (a - b) / 256;
        }
    }

    /* Using the diffused error as a perceptual error component is stupid,
     * because that’s not what it is at all, but I found that it helped a
     * bit in some cases. */
    for (i = 0; i < 3; i++)
        ret += tmperr[i] / 256 * tmperr[i] / 256;

    memcpy(outerr, tmperr, 3 * sizeof(int));

    return ret;
}

static uint8_t bestmove(int const *in, uint8_t bg, uint8_t fg,
                        int const *errvec, int depth, int maxerror,
                        int *error, int *out)
{
    int voidvec[3], nvoidvec[3], bestrgb[6 * 3], tmprgb[6 * 3], tmpvec[3];
    int const *voidrgb, *nvoidrgb, *vec, *rgb;
    int besterror, curerror, suberror, statice, voide, nvoide;
    int i, j, c;
    uint8_t command, bestcommand;

    /* Precompute error for the case where we change the foreground colour
     * and hence only print the background colour or its negative */
    voidrgb = palette[bg];
    voide = geterror(in, errvec, voidrgb, voidvec);
    nvoidrgb = palette[7 - bg];
    nvoide = geterror(in, errvec, nvoidrgb, nvoidvec);

    /* Precompute sub-error for the case where we print pixels (and hence
     * don’t change the palette). It’s not the exact error because we should
     * be propagating the error to the first pixel here. */
    if (depth > 0)
    {
        int tmp[3] = { 0, 0, 0 };
        bestmove(in + 6 * 3, bg, fg, tmp, depth - 1, maxerror, &statice, NULL);
    }

    /* Check every likely command:
     * 0-7: change foreground to 0-7
     * 8-15: change foreground to 0-7, print negative background
     * 16-23: change background to 0-7
     * 24-31: change background to 0-7, print negative background
     * 32: normal stuff
     * 33: inverse video stuff */
    besterror = 0x7ffffff;
    bestcommand = 0x10;
    memcpy(bestrgb, voidrgb, 6 * 3 * sizeof(int));
    for (j = 0; j < 34; j++)
    {
        static uint8_t const lookup[] =
        {
            0x00, 0x04, 0x01, 0x05, 0x02, 0x06, 0x03, 0x07,
            0x80, 0x84, 0x81, 0x85, 0x82, 0x86, 0x83, 0x87,
            0x10, 0x14, 0x11, 0x15, 0x12, 0x16, 0x13, 0x17,
            0x90, 0x94, 0x91, 0x95, 0x92, 0x96, 0x93, 0x97,
            0x40, 0xc0
        };

        uint8_t newbg = bg, newfg = fg;

        command = lookup[j];
        domove(command, &newbg, &newfg);

        /* Keeping bg and fg is useless, because we could use standard
         * pixel printing instead */
        if ((command & 0x40) == 0x00 && newbg == bg && newfg == fg)
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
            curerror = voide;
            rgb = voidrgb;
            vec = voidvec;
        }
        else if ((command & 0xf8) == 0x80)
        {
            curerror = nvoide;
            rgb = nvoidrgb;
            vec = nvoidvec;
        }
        else if ((command & 0xf8) == 0x10)
        {
            rgb = palette[newbg];
            curerror = geterror(in, errvec, rgb, tmpvec);
            vec = tmpvec;
        }
        else if ((command & 0xf8) == 0x90)
        {
            rgb = palette[7 - newbg];
            curerror = geterror(in, errvec, rgb, tmpvec);
            vec = tmpvec;
        }
        else
        {
            int const *bgcolor, *fgcolor;

            if ((command & 0x80) == 0x00)
            {
                bgcolor = palette[bg]; fgcolor = palette[fg];
            }
            else
            {
                bgcolor = palette[7 - bg]; fgcolor = palette[7 - fg];
            }

            memcpy(tmpvec, errvec, 3 * sizeof(int));
            curerror = 0;

            for (i = 0; i < 6; i++)
            {
                int vec1[3], vec2[3];
                int smalle1 = 0, smalle2 = 0;

                memcpy(vec1, tmpvec, 3 * sizeof(int));
                memcpy(vec2, tmpvec, 3 * sizeof(int));
                for (c = 0; c < 3; c++)
                {
                    int delta1, delta2;
                    delta1 = clamp(in[i * 3 + c] + tmpvec[c]) - bgcolor[c];
                    vec1[c] = delta1 * FS0 / FSX;
                    smalle1 += delta1 / 256 * delta1;
                    delta2 = clamp(in[i * 3 + c] + tmpvec[c]) - fgcolor[c];
                    vec2[c] = delta2 * FS0 / FSX;
                    smalle2 += delta2 / 256 * delta2;
                }

                if (smalle1 < smalle2)
                {
                    memcpy(tmpvec, vec1, 3 * sizeof(int));
                    memcpy(tmprgb + i * 3, bgcolor, 3 * sizeof(int));
                }
                else
                {
                    memcpy(tmpvec, vec2, 3 * sizeof(int));
                    memcpy(tmprgb + i * 3, fgcolor, 3 * sizeof(int));
                    command |= (1 << (5 - i));
                }
            }

            /* Recompute full error */
            curerror += geterror(in, errvec, tmprgb, tmpvec);

            rgb = tmprgb;
            vec = tmpvec;
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
            bestmove(in + 6 * 3, newbg, newfg, vec, depth - 1,
                     besterror - curerror, &suberror, NULL);

#if 0
            /* Slight penalty for colour changes; they're hard to revert. The
             * value of 2 was determined empirically. 1.5 is not enough and
             * 3 is too much. */
            if (newbg != bg)
                suberror = suberror * 10 / 8;
            else if (newfg != fg)
                suberror = suberror * 9 / 8;
#endif
        }
        else
            suberror = statice;

        if (curerror + suberror < besterror)
        {
            besterror = curerror + suberror;
            bestcommand = command;
            memcpy(bestrgb, rgb, 6 * 3 * sizeof(int));
        }
    }

    *error = besterror;
    if (out)
        memcpy(out, bestrgb, 6 * 3 * sizeof(int));

    return bestcommand;
}

static void write_screen(float const *data, uint8_t *screen)
{
    int *src, *srcl, *dst, *dstl;
    int stride, x, y, depth, c;

    stride = (WIDTH + 1) * 3;

    src = malloc((WIDTH + 1) * (HEIGHT + 1) * 3 * sizeof(int));
    memset(src, 0, (WIDTH + 1) * (HEIGHT + 1) * 3 * sizeof(int));

    dst = malloc((WIDTH + 1) * (HEIGHT + 1) * 3 * sizeof(int));
    memset(dst, 0, (WIDTH + 1) * (HEIGHT + 1) * 3 * sizeof(int));

    /* Import pixels into our custom format */
    for (y = 0; y < HEIGHT; y++)
        for (x = 0; x < WIDTH; x++)
            for (c = 0; c < 3; c++)
                src[y * stride + x * 3 + c] =
                     0xffff * data[(y * WIDTH + x) * 4 + (2 - c)];

    /* Let the fun begin */
    for (y = 0; y < HEIGHT; y++)
    {
        uint8_t bg = 0, fg = 7;

        //fprintf(stderr, "\rProcessing... %i%%", (y * 100 + 99) / HEIGHT);

        for (x = 0; x < WIDTH; x += 6)
        {
            int errvec[3] = { 0, 0, 0 };
            int dummy, i;
            uint8_t command;

            depth = (x + DEPTH < WIDTH) ? DEPTH : (WIDTH - x) / 6 - 1;
            srcl = src + y * stride + x * 3;
            dstl = dst + y * stride + x * 3;

            /* Recursively compute and apply best command */
            command = bestmove(srcl, bg, fg, errvec, depth, 0x7fffff,
                               &dummy, dstl);
            /* Propagate error */
            for (c = 0; c < 3; c++)
            {
                for (i = 0; i < 6; i++)
                {
                    int error = srcl[i * 3 + c] - dstl[i * 3 + c];
                    srcl[i * 3 + c + 3] =
                            clamp(srcl[i * 3 + c + 3] + error * FS0 / FSX);
                    srcl[i * 3 + c + stride - 3] += error * FS1 / FSX;
                    srcl[i * 3 + c + stride] += error * FS2 / FSX;
                    srcl[i * 3 + c + stride + 3] += error * FS3 / FSX;
                }

                for (i = -1; i < 7; i++)
                    srcl[i * 3 + c + stride] = clamp(srcl[i * 3 + c + stride]);
            }
            /* Iterate */
            domove(command, &bg, &fg);
            /* Write byte to file */
            screen[y * (WIDTH / 6) + (x / 6)] = command;
        }
    }

    //fprintf(stderr, " done.\n");
}
#endif

} /* namespace lol */

