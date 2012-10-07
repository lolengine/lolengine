//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Texture class
// -----------------
//

#if !defined __LOL_TEXTURE_H__
#define __LOL_TEXTURE_H__

namespace lol
{

class Texture
{
public:
    Texture(ivec2 size);
    ~Texture();

    void Bind();
    void SetData(void *data);
    void SetSubData(ivec2 origin, ivec2 size, void *data);

private:
    class TextureData *m_data;
};

} /* namespace lol */

#endif // __LOL_TEXTURE_H__

