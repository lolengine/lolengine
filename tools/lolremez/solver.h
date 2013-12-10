//
// LolRemez - Remez algorithm implementation
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The RemezSolver class
// ---------------------
//

#include <cstdio>

template<typename NUMERIC_T> class RemezSolver
{
public:
    typedef NUMERIC_T RealFunc(NUMERIC_T const &x);

    inline RemezSolver()
      : m_order(0)
    {
    }

    void Run(int order, int decimals, NUMERIC_T a, NUMERIC_T b,
             RealFunc *func, RealFunc *weight = nullptr);

    NUMERIC_T EvalCheby(NUMERIC_T const &x);
    void Init();
    void FindZeroes();
    NUMERIC_T FindExtrema();
    void Step();

    int Cheby(int n, int k);
    int Comb(int n, int k);

    void PrintPoly();
    NUMERIC_T EvalFunc(NUMERIC_T const &x);
    NUMERIC_T Weight(NUMERIC_T const &x);

private:
    int m_order;

    lol::Array<NUMERIC_T> m_coeff;
    lol::Array<NUMERIC_T> m_zeroes;
    lol::Array<NUMERIC_T> m_control;

    RealFunc *m_func, *m_weight;
    NUMERIC_T m_k1, m_k2, m_invk1, m_invk2, m_epsilon;
    int m_decimals;
};

