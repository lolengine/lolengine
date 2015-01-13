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
// The remez_solver class
// ----------------------
//

#include <cstdio>

#include "expression.h"

class remez_solver
{
public:
    remez_solver(int order, int decimals);

    void run(lol::real a, lol::real b,
             char const *func, char const *weight = nullptr);

private:
    void remez_init();
    void remez_step();

    void find_zeroes();
    void find_extrema();

    void print_poly();

    lol::real eval_estimate(lol::real const &x);
    lol::real eval_func(lol::real const &x);
    lol::real eval_weight(lol::real const &x);
    lol::real eval_error(lol::real const &x);

private:
    /* User-defined parameters */
    expression m_func, m_weight;
    int m_order, m_decimals;
    bool m_has_weight;

    /* Solver state */
    lol::polynomial<lol::real> m_estimate;

    lol::array<lol::real> m_zeroes;
    lol::array<lol::real> m_control;

    lol::real m_k1, m_k2, m_epsilon, m_error;

    /* Statistics */
    float m_stats_cheby;
    float m_stats_func;
    float m_stats_weight;
};

