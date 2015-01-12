//
//  LolRemez - Remez algorithm implementation
//
//  Copyright © 2005-2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The RemezSolver class
// ---------------------
//

#include <cstdio>

#include "expression.h"

class RemezSolver
{
public:
    RemezSolver(int order, int decimals);

    void Run(lol::real a, lol::real b,
             char const *func, char const *weight = nullptr);

private:
    void Init();
    void FindZeroes();
    lol::real FindExtrema();
    void Step();
    void PrintPoly();

    lol::real EvalEstimate(lol::real const &x);
    lol::real EvalFunc(lol::real const &x);
    lol::real EvalWeight(lol::real const &x);

private:
    /* User-defined parameters */
    expression m_func, m_weight;
    int m_order, m_decimals;
    bool m_has_weight;

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

