
/*
 * The tile manager
 */

#if !defined __DH_TILER_H__
#define __DH_TILER_H__

#include <stdint.h>

class Tiler
{
public:
    static int Register(char const *path);
    static void Deregister(int id);

    static void Render(uint32_t code, int x, int y);
};

#endif // __DH_TILER_H__

