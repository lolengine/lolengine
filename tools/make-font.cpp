
#include <caca.h>

#include <cstdlib>
#include <cstdio>

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

    /* Free everything */
    caca_free_canvas(cv);

    return 0;
}

