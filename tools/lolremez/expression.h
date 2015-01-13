//
//  LolRemez - Remez algorithm implementation
//
//  Copyright © 2005—2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Parser tools for a simple calculator grammar with + - * /
//

#include "pegtl.hh"

namespace grammar
{

using namespace pegtl;

enum class op : uint8_t
{
    /* Variables and constants */
    x,
    constant,
    /* Unary functions/operators */
    plus, minus, abs,
    sqrt, cbrt,
    exp, exp2, log, log2, log10,
    sin, cos, tan,
    asin, acos, atan,
    sinh, cosh, tanh,
    /* Binary functions/operators */
    add, sub, mul, div,
    atan2, pow,
    min, max,
};

// Map operation enums to pegl::string<> rules
template<op OP> struct r_call_string {};

template<> struct r_call_string<op::abs>   : string<'a','b','s'> {};
template<> struct r_call_string<op::sqrt>  : string<'s','q','r','t'> {};
template<> struct r_call_string<op::cbrt>  : string<'c','b','r','t'> {};
template<> struct r_call_string<op::exp>   : string<'e','x','p'> {};
template<> struct r_call_string<op::exp2>  : string<'e','x','p','2'> {};
template<> struct r_call_string<op::log>   : string<'l','o','g'> {};
template<> struct r_call_string<op::log2>  : string<'l','o','g','2'> {};
template<> struct r_call_string<op::log10> : string<'l','o','g','1','0'> {};
template<> struct r_call_string<op::sin>   : string<'s','i','n'> {};
template<> struct r_call_string<op::cos>   : string<'c','o','s'> {};
template<> struct r_call_string<op::tan>   : string<'t','a','n'> {};
template<> struct r_call_string<op::asin>  : string<'a','s','i','n'> {};
template<> struct r_call_string<op::acos>  : string<'a','c','o','s'> {};
template<> struct r_call_string<op::atan>  : string<'a','t','a','n'> {};
template<> struct r_call_string<op::sinh>  : string<'s','i','n','h'> {};
template<> struct r_call_string<op::cosh>  : string<'c','o','s','h'> {};
template<> struct r_call_string<op::tanh>  : string<'t','a','n','h'> {};

template<> struct r_call_string<op::atan2> : string<'a','t','a','n','2'> {};
template<> struct r_call_string<op::pow>   : string<'p','o','w'> {};
template<> struct r_call_string<op::min>   : string<'m','i','n'> {};
template<> struct r_call_string<op::max>   : string<'m','a','x'> {};

struct expression
{
    /*
     * Parse arithmetic expression in x, e.g. 2*x+3
     */
    void parse(std::string const &str)
    {
        m_ops.Empty();
        m_constants.Empty();

        basic_parse_string<r_stmt>(str, this);
    }

    /*
     * Evaluate expression at x
     */
    lol::real eval(lol::real const &x) const
    {
        lol::array<lol::real> stack;
        lol::real tmp;

        for (int i = 0; i < m_ops.Count(); ++i)
        {
            switch (m_ops[i].m1)
            {
            case op::plus:
                break;
            case op::minus:
                stack.Push(-stack.Pop());
                break;
            case op::abs:
                stack.Push(fabs(stack.Pop()));
                break;
            case op::sqrt:
                stack.Push(sqrt(stack.Pop()));
                break;
            case op::cbrt:
                stack.Push(cbrt(stack.Pop()));
                break;
            case op::exp:
                stack.Push(exp(stack.Pop()));
                break;
            case op::exp2:
                stack.Push(exp2(stack.Pop()));
                break;
            case op::log:
                stack.Push(log(stack.Pop()));
                break;
            case op::log2:
                stack.Push(log2(stack.Pop()));
                break;
            case op::log10:
                stack.Push(log10(stack.Pop()));
                break;
            case op::sin:
                stack.Push(sin(stack.Pop()));
                break;
            case op::cos:
                stack.Push(cos(stack.Pop()));
                break;
            case op::tan:
                stack.Push(tan(stack.Pop()));
                break;
            case op::asin:
                stack.Push(asin(stack.Pop()));
                break;
            case op::acos:
                stack.Push(acos(stack.Pop()));
                break;
            case op::atan:
                stack.Push(atan(stack.Pop()));
                break;
            case op::sinh:
                stack.Push(sinh(stack.Pop()));
                break;
            case op::cosh:
                stack.Push(cosh(stack.Pop()));
                break;
            case op::tanh:
                stack.Push(tanh(stack.Pop()));
                break;

            case op::add:
                tmp = stack.Pop();
                stack.Push(stack.Pop() + tmp);
                break;
            case op::sub:
                tmp = stack.Pop();
                stack.Push(stack.Pop() - tmp);
                break;
            case op::mul:
                tmp = stack.Pop();
                stack.Push(stack.Pop() * tmp);
                break;
            case op::div:
                tmp = stack.Pop();
                stack.Push(stack.Pop() / tmp);
                break;
            case op::atan2:
                tmp = stack.Pop();
                stack.Push(atan2(stack.Pop(), tmp));
                break;
            case op::pow:
                tmp = stack.Pop();
                stack.Push(pow(stack.Pop(), tmp));
                break;
            case op::min:
                tmp = stack.Pop();
                stack.Push(min(stack.Pop(), tmp));
                break;
            case op::max:
                tmp = stack.Pop();
                stack.Push(max(stack.Pop(), tmp));
                break;

            case op::x:
                stack.Push(x);
                break;

            case op::constant:
                stack.Push(m_constants[m_ops[i].m2]);
                break;
            }
        }

        return stack.Pop();
    }

private:
    lol::array<op, int> m_ops;
    lol::array<lol::real> m_constants;

private:
    struct do_constant : action_base<do_constant>
    {
        static void apply(std::string const &ctx, expression *that)
        {
            /* FIXME: check if the constant is already in the list */
            that->m_ops.Push(op::constant, that->m_constants.Count());
            that->m_constants.Push(lol::real(ctx.c_str()));
        }
    };

