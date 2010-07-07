
/*
 * The tile manager
 */

#if !defined __DH_TILER_H__
#define __DH_TILER_H__

class Tiler
{
public:
    static int Register(char const *path);
    static void Deregister(int id);
};

#endif // __DH_TILER_H__

