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
    void Print(vec3i pos, char const *str);
    vec2i GetSize() const;

private:
    FontData *data;
};

#endif // __DH_FONT_H__