    template<op OP>
    struct do_op : action_base<do_op<OP>>
    {
        static void apply(std::string const &ctx, expression *that)
        {
            UNUSED(ctx);
            that->m_ops.Push(OP, -1);
        }
    };

    struct r_expr;

    // r_ <- <blank> *
    struct _ : star<space> {};

    // r_call_unary <- <unary_op> "(" r_expr ")"
    template<op OP>
    struct r_call_unary : ifapply<seq<r_call_string<OP>,
                                      _,
                                      one<'('>,
                                      _,
                                      r_expr,
                                      _,
                                      one<')'>>,
                                  do_op<OP>> {};

    // r_call_binary <- <binary_op> "(" r_expr "," r_expr ")"
    template<op OP>
    struct r_call_binary : ifapply<seq<r_call_string<OP>,
                                       _,
                                       one<'('>,
                                       _,
                                       r_expr,
                                       _,
                                       one<','>,
                                       _,
                                       r_expr,
                                       _,
                                       one<')'>>,
                                   do_op<OP>> {};

    // r_constant <- <digit> + ( "." <digit> * ) ? ( [eE] [+-] ? <digit> + ) ?
    struct r_constant : ifapply<seq<plus<digit>,
                                    opt<seq<one<'.'>,
                                            star<digit>>>,
                                    opt<seq<one<'e', 'E'>,
                                            opt<one<'+', '-'>>,
                                            plus<digit>>>>,
                                do_constant> {};

    // r_var <- "x"
    struct r_var : ifapply<one<'x'>, do_op<op::x>> {};

    // r_call <- r_call_unary / r_call_binary
    struct r_call : sor<r_call_unary<op::abs>,
                        r_call_unary<op::sqrt>,
                        r_call_unary<op::cbrt>,
                        r_call_unary<op::exp>,
                        r_call_unary<op::exp2>,
                        r_call_unary<op::log>,
                        r_call_unary<op::log2>,
                        r_call_unary<op::log10>,
                        r_call_unary<op::sin>,
                        r_call_unary<op::cos>,
                        r_call_unary<op::tan>,
                        r_call_unary<op::asin>,
                        r_call_unary<op::acos>,
                        r_call_unary<op::atan>,
                        r_call_unary<op::sinh>,
                        r_call_unary<op::cosh>,
                        r_call_unary<op::tanh>,
                        r_call_binary<op::atan2>,
                        r_call_binary<op::pow>,
                        r_call_binary<op::min>,
                        r_call_binary<op::max>> {};

    // r_parentheses <- "(" r_expr ")"
    struct r_parentheses : seq<one<'('>,
                               _,
                               r_expr,
                               _,
                               one<')'>> {};

    // r_terminal <- r_call / r_var / r_constant / r_parentheses
    struct r_terminal : sor<r_call,
                            r_var,
                            r_constant,
                            r_parentheses> {};

    // r_signed <- "-" r_signed / "+" r_signed / r_terminal
    struct r_signed : sor<ifapply<seq<one<'-'>, _, r_signed>,
                                  do_op<op::minus>>,
                          seq<one<'+'>, _, r_signed>,
                          r_terminal> {};

    // r_exponent <- ( "^" / "**" ) r_signed
    struct r_exponent : ifapply<seq<_,
                                    sor<one<'^'>, string<'*', '*'>>,
                                    _,
                                    r_signed>, do_op<op::pow>> {};

    // r_factor <- r_signed ( r_exponent ) *
    struct r_factor : seq<r_signed,
                          star<r_exponent>> {};

    // r_mul <- "*" r_factor
    // r_div <- "/" r_factor
    // r_term <- r_factor ( r_mul / r_div ) *
    struct r_mul : ifapply<seq<_, one<'*'>, _, r_factor>, do_op<op::mul>> {};
    struct r_div : ifapply<seq<_, one<'/'>, _, r_factor>, do_op<op::div>> {};
    struct r_term : seq<r_factor,
                        star<sor<r_mul, r_div>>> {};

    // r_add <- "+" r_term
    // r_sub <- "-" r_term
    // r_expr <- r_term ( r_add / r_sub ) *
    struct r_add : ifapply<seq<_, one<'+'>, _, r_term>, do_op<op::add>> {};
    struct r_sub : ifapply<seq<_, one<'-'>, _, r_term>, do_op<op::sub>> {};
    struct r_expr : seq<r_term,
                        star<sor<r_add, r_sub>>> {};

    // r_stmt <- r_expr <end>
    struct r_stmt : seq<_, r_expr, _, eof> {};
};

} /* namespace grammar */

using grammar::expression;

