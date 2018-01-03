//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Light class
// ---------------
//

#include "engine/worldentity.h"

#include <cfloat> /* for FLT_MAX */
#include <string>

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

    std::string tostring()
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

