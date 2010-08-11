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

