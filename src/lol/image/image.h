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
    //Create/Load/Store image into bank.    THREAD: NOT SAFE
    static Image*   Create(char const *path);
    //Create/Load image into bank.          THREAD: SAFE
    static Image*   Load(char const *path);
    //Store image into bank.                THREAD: NOT SAFE
    static bool     Store(Image *img);

    bool Save(char const *path);
    void Destroy();

    ivec2 GetSize() const;
    PixelFormat GetFormat() const;
    uint8_t *GetData() const;
    String GetPath() const;
    bool RetrieveTiles(Array<ivec2, ivec2>& tiles) const;

private:
    Image(char const* path);
    ~Image();

    class ImageData *m_data;
    String m_path;
};

} /* namespace lol */

#endif // __LOL_IMAGE_IMAGE_H__

