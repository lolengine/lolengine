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

#if !defined __DH_TEXT_H__
#define __DH_TEXT_H__

#include "entity.h"

class TextData;

class Text : public Entity
{
public:
    Text(char const *text, char const *font);
    virtual ~Text();

    void SetText(char const *text);
    void SetPos(int3 pos);

protected:
    virtual void TickDraw(float deltams);

private:
    TextData *data;
};

#endif // __DH_TEXT_H__

