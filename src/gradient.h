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
// The Gradient class
// ------------------
//

#if !defined __LOL_GRADIENT_H__
#define __LOL_GRADIENT_H__

#include "worldentity.h"

namespace lol
{

class GradientData;

class Gradient : public WorldEntity
{
public:
    Gradient(vec3 aa, vec3 bb);
    virtual ~Gradient();

    char const *GetName() { return "<gradient>"; }

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds);

private:
    GradientData *data;
};

} /* namespace lol */

#endif // __LOL_GRADIENT_H__

