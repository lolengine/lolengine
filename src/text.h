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
// The Text class
// --------------
//

#if !defined __LOL_TEXT_H__
#define __LOL_TEXT_H__

#include "entity.h"

namespace lol
{

class TextData;

class Text : public Entity
{
public:
    Text(char const *text, char const *font);
    virtual ~Text();

    void SetText(char const *text);
    void SetInt(int val);
    void SetPos(vec3i pos);
    void SetAlign(int align);

    enum
    {
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_CENTER,
    };

protected:
    virtual void TickDraw(float deltams);

private:
    TextData *data;
};

} /* namespace lol */

#endif // __LOL_TEXT_H__

