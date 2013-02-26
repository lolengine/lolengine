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
// The VertexBuffer and VertexDeclaration classes
// ----------------------------------------------
//

#if !defined __LOL_GPU_VERTEXBUFFER_H__
#define __LOL_GPU_VERTEXBUFFER_H__

#include <cstring>

namespace lol
{

class VertexBuffer
{
    friend class VertexDeclaration;

public:
    VertexBuffer(size_t size);
    ~VertexBuffer();

    void *Lock(size_t offset, size_t size);
    void Unlock();

private:
    class VertexBufferData *m_data;
};

/* A safe enum to indicate how a vertex stream is going to be used. For
 * now there is only TexCoord and not TexCoord0 TexCoord1 etc. because
 * we can always reorganise the vertex declaration for the indices to
 * match. If the need arises these enums will be added. */
struct VertexUsage
{
    enum Value
    {
        Position = 0,
        BlendWeight,
        BlendIndices,
        Normal,
        PointSize,
        TexCoord,
        TexCoordExt,
        Tangent,
        Binormal,
        TessFactor,
        PositionT,
        Color,
        Fog,
        Depth,
        Sample,

        Max
    }
    m_value;

private:
    static String GetName(Value v, bool use_simple)
    {
        String tmp = String("");
        if (!use_simple) tmp += "<";
        switch (v)
        {
            case Position:      { tmp += "Position";    break; }
            case BlendWeight:   { tmp += "BlendWeight"; break; }
            case BlendIndices:  { tmp += "BlendIndices";break; }
            case Normal:        { tmp += "Normal";      break; }
            case PointSize:     { tmp += "PointSize";   break; }
            case TexCoord:      { tmp += "TexCoord";    break; }
            case TexCoordExt:   { tmp += "TexCoordExt"; break; }
            case Tangent:       { tmp += "Tangent";     break; }
            case Binormal:      { tmp += "Binormal";    break; }
            case TessFactor:    { tmp += "TessFactor";  break; }
            case PositionT:     { tmp += "PositionT";   break; }
            case Color:         { tmp += "Color";       break; }
            case Fog:           { tmp += "Fog";         break; }
            case Depth:         { tmp += "Depth";       break; }
            case Sample:        { tmp += "Sample";      break; }
            default:            { tmp += "UNDEFINED";   break; }
        }
        if (!use_simple) tmp += ">";
        return tmp;
    }

public:
    static String GetName(Value v)
    {
        return GetName(v, false);
    }

    static String GetNameList(uint32_t v)
    {
        String tmp = String("<");
        int nb = 0;
        for (int i = 0; i < Max; ++i)
        {
            if (v & (1<<i))
            {
                if (nb != 0)
                    tmp += ", ";
                tmp += GetName(Value(i), true);
                ++nb;
            }
        }
        return tmp + ">";
    }

    inline VertexUsage(Value v) : m_value(v) {}
    inline VertexUsage(int v) : m_value((Value)v) {}
    inline operator Value() { return m_value; }
};

/* A safe enum to indicate what kind of primitive to draw. Used in
 * VertexDeclaration::DrawElements() for instance. */
struct MeshPrimitive
{
    enum Value
    {
        Triangles,
        TriangleStrips,
        TriangleFans,
        Points,
        Lines,
    }
    m_value;

