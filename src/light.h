//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Light class
// ---------------
//

#include <cfloat> /* for FLT_MAX */

#include "engine/worldentity.h"

namespace lol
{

struct LightType
{
    enum Value
    {
        Directional = 0,
        Point,

        Max,
    }
    m_value;

    String ToString()
    {
        switch (m_value)
        {
            case Directional:
                return "<Directional>";
            case Point:
                return "<Point>";
            default:
                return "<UNDEFINED>";
        }
    }

    inline LightType(float v)
    {
        float top = FLT_MAX;
        int iv = Directional;
        for (int i = 0; i < Max; ++i)
        {
            LightType lv = LightType(i);
            float nv = lv.f();
            float ntop = lol::abs(nv - v);
            if (ntop < top)
            {
                top = ntop;
                iv = i;
            }
        }
        m_value = LightType(iv);
    }
    inline LightType(int v) : m_value((Value)v) {}
    inline LightType(Value v) : m_value(v) {}
    inline LightType() : m_value(Directional) {}
    inline operator Value() { return m_value; }
    inline float f() { return ((float)m_value / (float)Max); }
};

class Light : public WorldEntity
{
public:
    Light();
    ~Light();

    char const *GetName() { return "<light>"; }

    void SetType(LightType const &type);
    LightType GetType();

    void SetColor(vec4 const &color);
    vec4 GetColor();

    void SetPosition(vec3 const &pos);
    vec3 GetPosition();

protected:
    virtual void TickGame(float seconds);
    virtual void TickDraw(float seconds, Scene &scene);

private:
    vec4        m_color;
    LightType   m_type;
};

} /* namespace lol */

