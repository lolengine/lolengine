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

    size_t GetSize();

    void *Lock(size_t offset, size_t size);
    void Unlock();

private:
    class VertexBufferData *m_data;
};

/* A safe enum to indicate what kind of primitive to draw. Used in
 * VertexDeclaration::DrawElements() for instance. */
LOL_SAFE_ENUM(MeshPrimitive,
    Triangles,
    TriangleStrips,
    TriangleFans,
    Points,
    Lines,
);

class VertexStreamBase
{
    friend class VertexDeclaration;

public:
    enum
    {
        /* XXX: be sure to fix all appropriate places in vertexbuffer.cpp
         * when modifying this list. */
        Typevoid = 0,
#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
        Typehalf,     Typef16vec2, Typef16vec3, Typef16vec4,
#endif
        Typefloat,    Typevec2,    Typevec3,    Typevec4,
        Typedouble,   Typedvec2,   Typedvec3,   Typedvec4,
        Typeint8_t,   Typei8vec2,  Typei8vec3,  Typei8vec4,
        Typeuint8_t,  Typeu8vec2,  Typeu8vec3,  Typeu8vec4,
        Typeint16_t,  Typei16vec2, Typei16vec3, Typei16vec4,
        Typeuint16_t, Typeu16vec2, Typeu16vec3, Typeu16vec4,
        Typeint32_t,  Typeivec2,   Typeivec3,   Typeivec4,
        Typeuint32_t, Typeuvec2,   Typeuvec3,   Typeuvec4,
    };

    int GetSize() const
    {
        int size = 0, i = 0;
        while (m_streams[i].size)
            size += m_streams[i++].size;
        return size;
    }

    int GetStreamCount() const
    {
        int i = 0;
        while (m_streams[i].size) ++i;
        return i;
    }

    VertexUsage GetUsage(int index) const
    {
        return m_streams[index].usage;
    }

    uint8_t GetType(int index) const
    {
        return m_streams[index].stream_type;
    }

    uint8_t GetSize(int index) const
    {
        return m_streams[index].size;
    }

protected:

#define __T(T) \
    static uint8_t GetType(T *x) { UNUSED(x); return Type##T; }

    __T(void)
#if LOL_FEATURE_CXX11_UNRESTRICTED_UNIONS
    __T(half)     __T(f16vec2) __T(f16vec3) __T(f16vec4)
#endif
    __T(float)    __T(vec2)    __T(vec3)    __T(vec4)
    __T(double)   __T(dvec2)   __T(dvec3)   __T(dvec4)
    __T(int8_t)   __T(i8vec2)  __T(i8vec3)  __T(i8vec4)
    __T(uint8_t)  __T(u8vec2)  __T(u8vec3)  __T(u8vec4)
    __T(int16_t)  __T(i16vec2) __T(i16vec3) __T(i16vec4)
    __T(uint16_t) __T(u16vec2) __T(u16vec3) __T(u16vec4)
    __T(int32_t)  __T(ivec2)   __T(ivec3)   __T(ivec4)
    __T(uint32_t) __T(uvec2)   __T(uvec3)   __T(uvec4)
#undef __T

    template<typename T> inline void AddStream(int n, VertexUsage usage)
    {
        m_streams[n].stream_type = GetType((T *)nullptr);
        m_streams[n].usage = usage;
        m_streams[n].size = sizeof(T);
    }

    VertexStreamBase() {}

private:
    struct { uint8_t stream_type, size; VertexUsage usage; } m_streams[12 + 1];

    static VertexStreamBase const Empty;
};

/* Specialise this template for "void" to act as a NOP */
template<>
inline void VertexStreamBase::AddStream<void>(int n, VertexUsage usage)
{
    UNUSED(usage);
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

    void SetStream(VertexBuffer *vb, ShaderAttrib attribs[]);

    int GetStreamCount() const;

    VertexStreamBase GetStream(int index) const;

private:
    void Initialize();
    void AddStream(VertexStreamBase const &);

    struct
    {
        uint8_t stream_type, index, size;
        VertexUsage usage;
        int reg;
    } m_streams[12 + 1];

    int m_count;

private:
    class VertexDeclarationData *m_data;
};

} /* namespace lol */

#endif // __LOL_GPU_VERTEXBUFFER_H__