    inline MeshPrimitive(Value v) : m_value(v) {}
    inline operator Value() { return m_value; }
};

class VertexStreamBase
{
    friend class VertexDeclaration;

protected:
    enum
    {
        Typevoid = 0,
        Typehalf,     Typef16vec2, Typef16vec3, Typef16vec4,
        Typefloat,    Typevec2,    Typevec3,    Typevec4,
        Typedouble,   Typedvec2,   Typedvec3,   Typedvec4,
        Typeint8_t,   Typei8vec2,  Typei8vec3,  Typei8vec4,
        Typeuint8_t,  Typeu8vec2,  Typeu8vec3,  Typeu8vec4,
        Typeint16_t,  Typei16vec2, Typei16vec3, Typei16vec4,
        Typeuint16_t, Typeu16vec2, Typeu16vec3, Typeu16vec4,
        Typeint32_t,  Typeivec2,   Typeivec3,   Typeivec4,
        Typeuint32_t, Typeuvec2,   Typeuvec3,   Typeuvec4,
    };

#define LOL_TYPE(T) \
    static uint8_t GetType(T *x) { (void)x; return Type##T; }

    LOL_TYPE(void)
    LOL_TYPE(half)     LOL_TYPE(f16vec2) LOL_TYPE(f16vec3) LOL_TYPE(f16vec4)
    LOL_TYPE(float)    LOL_TYPE(vec2)    LOL_TYPE(vec3)    LOL_TYPE(vec4)
    LOL_TYPE(double)   LOL_TYPE(dvec2)   LOL_TYPE(dvec3)   LOL_TYPE(dvec4)
    LOL_TYPE(int8_t)   LOL_TYPE(i8vec2)  LOL_TYPE(i8vec3)  LOL_TYPE(i8vec4)
    LOL_TYPE(uint8_t)  LOL_TYPE(u8vec2)  LOL_TYPE(u8vec3)  LOL_TYPE(u8vec4)
    LOL_TYPE(int16_t)  LOL_TYPE(i16vec2) LOL_TYPE(i16vec3) LOL_TYPE(i16vec4)
    LOL_TYPE(uint16_t) LOL_TYPE(u16vec2) LOL_TYPE(u16vec3) LOL_TYPE(u16vec4)
    LOL_TYPE(int32_t)  LOL_TYPE(ivec2)   LOL_TYPE(ivec3)   LOL_TYPE(ivec4)
    LOL_TYPE(uint32_t) LOL_TYPE(uvec2)   LOL_TYPE(uvec3)   LOL_TYPE(uvec4)
#undef LOL_TYPE

    template<typename T> inline void AddStream(int n, VertexUsage usage)
    {
        m_streams[n].stream_type = GetType((T *)nullptr);
        m_streams[n].usage = usage;
        m_streams[n].size = sizeof(T);
    }

    VertexStreamBase() {}

private:
    struct { uint8_t stream_type, usage, size; } m_streams[12 + 1];

    static VertexStreamBase const Empty;
};

/* Specialise this template for "void" to act as a NOP */
template<>
inline void VertexStreamBase::AddStream<void>(int n, VertexUsage usage)
{
    (void)usage;
    m_streams[n].size = 0;
}

template<typename T1 = void,  typename T2 = void,  typename T3 = void,
         typename T4 = void,  typename T5 = void,  typename T6 = void,
         typename T7 = void,  typename T8 = void,  typename T9 = void,
         typename T10 = void, typename T11 = void, typename T12 = void>
class VertexStream : public VertexStreamBase
{
public:
    inline VertexStream(VertexUsage u1,
                        VertexUsage u2 = VertexUsage::Position,
                        VertexUsage u3 = VertexUsage::Position,
                        VertexUsage u4 = VertexUsage::Position,
                        VertexUsage u5 = VertexUsage::Position,
                        VertexUsage u6 = VertexUsage::Position,
                        VertexUsage u7 = VertexUsage::Position,
                        VertexUsage u8 = VertexUsage::Position,
                        VertexUsage u9 = VertexUsage::Position,
                        VertexUsage u10 = VertexUsage::Position,
                        VertexUsage u11 = VertexUsage::Position,
                        VertexUsage u12 = VertexUsage::Position)
    {
        AddStream<T1>(0, u1);    AddStream<T2>(1, u2);
        AddStream<T3>(2, u3);    AddStream<T4>(3, u4);
        AddStream<T5>(4, u5);    AddStream<T6>(5, u6);
        AddStream<T7>(6, u7);    AddStream<T8>(7, u8);
        AddStream<T9>(8, u9);    AddStream<T10>(9, u10);
        AddStream<T11>(10, u11); AddStream<T12>(11, u12);
    }
};

class VertexDeclaration
{
public:
    VertexDeclaration(VertexStreamBase const &s1 = VertexStreamBase::Empty,
                      VertexStreamBase const &s2 = VertexStreamBase::Empty,
                      VertexStreamBase const &s3 = VertexStreamBase::Empty,
                      VertexStreamBase const &s4 = VertexStreamBase::Empty,
                      VertexStreamBase const &s5 = VertexStreamBase::Empty,
                      VertexStreamBase const &s6 = VertexStreamBase::Empty,
                      VertexStreamBase const &s7 = VertexStreamBase::Empty,
                      VertexStreamBase const &s8 = VertexStreamBase::Empty,
                      VertexStreamBase const &s9 = VertexStreamBase::Empty,
                      VertexStreamBase const &s10 = VertexStreamBase::Empty,
                      VertexStreamBase const &s11 = VertexStreamBase::Empty,
                      VertexStreamBase const &s12 = VertexStreamBase::Empty);
    ~VertexDeclaration();

    void Bind();

    /* Draw elements. See MeshPrimitive for a list of all available
     * types. Both skip and count are numbers of vertices, not primitives. */
    void DrawElements(MeshPrimitive type, int skip, int count);

    /* Draw elements. See MeshPrimitive for a list of all available
     * types. Both skip and count are numbers of indices, not primitives. */
    void DrawIndexedElements(MeshPrimitive type, int vbase, int vskip,
                             int vcount, int skip, int count);

    void Unbind();
    void SetStream(VertexBuffer *vb, ShaderAttrib attr1,
                                     ShaderAttrib attr2 = ShaderAttrib(),
                                     ShaderAttrib attr3 = ShaderAttrib(),
                                     ShaderAttrib attr4 = ShaderAttrib(),
                                     ShaderAttrib attr5 = ShaderAttrib(),
                                     ShaderAttrib attr6 = ShaderAttrib(),
                                     ShaderAttrib attr7 = ShaderAttrib(),
                                     ShaderAttrib attr8 = ShaderAttrib(),
                                     ShaderAttrib attr9 = ShaderAttrib(),
                                     ShaderAttrib attr10 = ShaderAttrib(),
                                     ShaderAttrib attr11 = ShaderAttrib(),
                                     ShaderAttrib attr12 = ShaderAttrib());

private:
    void Initialize();
    void AddStream(VertexStreamBase const &);

    struct { uint8_t stream_type, index, usage, size; int reg; } m_streams[12 + 1];
    int m_count;

    void *m_data;
};

} /* namespace lol */

#endif // __LOL_GPU_VERTEXBUFFER_H__

