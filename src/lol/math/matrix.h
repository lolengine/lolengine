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
 * The generic "matrix" type, which is fixed-size
 */

template<int COLS, int ROWS, typename T>
struct matrix
{
    typedef matrix<COLS,ROWS,T> type;

    inline matrix() {}

    explicit inline matrix(T const &val)
    {
        T const zero = T(0);
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < ROWS; ++j)
                m_data[i][j] = i == j ? val : zero;
    }

    inline vec<ROWS,T>& operator[](size_t n) { return m_data[n]; }
    inline vec<ROWS,T> const& operator[](size_t n) const { return m_data[n]; }

private:
    vec<ROWS,T> m_data[COLS];
};

/*
 * 2×2-element matrices
 */

template <typename T>
struct matrix<2, 2, T>
{
    typedef matrix<2,2,T> type;

    inline matrix() {}
    inline matrix(vec<2,T> v0, vec<2,T> v1)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ v0, v1 } {}
#else
      : m_v0(v0), m_v1(v1) {}
#endif

    explicit inline matrix(T const &val)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec<2,T>(val, T(0)),
                vec<2,T>(T(0), val) } {}
#else
      : m_v0(val, T(0)),
        m_v1(T(0), val) {}
#endif

    explicit inline matrix(matrix<4,4,T> const &m)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ m[0].xy, m[1].xy } {}
#else
      : m_v0(m[0].xy), m_v1(m[1].xy) {}
#endif

#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    inline vec<2,T>& operator[](size_t n) { return m_data[n]; }
    inline vec<2,T> const& operator[](size_t n) const { return m_data[n]; }
#else
    inline vec<2,T>& operator[](size_t n) { return (&m_v0)[n]; }
    inline vec<2,T> const& operator[](size_t n) const { return (&m_v0)[n]; }
#endif

    /* Helpers for transformation matrices */
    static matrix<2,2,T> rotate(T degrees);
    static inline matrix<2,2,T> rotate(matrix<2,2,T> m, T degrees)
    {
        return rotate(degrees) * m;
    }

    void printf() const;
    String tostring() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream,
                                    matrix<2,2,U> const &m);

    static const matrix<2,2,T> identity;

private:
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    vec<2,T> m_data[2];
#else
    vec<2,T> m_v0, m_v1;
#endif
};

/*
 * 3×3-element matrices
 */

template <typename T>
struct matrix<3,3,T>
{
    typedef matrix<3,3,T> type;

    inline matrix() {}
    inline matrix(vec<3,T> v0, vec<3,T> v1, vec<3,T> v2)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ v0, v1, v2 } {}
#else
      : m_v0(v0), m_v1(v1), m_v2(v2) {}
#endif

    explicit inline matrix(T const &val)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec<3,T>(val, (T)0, (T)0),
                vec<3,T>((T)0, val, (T)0),
                vec<3,T>((T)0, (T)0, val) } {}
#else
      : m_v0(val, (T)0, (T)0),
        m_v1((T)0, val, (T)0),
        m_v2((T)0, (T)0, val) {}
#endif

    explicit inline matrix(matrix<2,2,T> m, T const &val = T(1))
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec<3,T>(m[0], (T)0),
                vec<3,T>(m[1], (T)0),
                vec<3,T>((T)0, (T)0, val) } {}
#else
      : m_v0(m[0], (T)0),
        m_v1(m[1], (T)0),
        m_v2((T)0, (T)0, val) {}
#endif

    explicit inline matrix(matrix<4,4,T> const &m)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ m[0].xyz, m[1].xyz, m[2].xyz } {}
#else
      : m_v0(m[0].xyz), m_v1(m[1].xyz), m_v2(m[2].xyz) {}
#endif

    explicit matrix(Quat<T> const &q);

#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    inline vec<3,T>& operator[](size_t n) { return m_data[n]; }
    inline vec<3,T> const& operator[](size_t n) const { return m_data[n]; }
#else
    inline vec<3,T>& operator[](size_t n) { return (&m_v0)[n]; }
    inline vec<3,T> const& operator[](size_t n) const { return (&m_v0)[n]; }
