//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cstdio>

#include <caca.h>

/* FIXME: ugly MinGW hack */
#if defined _WIN32 && defined __GNUC__
extern "C"
{
int sprintf_s(char *s, size_t n, const char *fmt, ...)
{
    va_list args;
    int ret;
    va_start(args, fmt);
    ret = vsnprintf(s, n, fmt, args);
    va_end(args);
    return ret;
}

int vsnprintf_s(char *s, size_t n, size_t c, const char *fmt, va_list args)
{
    return vsnprintf(s, n, fmt, args);
}

int _time32(__time32_t *timer)
{
    return 0;
}
}
#endif

int main(void)
{
    caca_canvas_t *cv = caca_create_canvas(16, 16);
    caca_set_color_ansi(cv, CACA_WHITE, CACA_TRANSPARENT);

    for (unsigned int n = 0; n < 256; n++)
        caca_put_char(cv, n % 16, n / 16, n);

    size_t len;
    void *buffer = caca_export_canvas_to_memory(cv, "tga", &len);
    fwrite(buffer, len, 1, stdout);
    free(buffer);

    caca_free_canvas(cv);

    return 0;
}

