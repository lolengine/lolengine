//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The matrix classes
// ------------------
//

#if !defined __LOL_MATH_MATRIX_H__
#define __LOL_MATH_MATRIX_H__

#include <ostream>

#include <lol/math/vector.h>
#include <lol/math/transform.h>

namespace lol
{

#if !LOL_FEATURE_CXX11_CONSTEXPR
#   define constexpr /* */
#endif

/*
 * The generic “mat_t” type, which is fixed-size
 */

template<typename T, int COLS, int ROWS>
struct mat_t : public linear_ops::base
{
    typedef mat_t<T,COLS,ROWS> type;

    inline mat_t() {}

    explicit inline mat_t(T const &val)
    {
        T const zero = T(0);
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < ROWS; ++j)
                m_data[i][j] = i == j ? val : zero;
    }

    inline vec_t<T,ROWS>& operator[](size_t n) { return m_data[n]; }
    inline vec_t<T,ROWS> const& operator[](size_t n) const { return m_data[n]; }

private:
    vec_t<T,ROWS> m_data[COLS];
};

/*
 * 2×2-element matrices
 */

template <typename T>
struct mat_t<T, 2, 2> : public linear_ops::base
{
    typedef mat_t<T,2,2> type;

    inline mat_t() {}
    inline mat_t(vec_t<T,2> v0, vec_t<T,2> v1)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ v0, v1 } {}
#else
      : m_v0(v0), m_v1(v1) {}
#endif

    explicit inline mat_t(T const &val)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec_t<T,2>(val, T(0)),
                vec_t<T,2>(T(0), val) } {}
#else
      : m_v0(val, T(0)),
        m_v1(T(0), val) {}
#endif

    explicit inline mat_t(mat_t<T,4,4> const &m)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ m[0].xy, m[1].xy } {}
#else
      : m_v0(m[0].xy), m_v1(m[1].xy) {}
#endif

#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    inline vec_t<T,2>& operator[](size_t n) { return m_data[n]; }
    inline vec_t<T,2> const& operator[](size_t n) const { return m_data[n]; }
#else
    inline vec_t<T,2>& operator[](size_t n) { return (&m_v0)[n]; }
    inline vec_t<T,2> const& operator[](size_t n) const { return (&m_v0)[n]; }
#endif

    /* Helpers for transformation matrices */
    static mat_t<T,2,2> rotate(T degrees);
    static inline mat_t<T,2,2> rotate(mat_t<T,2,2> m, T degrees)
    {
        return rotate(degrees) * m;
    }

    void printf() const;
    String tostring() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream,
                                    mat_t<U,2,2> const &m);

    static const mat_t<T,2,2> identity;

private:
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    vec_t<T,2> m_data[2];
#else
    vec_t<T,2> m_v0, m_v1;
#endif
};

/*
 * 3×3-element matrices
 */

template <typename T>
struct mat_t<T, 3, 3> : public linear_ops::base
{
    typedef mat_t<T,3,3> type;

    inline mat_t() {}
    inline mat_t(vec_t<T,3> v0, vec_t<T,3> v1, vec_t<T,3> v2)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ v0, v1, v2 } {}
#else
      : m_v0(v0), m_v1(v1), m_v2(v2) {}
#endif

    explicit inline mat_t(T const &val)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec_t<T,3>(val, (T)0, (T)0),
                vec_t<T,3>((T)0, val, (T)0),
                vec_t<T,3>((T)0, (T)0, val) } {}
#else
      : m_v0(val, (T)0, (T)0),
        m_v1((T)0, val, (T)0),
        m_v2((T)0, (T)0, val) {}
#endif

    explicit inline mat_t(mat_t<T,2,2> m, T const &val = T(1))
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec_t<T,3>(m[0], (T)0),
                vec_t<T,3>(m[1], (T)0),
                vec_t<T,3>((T)0, (T)0, val) } {}
#else
      : m_v0(m[0], (T)0),
        m_v1(m[1], (T)0),
        m_v2((T)0, (T)0, val) {}