#endif

    /* Helpers for transformation matrices */
    static matrix<3,3,T> scale(T x);
    static matrix<3,3,T> scale(T x, T y, T z);
    static matrix<3,3,T> scale(vec<3,T> v);
    static matrix<3,3,T> rotate(T degrees, T x, T y, T z);
    static matrix<3,3,T> rotate(T degrees, vec<3,T> v);

    static matrix<3,3,T> fromeuler_xyz(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_xzy(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_yxz(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_yzx(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_zxy(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_zyx(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_xyz(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_xzy(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_yxz(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_yzx(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_zxy(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_zyx(T phi, T theta, T psi);

    static matrix<3,3,T> fromeuler_xyx(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_xzx(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_yxy(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_yzy(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_zxz(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_zyz(vec<3,T> const &v);
    static matrix<3,3,T> fromeuler_xyx(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_xzx(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_yxy(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_yzy(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_zxz(T phi, T theta, T psi);
    static matrix<3,3,T> fromeuler_zyz(T phi, T theta, T psi);

    static inline matrix<3,3,T> rotate(matrix<3,3,T> m, T degrees, vec<3,T> v)
    {
        return rotate(degrees, v) * m;
    }

    void printf() const;
    String tostring() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream,
                                    matrix<3,3,U> const &m);

    static const matrix<3,3,T> identity;

private:
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    vec<3,T> m_data[3];
#else
    vec<3,T> m_v0, m_v1, m_v2;
#endif
};

/*
 * 4×4-element matrices
 */

template <typename T>
struct matrix<4, 4, T>
{
    typedef matrix<4,4,T> type;

    inline matrix() {}
    inline matrix(vec<4,T> v0, vec<4,T> v1, vec<4,T> v2, vec<4,T> v3)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ v0, v1, v2, v3 } {}
#else
      : m_v0(v0), m_v1(v1), m_v2(v2), m_v3(v3) {}
#endif

    explicit inline matrix(T const &val)
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec<4,T>(val, (T)0, (T)0, (T)0),
                vec<4,T>((T)0, val, (T)0, (T)0),
                vec<4,T>((T)0, (T)0, val, (T)0),
                vec<4,T>((T)0, (T)0, (T)0, val) } {}
#else
      : m_v0(val, (T)0, (T)0, (T)0),
        m_v1((T)0, val, (T)0, (T)0),
        m_v2((T)0, (T)0, val, (T)0),
        m_v3((T)0, (T)0, (T)0, val) {}
#endif

    explicit inline matrix(matrix<2,2,T> m, T const &val = T(1))
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec<4,T>(m[0], (T)0, (T)0),
                vec<4,T>(m[1], (T)0, (T)0),
                vec<4,T>((T)0, (T)0, val, (T)0),
                vec<4,T>((T)0, (T)0, (T)0, val) } {}
#else
      : m_v0(m[0], (T)0, (T)0),
        m_v1(m[1], (T)0, (T)0),
        m_v2((T)0, (T)0, val, (T)0),
        m_v3((T)0, (T)0, (T)0, val) {}
#endif

    explicit inline matrix(matrix<3,3,T> m, T const &val = T(1))
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
      : m_data{ vec<4,T>(m[0], (T)0),
                vec<4,T>(m[1], (T)0),
                vec<4,T>(m[2], (T)0),
                vec<4,T>((T)0, (T)0, (T)0, val) } {}
#else
      : m_v0(m[0], (T)0),
        m_v1(m[1], (T)0),
        m_v2(m[2], (T)0),
        m_v3((T)0, (T)0, (T)0, val) {}
#endif

    explicit matrix(Quat<T> const &q);

#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    inline vec<4,T>& operator[](size_t n) { return m_data[n]; }
    inline vec<4,T> const& operator[](size_t n) const { return m_data[n]; }
#else
    inline vec<4,T>& operator[](size_t n) { return (&m_v0)[n]; }
    inline vec<4,T> const& operator[](size_t n) const { return (&m_v0)[n]; }
#endif

    /* Helpers for transformation matrices */
    static matrix<4,4,T> translate(T x, T y, T z);
    static matrix<4,4,T> translate(vec<3,T> v);

    static inline matrix<4,4,T> scale(T x)
    {
        return matrix<4,4,T>(matrix<3,3,T>::scale(x), (T)1);
    }

    static inline matrix<4,4,T> scale(T x, T y, T z)
    {
        return matrix<4,4,T>(matrix<3,3,T>::scale(x, y, z), (T)1);
    }

    static inline matrix<4,4,T> scale(vec<3,T> v)
    {
        return matrix<4,4,T>(matrix<3,3,T>::scale(v), (T)1);
    }

    static inline matrix<4,4,T> translate(matrix<4,4,T> const &m, vec<3,T> v)
    {
        return translate(v) * m;
    }

    static inline matrix<4,4,T> rotate(T degrees, T x, T y, T z)
    {
        return matrix<4,4,T>(matrix<3,3,T>::rotate(degrees, x, y, z), (T)1);
    }

    static inline matrix<4,4,T> rotate(T degrees, vec<3,T> v)
    {
        return matrix<4,4,T>(matrix<3,3,T>::rotate(degrees, v), (T)1);
    }

    static inline matrix<4,4,T> rotate(matrix<4,4,T> &m, T degrees, vec<3,T> v)
    {
        return rotate(degrees, v) * m;
    }

    static matrix<4,4,T> fromeuler_xyz(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_xzy(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_yxz(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_yzx(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_zxy(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_zyx(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_xyz(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_xzy(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_yxz(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_yzx(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_zxy(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_zyx(T phi, T theta, T psi);

    static matrix<4,4,T> fromeuler_xyx(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_xzx(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_yxy(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_yzy(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_zxz(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_zyz(vec<3,T> const &v);
    static matrix<4,4,T> fromeuler_xyx(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_xzx(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_yxy(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_yzy(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_zxz(T phi, T theta, T psi);
    static matrix<4,4,T> fromeuler_zyz(T phi, T theta, T psi);

    /* Helpers for view matrices */
    static matrix<4,4,T> lookat(vec<3,T> eye, vec<3,T> center, vec<3,T> up);

    /* Helpers for projection matrices */
    static matrix<4,4,T> ortho(T left, T right, T bottom, T top, T near, T far);
    static matrix<4,4,T> ortho(T width, T height, T near, T far);
    static matrix<4,4,T> frustum(T left, T right, T bottom, T top, T near, T far);
    static matrix<4,4,T> perspective(T fov_y, T width, T height, T near, T far);
    static matrix<4,4,T> shifted_perspective(T fov_y, T screen_size, T screen_ratio_yx, T near, T far);

    void printf() const;
    String tostring() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream,
                                    matrix<4,4,U> const &m);

    static const matrix<4,4,T> identity;

private:
#if LOL_FEATURE_CXX11_ARRAY_INITIALIZERS
    vec<4,T> m_data[4];
#else
    vec<4,T> m_v0, m_v1, m_v2, m_v3;
#endif
};

template<typename T> T determinant(matrix<2,2,T> const &);
template<typename T> T determinant(matrix<3,3,T> const &);
template<typename T> T determinant(matrix<4,4,T> const &);

template<typename T> matrix<2,2,T> inverse(matrix<2,2,T> const &);
template<typename T> matrix<3,3,T> inverse(matrix<3,3,T> const &);
template<typename T> matrix<4,4,T> inverse(matrix<4,4,T> const &);

template<int COLS, int ROWS, typename T>
static inline matrix<ROWS, COLS, T> transpose(matrix<COLS, ROWS, T> const &m)
{
    matrix<ROWS, COLS, T> ret;
    for (int i = 0; i < COLS; ++i)
        for (int j = 0; j < ROWS; ++j)
            ret[j][i] = m[i][j];
    return ret;
}

/*
 * Addition/subtraction/unary
 */

template<int COLS, int ROWS, typename T>
static inline matrix<COLS, ROWS, T> &operator +=(matrix<COLS, ROWS, T> &a,
                                                 matrix<COLS, ROWS, T> const &b)
{
    for (int i = 0; i < COLS; ++i)
        a[i] += b[i];
    return a;
}

template<int COLS, int ROWS, typename T>
static inline matrix<COLS, ROWS, T> operator +(matrix<COLS, ROWS, T> const &a,
                                               matrix<COLS, ROWS, T> const &b)
{
    matrix<COLS, ROWS, T> ret = a;
    return ret += b;
}

template<int COLS, int ROWS, typename T>
static inline matrix<COLS, ROWS, T> operator +(matrix<COLS, ROWS, T> const &m)
{
    return m;
}

template<int COLS, int ROWS, typename T>
static inline matrix<COLS, ROWS, T> &operator -=(matrix<COLS, ROWS, T> &a,
                                                 matrix<COLS, ROWS, T> const &b)
{
    for (int i = 0; i < COLS; ++i)
        a[i] -= b[i];
    return a;
}

template<int COLS, int ROWS, typename T>
static inline matrix<COLS, ROWS, T> operator -(matrix<COLS, ROWS, T> const &a,
                                               matrix<COLS, ROWS, T> const &b)
{
    matrix<COLS, ROWS, T> ret = a;
    return ret -= b;
}

template<int COLS, int ROWS, typename T>
static inline matrix<COLS, ROWS, T> operator -(matrix<COLS, ROWS, T> const &m)
{
    matrix<COLS, ROWS, T> ret;
    for (int i = 0; i < COLS; ++i)
        ret[i] = -m[i];
    return ret;
}

/*
 * Matrix-vector and vector-matrix multiplication
 */

template<int COLS, int ROWS, int MASK, typename T>
static inline vec<ROWS, T> operator *(matrix<COLS, ROWS, T> const &m,
                                      vec<COLS, T, MASK> const &v)
{
    vec<ROWS, T> ret(T(0));
    for (int i = 0; i < COLS; ++i)
        ret += m[i] * v[i];
    return ret;
}

template<int COLS, int ROWS, int MASK, typename T>
static inline vec<COLS, T> operator *(vec<ROWS, T, MASK> const &v,
                                      matrix<COLS, ROWS, T> const &m)
{
    vec<COLS, T> ret(T(0));
    for (int i = 0; i < COLS; ++i)
        ret[i] = dot(v, m[i]);
    return ret;
}

/*
 * Matrix-matrix multiplication
 */

template<int COLS, int N, int ROWS, typename T>
static inline matrix<COLS, ROWS, T> operator *(matrix<N, ROWS, T> const &a,
                                               matrix<COLS, N, T> const &b)
{
    matrix<COLS, ROWS, T> ret;
    for (int i = 0; i < COLS; ++i)
        ret[i] = a * b[i];
    return ret;
}

template<int N, typename T>
static inline matrix<N, N, T> &operator *=(matrix<N, N, T> &a,
                                           matrix<N, N, T> const &b)
{
    return a = a * b;
}

#if !LOL_FEATURE_CXX11_CONSTEXPR
#undef constexpr
#endif

} /* namespace lol */

#endif // __LOL_MATH_MATRIX_H__

