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
// The Font class
// --------------
//

#if !defined __DH_FONT_H__
#define __DH_FONT_H__

#include "entity.h"

class FontData;

class Font : public Entity
{
public:
    Font(char const *path);
    ~Font();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual void TickDraw(float deltams);

public:
    /* New methods */
    void Print(int3 pos, char const *str);
    void PrintBold(int3 pos, char const *str);

private:
    FontData *data;
};

#endif // __DH_FONT_H__

