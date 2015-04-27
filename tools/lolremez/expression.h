//
//  LolRemez — Remez algorithm implementation
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
// Powerful arithmetic expression parser/evaluator
//
// Usage:
//   expression e;
//   e.parse(" 2*x^3 + 3 * sin(x - atan(x))");
//   auto y = e.eval("1.5");
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
        m_ops.empty();
        m_constants.empty();

        basic_parse_string<r_stmt>(str, this);
    }

    /*
     * Evaluate expression at x
     */
    lol::real eval(lol::real const &x) const
    {
        /* Use a stack */
        lol::array<lol::real> stack;

        for (int i = 0; i < m_ops.count(); ++i)
        {
            /* Rules that do not consume stack elements */
            if (m_ops[i].m1 == op::x)
            {
                stack.push(x);
                continue;
            }
            else if (m_ops[i].m1 == op::constant)
            {
                stack.push(m_constants[m_ops[i].m2]);
                continue;
            }

            /* All other rules consume at least the head of the stack */
            lol::real head = stack.pop();

            switch (m_ops[i].m1)
            {
            case op::plus:  stack.push(head);  break;
            case op::minus: stack.push(-head); break;

            case op::abs:   stack.push(fabs(head));  break;
            case op::sqrt:  stack.push(sqrt(head));  break;
            case op::cbrt:  stack.push(cbrt(head));  break;
            case op::exp:   stack.push(exp(head));   break;
            case op::exp2:  stack.push(exp2(head));  break;
            case op::log:   stack.push(log(head));   break;
            case op::log2:  stack.push(log2(head));  break;
            case op::log10: stack.push(log10(head)); break;
            case op::sin:   stack.push(sin(head));   break;
            case op::cos:   stack.push(cos(head));   break;
            case op::tan:   stack.push(tan(head));   break;
            case op::asin:  stack.push(asin(head));  break;
            case op::acos:  stack.push(acos(head));  break;
            case op::atan:  stack.push(atan(head));  break;
            case op::sinh:  stack.push(sinh(head));  break;
            case op::cosh:  stack.push(cosh(head));  break;
            case op::tanh:  stack.push(tanh(head));  break;

            case op::add:   stack.push(stack.pop() + head); break;
            case op::sub:   stack.push(stack.pop() - head); break;
            case op::mul:   stack.push(stack.pop() * head); break;
            case op::div:   stack.push(stack.pop() / head); break;

            case op::atan2: stack.push(atan2(stack.pop(), head)); break;
            case op::pow:   stack.push(pow(stack.pop(), head));   break;
            case op::min:   stack.push(min(stack.pop(), head));   break;
            case op::max:   stack.push(max(stack.pop(), head));   break;

            case op::x:
            case op::constant:
                /* Already handled above */
                break;
            }
        }

        ASSERT(stack.count() == 1);
        return stack.pop();
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
            that->m_ops.push(op::constant, that->m_constants.count());
            that->m_constants.push(lol::real(ctx.c_str()));
        }
    };

    template<op OP>
    struct do_op : action_base<do_op<OP>>
    {
        static void apply(std::string const &ctx, expression *that)
        {
            UNUSED(ctx);
            that->m_ops.push(OP, -1);
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
    struct r_stmt : seq<_, r_expr, _, pegtl::eof> {};
};

} /* namespace grammar */

using grammar::expression;