#endif

    explicit inline mat_t(mat_t<T,4,4> const &m)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ m[0].xyz, m[1].xyz, m[2].xyz } {}
#else
      : m_v0(m[0].xyz), m_v1(m[1].xyz), m_v2(m[2].xyz) {}
#endif

    explicit mat_t(quat_t<T> const &q);

#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    inline vec_t<T,3>& operator[](size_t n) { return m_data[n]; }
    inline vec_t<T,3> const& operator[](size_t n) const { return m_data[n]; }
#else
    inline vec_t<T,3>& operator[](size_t n) { return (&m_v0)[n]; }
    inline vec_t<T,3> const& operator[](size_t n) const { return (&m_v0)[n]; }
#endif

    /* Helpers for transformation matrices */
    static mat_t<T,3,3> scale(T x);
    static mat_t<T,3,3> scale(T x, T y, T z);
    static mat_t<T,3,3> scale(vec_t<T,3> v);
    static mat_t<T,3,3> rotate(T degrees, T x, T y, T z);
    static mat_t<T,3,3> rotate(T degrees, vec_t<T,3> v);

    static mat_t<T,3,3> fromeuler_xyz(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_xzy(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_yxz(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_yzx(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_zxy(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_zyx(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_xyz(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_xzy(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_yxz(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_yzx(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_zxy(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_zyx(T phi, T theta, T psi);

    static mat_t<T,3,3> fromeuler_xyx(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_xzx(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_yxy(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_yzy(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_zxz(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_zyz(vec_t<T,3> const &v);
    static mat_t<T,3,3> fromeuler_xyx(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_xzx(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_yxy(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_yzy(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_zxz(T phi, T theta, T psi);
    static mat_t<T,3,3> fromeuler_zyz(T phi, T theta, T psi);

    static inline mat_t<T,3,3> rotate(mat_t<T,3,3> m, T degrees, vec_t<T,3> v)
    {
        return rotate(degrees, v) * m;
    }

    void printf() const;
    String tostring() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream,
                                    mat_t<U,3,3> const &m);

    static const mat_t<T,3,3> identity;

private:
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    vec_t<T,3> m_data[3];
#else
    vec_t<T,3> m_v0, m_v1, m_v2;
#endif
};

/*
 * 4×4-element matrices
 */

template <typename T>
struct mat_t<T, 4, 4> : public linear_ops::base
{
    typedef mat_t<T,4,4> type;

    inline mat_t() {}
    inline mat_t(vec_t<T,4> v0, vec_t<T,4> v1, vec_t<T,4> v2, vec_t<T,4> v3)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ v0, v1, v2, v3 } {}
#else
      : m_v0(v0), m_v1(v1), m_v2(v2), m_v3(v3) {}
#endif

    explicit inline mat_t(T const &val)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec_t<T,4>(val, (T)0, (T)0, (T)0),
                vec_t<T,4>((T)0, val, (T)0, (T)0),
                vec_t<T,4>((T)0, (T)0, val, (T)0),
                vec_t<T,4>((T)0, (T)0, (T)0, val) } {}
#else
      : m_v0(val, (T)0, (T)0, (T)0),
        m_v1((T)0, val, (T)0, (T)0),
        m_v2((T)0, (T)0, val, (T)0),
        m_v3((T)0, (T)0, (T)0, val) {}
#endif

    explicit inline mat_t(mat_t<T,2,2> m, T const &val = T(1))
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec_t<T,4>(m[0], (T)0, (T)0),
                vec_t<T,4>(m[1], (T)0, (T)0),
                vec_t<T,4>((T)0, (T)0, val, (T)0),
                vec_t<T,4>((T)0, (T)0, (T)0, val) } {}
#else
      : m_v0(m[0], (T)0, (T)0),
        m_v1(m[1], (T)0, (T)0),
        m_v2((T)0, (T)0, val, (T)0),
        m_v3((T)0, (T)0, (T)0, val) {}
#endif

    explicit inline mat_t(mat_t<T,3,3> m, T const &val = T(1))
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec_t<T,4>(m[0], (T)0),
                vec_t<T,4>(m[1], (T)0),
                vec_t<T,4>(m[2], (T)0),
                vec_t<T,4>((T)0, (T)0, (T)0, val) } {}
#else
      : m_v0(m[0], (T)0),
        m_v1(m[1], (T)0),
        m_v2(m[2], (T)0),
        m_v3((T)0, (T)0, (T)0, val) {}
#endif

    explicit mat_t(quat_t<T> const &q);

#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    inline vec_t<T,4>& operator[](size_t n) { return m_data[n]; }
    inline vec_t<T,4> const& operator[](size_t n) const { return m_data[n]; }
#else
    inline vec_t<T,4>& operator[](size_t n) { return (&m_v0)[n]; }
    inline vec_t<T,4> const& operator[](size_t n) const { return (&m_v0)[n]; }
#endif

    /* Helpers for transformation matrices */
    static mat_t<T,4,4> translate(T x, T y, T z);
    static mat_t<T,4,4> translate(vec_t<T,3> v);

    static inline mat_t<T,4,4> scale(T x)
    {
        return mat_t<T,4,4>(mat_t<T,3,3>::scale(x), (T)1);
    }

    static inline mat_t<T,4,4> scale(T x, T y, T z)
    {
        return mat_t<T,4,4>(mat_t<T,3,3>::scale(x, y, z), (T)1);
    }

    static inline mat_t<T,4,4> scale(vec_t<T,3> v)
    {
        return mat_t<T,4,4>(mat_t<T,3,3>::scale(v), (T)1);
    }

    static inline mat_t<T,4,4> translate(mat_t<T,4,4> const &m, vec_t<T,3> v)
    {
        return translate(v) * m;
    }

    static inline mat_t<T,4,4> rotate(T degrees, T x, T y, T z)
    {
        return mat_t<T,4,4>(mat_t<T,3,3>::rotate(degrees, x, y, z), (T)1);
    }

    static inline mat_t<T,4,4> rotate(T degrees, vec_t<T,3> v)
    {
        return mat_t<T,4,4>(mat_t<T,3,3>::rotate(degrees, v), (T)1);
    }

    static inline mat_t<T,4,4> rotate(mat_t<T,4,4> &m, T degrees, vec_t<T,3> v)
    {
        return rotate(degrees, v) * m;
    }

    static mat_t<T,4,4> fromeuler_xyz(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_xzy(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_yxz(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_yzx(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_zxy(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_zyx(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_xyz(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_xzy(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_yxz(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_yzx(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_zxy(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_zyx(T phi, T theta, T psi);

    static mat_t<T,4,4> fromeuler_xyx(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_xzx(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_yxy(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_yzy(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_zxz(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_zyz(vec_t<T,3> const &v);
    static mat_t<T,4,4> fromeuler_xyx(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_xzx(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_yxy(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_yzy(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_zxz(T phi, T theta, T psi);
    static mat_t<T,4,4> fromeuler_zyz(T phi, T theta, T psi);

    /* Helpers for view matrices */
    static mat_t<T,4,4> lookat(vec_t<T,3> eye, vec_t<T,3> center, vec_t<T,3> up);

    /* Helpers for projection matrices */
    static mat_t<T,4,4> ortho(T left, T right, T bottom, T top, T near, T far);
    static mat_t<T,4,4> ortho(T width, T height, T near, T far);
    static mat_t<T,4,4> frustum(T left, T right, T bottom, T top, T near, T far);
    static mat_t<T,4,4> perspective(T fov_y, T width, T height, T near, T far);
    static mat_t<T,4,4> shifted_perspective(T fov_y, T screen_size, T screen_ratio_yx, T near, T far);

    void printf() const;
    String tostring() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream,
                                    mat_t<U,4,4> const &m);

    static const mat_t<T,4,4> identity;

private:
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    vec_t<T,4> m_data[4];
#else
    vec_t<T,4> m_v0, m_v1, m_v2, m_v3;
#endif
};

template<typename T> T determinant(mat_t<T,2,2> const &);
template<typename T> T determinant(mat_t<T,3,3> const &);
template<typename T> T determinant(mat_t<T,4,4> const &);

template<typename T> mat_t<T,2,2> inverse(mat_t<T,2,2> const &);
template<typename T> mat_t<T,3,3> inverse(mat_t<T,3,3> const &);
template<typename T> mat_t<T,4,4> inverse(mat_t<T,4,4> const &);

template<typename T, int COLS, int ROWS>
static inline mat_t<T, ROWS, COLS> transpose(mat_t<T, COLS, ROWS> const &m)
{
    mat_t<T, ROWS, COLS> ret;
    for (int i = 0; i < COLS; ++i)
        for (int j = 0; j < ROWS; ++j)
            ret[j][i] = m[i][j];
    return ret;
}

/*
 * Addition/subtraction/unary
 */

template<typename T, int COLS, int ROWS>
static inline mat_t<T, COLS, ROWS> &operator +=(mat_t<T, COLS, ROWS> &a,
                                                mat_t<T, COLS, ROWS> const &b)
{
    for (int i = 0; i < COLS; ++i)
        a[i] += b[i];
    return a;
}

template<typename T, int COLS, int ROWS>
static inline mat_t<T, COLS, ROWS> operator +(mat_t<T, COLS, ROWS> const &a,
                                              mat_t<T, COLS, ROWS> const &b)
{
    mat_t<T, COLS, ROWS> ret = a;
    return ret += b;
}

template<typename T, int COLS, int ROWS>
static inline mat_t<T, COLS, ROWS> &operator -=(mat_t<T, COLS, ROWS> &a,
                                                mat_t<T, COLS, ROWS> const &b)
{
    for (int i = 0; i < COLS; ++i)
        a[i] -= b[i];
    return a;
}

template<typename T, int COLS, int ROWS>
static inline mat_t<T, COLS, ROWS> operator -(mat_t<T, COLS, ROWS> const &a,
                                              mat_t<T, COLS, ROWS> const &b)
{
    mat_t<T, COLS, ROWS> ret = a;
    return ret -= b;
}

template<typename T, int COLS, int ROWS>
static inline mat_t<T, COLS, ROWS> operator -(mat_t<T, COLS, ROWS> const &m)
{
    mat_t<T, COLS, ROWS> ret;
    for (int i = 0; i < COLS; ++i)
        ret[i] = -m[i];
    return ret;
}

/*
 * Matrix-vector and vector-matrix multiplication
 */

template<typename T, int COLS, int ROWS, int SWIZZLE>
static inline vec_t<T, ROWS> operator *(mat_t<T, COLS, ROWS> const &m,
                                        vec_t<T, COLS, SWIZZLE> const &v)
{
    vec_t<T, ROWS> ret(T(0));
    for (int i = 0; i < COLS; ++i)
        ret += m[i] * v[i];
    return ret;
}

template<typename T, int COLS, int ROWS, int SWIZZLE>
static inline vec_t<T, COLS> operator *(vec_t<T, ROWS, SWIZZLE> const &v,
                                        mat_t<T, COLS, ROWS> const &m)
{
    vec_t<T, COLS> ret(T(0));
    for (int i = 0; i < COLS; ++i)
        ret[i] = dot(v, m[i]);
    return ret;
}

/*
 * Matrix-matrix multiplication
 */

template<typename T, int COLS, int N, int ROWS>
static inline mat_t<T, COLS, ROWS> operator *(mat_t<T, N, ROWS> const &a,
                                              mat_t<T, COLS, N> const &b)
{
    mat_t<T, COLS, ROWS> ret;
    for (int i = 0; i < COLS; ++i)
        ret[i] = a * b[i];
    return ret;
}

template<typename T, int N>
static inline mat_t<T, N, N> &operator *=(mat_t<T, N, N> &a,
                                          mat_t<T, N, N> const &b)
{
    return a = a * b;
}

#if !LOL_FEATURE_CXX11_CONSTEXPR
#undef constexpr
#endif

} /* namespace lol */

#endif // __LOL_MATH_MATRIX_H__

