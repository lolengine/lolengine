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

#include <stdint.h>
#include <ostream>

#include <lol/math/vector.h>

namespace lol
{

#if !LOL_FEATURE_CXX11_CONSTEXPR
#   define constexpr /* */
#endif

/* The generic "matrix" type, which is a fixed-size matrix */
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
    inline matrix(vec<2,T> V0, vec<2,T> V1)
      : v0(V0), v1(V1) {}

    explicit inline matrix(T const &val)
      : v0(val, (T)0),
        v1((T)0, val) {}

    explicit inline matrix(matrix<4,4,T> const &mat)
      : v0(mat[0].xy),
        v1(mat[1].xy) {}

    inline vec<2,T>& operator[](size_t n) { return (&v0)[n]; }
    inline vec<2,T> const& operator[](size_t n) const { return (&v0)[n]; }

    /* Helpers for transformation matrices */
    static matrix<2,2,T> rotate(T degrees);
    static inline matrix<2,2,T> rotate(matrix<2,2,T> mat, T degrees)
    {
        return rotate(degrees) * mat;
    }

    void printf() const;
    String tostring() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream,
                                    matrix<2,2,U> const &m);

    vec<2,T> v0, v1;

    static const matrix<2,2,T> identity;
};

/*
 * 3×3-element matrices
 */

template <typename T>
struct matrix<3,3,T>
{
    typedef matrix<3,3,T> type;

    inline matrix() {}
    inline matrix(vec<3,T> V0, vec<3,T> V1, vec<3,T> V2)
      : v0(V0), v1(V1), v2(V2) {}

    explicit inline matrix(T const &val)
      : v0(val, (T)0, (T)0),
        v1((T)0, val, (T)0),
        v2((T)0, (T)0, val) {}

    explicit inline matrix(matrix<2,2,T> mat)
      : v0(mat[0], (T)0),
        v1(mat[1], (T)0),
        v2((T)0, (T)0, (T)0) {}

    explicit inline matrix(matrix<2,2,T> mat, T const &val)
      : v0(vec<3,T>(mat[0], (T)0)),
        v1(vec<3,T>(mat[1], (T)0)),
        v2((T)0, (T)0, val) {}

    explicit inline matrix(matrix<4,4,T> const &mat)
      : v0(mat[0].xyz),
        v1(mat[1].xyz),
        v2(mat[2].xyz) {}

    explicit matrix(Quat<T> const &q);

    inline vec<3,T>& operator[](size_t n) { return (&v0)[n]; }
    inline vec<3,T> const& operator[](size_t n) const { return (&v0)[n]; }

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

    static inline matrix<3,3,T> rotate(matrix<3,3,T> mat, T degrees, vec<3,T> v)
    {
        return rotate(degrees, v) * mat;
    }

    void printf() const;
    String tostring() const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &stream,
                                    matrix<3,3,U> const &m);

    vec<3,T> v0, v1, v2;

    static const matrix<3,3,T> identity;
};

/*
 * 4×4-element matrices
 */

template <typename T>
struct matrix<4, 4, T>
{
    typedef matrix<4,4,T> type;

    inline matrix() {}
    inline matrix(vec<4,T> V0, vec<4,T> V1, vec<4,T> V2, vec<4,T> V3)
      : v0(V0), v1(V1), v2(V2), v3(V3) {}

    explicit inline matrix(T const &val)
      : v0(val, (T)0, (T)0, (T)0),
        v1((T)0, val, (T)0, (T)0),
        v2((T)0, (T)0, val, (T)0),
        v3((T)0, (T)0, (T)0, val) {}

    explicit inline matrix(matrix<2,2,T> mat)
      : v0(mat[0], (T)0, (T)0),
        v1(mat[1], (T)0, (T)0),
        v2((T)0, (T)0, (T)0, (T)0),
        v3((T)0, (T)0, (T)0, (T)0) {}

    explicit inline matrix(matrix<2,2,T> mat, T const &val1, T const &val2)
      : v0(mat[0], (T)0, (T)0),
        v1(mat[1], (T)0, (T)0),
        v2((T)0, (T)0, val1, (T)0),
        v3((T)0, (T)0, (T)0, val2) {}

    explicit inline matrix(matrix<3,3,T> mat)
      : v0(mat[0], (T)0),
        v1(mat[1], (T)0),
        v2(mat[2], (T)0),
        v3((T)0, (T)0, (T)0, (T)0) {}

    explicit inline matrix(matrix<3,3,T> mat, T const &val)
      : v0(mat[0], (T)0),
        v1(mat[1], (T)0),
        v2(mat[2], (T)0),
        v3((T)0, (T)0, (T)0, val) {}

    explicit matrix(Quat<T> const &q);

    inline vec<4,T>& operator[](size_t n) { return (&v0)[n]; }
    inline vec<4,T> const& operator[](size_t n) const { return (&v0)[n]; }

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

    static inline matrix<4,4,T> translate(matrix<4,4,T> const &mat, vec<3,T> v)
    {
        return translate(v) * mat;
    }

    static inline matrix<4,4,T> rotate(T degrees, T x, T y, T z)
    {
        return matrix<4,4,T>(matrix<3,3,T>::rotate(degrees, x, y, z), (T)1);
    }

    static inline matrix<4,4,T> rotate(T degrees, vec<3,T> v)
    {
        return matrix<4,4,T>(matrix<3,3,T>::rotate(degrees, v), (T)1);
    }

    static inline matrix<4,4,T> rotate(matrix<4,4,T> &mat, T degrees, vec<3,T> v)
    {
        return rotate(degrees, v) * mat;
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

    vec<4,T> v0, v1, v2, v3;

    static const matrix<4,4,T> identity;
};

template<typename T> T determinant(matrix<2,2,T> const &);
template<typename T> T determinant(matrix<3,3,T> const &);
template<typename T> T determinant(matrix<4,4,T> const &);

template<typename T> matrix<2,2,T> transpose(matrix<2,2,T> const &);
template<typename T> matrix<3,3,T> transpose(matrix<3,3,T> const &);
template<typename T> matrix<4,4,T> transpose(matrix<4,4,T> const &);

template<typename T> matrix<2,2,T> inverse(matrix<2,2,T> const &);
template<typename T> matrix<3,3,T> inverse(matrix<3,3,T> const &);
template<typename T> matrix<4,4,T> inverse(matrix<4,4,T> const &);

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
 * Matrix-vector multiplication
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

