//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Image class
// ---------------
//

#if !defined __LOL_IMAGE_H__
#define __LOL_IMAGE_H__

#include "lol/math/vector.h"

namespace lol
{

class ImageData;

class Image
{
public:
    Image(char const *path);
    ~Image();

    typedef enum
    {
        FORMAT_RGBA = 0,
        FORMAT_RGB,
        FORMAT_UNKNOWN,
    }
    format_t;

    ivec2 GetSize() const;
    format_t GetFormat() const;
    void *GetData() const;

private:
    ImageData *data;
};

} /* namespace lol */

#endif // __LOL_IMAGE_H__

