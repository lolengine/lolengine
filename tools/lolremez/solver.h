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
    lol::real EvalCheby(lol::real const &x);
    void Init();
    void FindZeroes();
    lol::real FindExtrema();
    void Step();

    int Cheby(int n, int k);
    int Comb(int n, int k);

    void PrintPoly();
    lol::real EvalFunc(lol::real const &x);
    lol::real Weight(lol::real const &x);

private:
    int m_order;

    lol::Array<lol::real> m_coeff;
    lol::Array<lol::real> m_zeroes;
    lol::Array<lol::real> m_control;

    RealFunc *m_func, *m_weight;
    lol::real m_k1, m_k2, m_invk1, m_invk2, m_epsilon;
    int m_decimals;
};

