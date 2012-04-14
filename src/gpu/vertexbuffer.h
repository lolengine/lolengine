//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The VertexBuffer class
// ----------------------
//

#if !defined __LOL_VERTEXBUFFER_H__
#define __LOL_VERTEXBUFFER_H__

#include <cstring>

namespace lol
{

#if 0
VertexBuffer(0, LOL_TYPE_VEC2 | LOL_USAGE_TEXTURE(0),
                LOL_TYPE_FLOAT | LOL_USAGE_POSITION(0),
             1, LOL_TYPE_FLOAT | LOL_USAGE_TEXCOORD(0),
             2, LOL_TYPE_FLOAT | LOL_USAGE_TEXCOORD(1));

VertexBuffer(VertexStream<vec2, LOL_USAGE_TEXTURE(0)
                          float, Texture(
#endif

class VertexBuffer
{
public:
    VertexBuffer(VertexBuffer const& that)
    {
        memcpy(this, &that, sizeof(*this));

        /* Now call the platform-specific initialisation code */
        Initialize();
    }

    ~VertexBuffer() {}

protected:
    VertexBuffer() {}

    enum
    {
        VBO_TYPE_VOID = 0,
        VBO_TYPE_FLOAT,
        VBO_TYPE_VEC2,
        VBO_TYPE_VEC3,
        VBO_TYPE_VEC4,
        VBO_TYPE_I16VEC4,
        VBO_TYPE_U8VEC4,
    };

    static uint8_t GetType(void *x) { (void)x; return VBO_TYPE_VOID; }
    static uint8_t GetType(float *x) { (void)x; return VBO_TYPE_FLOAT; }
    static uint8_t GetType(vec2 *x) { (void)x; return VBO_TYPE_VEC2; }
    static uint8_t GetType(vec3 *x) { (void)x; return VBO_TYPE_VEC3; }
    static uint8_t GetType(vec4 *x) { (void)x; return VBO_TYPE_VEC4; }
    static uint8_t GetType(i16vec4 *x) { (void)x; return VBO_TYPE_I16VEC4; }
    static uint8_t GetType(u8vec4 *x) { (void)x; return VBO_TYPE_U8VEC4; }

    struct { uint8_t stream_type, index, size; } m_streams[12 + 1];

    template<typename T> void AddStream(int n, int index)
    {
        m_streams[n].stream_type = GetType((T *)NULL);
        m_streams[n].index = index;
        m_streams[n].size = sizeof(T);
    }

private:
    void Initialize();
};

template<> void VertexBuffer::AddStream<void>(int n, int index);

template<typename T1 = void,  typename T2 = void,  typename T3 = void,
         typename T4 = void,  typename T5 = void,  typename T6 = void,
         typename T7 = void,  typename T8 = void,  typename T9 = void,
         typename T10 = void, typename T11 = void, typename T12 = void>
class VertexDeclaration : public VertexBuffer
{
public:
    /* Arguments are the stream index; default to zero */
    VertexDeclaration(int s1 = 0,  int s2 = 0,  int s3 = 0,
                      int s4 = 0,  int s5 = 0,  int s6 = 0,
                      int s7 = 0,  int s8 = 0,  int s9 = 0,
                      int s10 = 0, int s11 = 0, int s12 = 0)
    {
        for (int i = 0; i < 12 + 1; i++)
            m_streams[i].stream_type = VBO_TYPE_VOID;

        AddStream<T1>(0, s1); AddStream<T2>(1, s2); AddStream<T3>(2, s3);
        AddStream<T4>(3, s4); AddStream<T5>(4, s5); AddStream<T6>(5, s6);
        AddStream<T7>(6, s7); AddStream<T8>(7, s8); AddStream<T9>(8, s9);
        AddStream<T10>(9, s10); AddStream<T11>(10, s11); AddStream<T12>(11, s12);
    }

private:
};

} /* namespace lol */

#endif // __LOL_VERTEXBUFFER_H__

