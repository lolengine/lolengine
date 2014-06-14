//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Image class
// ---------------
//

#if !defined __LOL_IMAGE_IMAGE_H__
#define __LOL_IMAGE_IMAGE_H__

#include <lol/math/vector.h>

namespace lol
{

class Image
{
    friend class ImageBank;

public:
    static Image *Create(char const *path);

    bool Save(char const *path);
    void Destroy();

    ivec2 GetSize() const;
    PixelFormat GetFormat() const;
    uint8_t *GetData() const;
    bool RetrieveTiles(Array<ivec2, ivec2>& tiles) const;

private:
    Image();
    ~Image();

    class ImageData *m_data;
};

} /* namespace lol */

#endif // __LOL_IMAGE_IMAGE_H__

