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

class RemezSolver
{
public:
    typedef lol::real RealFunc(lol::real const &x);

    RemezSolver(int order, int decimals);

    void Run(lol::real a, lol::real b,
             RealFunc *func, RealFunc *weight = nullptr);

private:
    void Init();
    void FindZeroes();
    lol::real FindExtrema();
    void Step();
    void PrintPoly();

    lol::real EvalCheby(lol::real const &x);
    lol::real EvalFunc(lol::real const &x);
    lol::real EvalWeight(lol::real const &x);

private:
    /* User-defined parameters */
    RealFunc *m_func, *m_weight;
    int m_order, m_decimals;

    /* Solver state */
    lol::polynomial<lol::real> m_estimate;

    lol::array<lol::real> m_zeroes;
    lol::array<lol::real> m_control;

    lol::real m_k1, m_k2, m_epsilon;

    /* Statistics */
    float m_stats_cheby;
    float m_stats_func;
    float m_stats_weight;
};

