//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Font class
// --------------
//

#if !defined __LOL_FONT_H__
#define __LOL_FONT_H__

#include "entity.h"

namespace lol
{

class FontData;

class Font : public Entity
{
public:
    Font(char const *path);
    ~Font();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual void TickDraw(float seconds);

public:
    /* New methods */
    void Print(vec3 pos, char const *str, vec2 scale = vec2(1.0f));
    ivec2 GetSize() const;

private:
    FontData *data;
};

} /* namespace lol */

#endif // __LOL_FONT_H__

