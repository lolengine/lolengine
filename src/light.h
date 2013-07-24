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
// The Light class
// ---------------
//

#if !defined __LIGHT_H__
#define __LIGHT_H__

#include "worldentity.h"

namespace lol
{

class Light : public WorldEntity
{
public:
    Light();
    ~Light();

    char const *GetName() { return "<light>"; }

    void SetColor(vec4 const &col);
    vec4 GetColor();

    void SetPosition(vec4 const &pos);
    vec4 GetPosition();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    vec4 m_color;
    bool m_directional;
};

} /* namespace lol */

#endif /* __LIGHT_H__ */

