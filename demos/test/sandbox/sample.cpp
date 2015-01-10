//
//  Lol Engine - Sandbox program
//
//  Copyright © 2005-2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>

#include "pegtl.hh"
namespace lol { namespace parser = pegtl; }

#include "axe.h"
namespace lol { using namespace axe; }

using namespace lol;

//
// Parser tools for a simple calculator grammar with + - * /
//

struct calculator
{
    void push(double d) { stack.Push(d); }
    void pop() { printf("%f\n", stack.Pop()); }

    void mul() { auto x = stack.Pop(); stack.Push(stack.Pop() * x); }
    void div() { auto x = stack.Pop(); stack.Push(stack.Pop() / x); }
    void add() { auto x = stack.Pop(); stack.Push(stack.Pop() + x); }
    void sub() { auto x = stack.Pop(); stack.Push(stack.Pop() - x); }

    array<double> stack;
};

static void parse_pegtl(std::string const &str);
static void parse_axe(std::string const &str);

int main()
{
    std::string const str("42+2*(1-1+2+3-4*5)");

    parse_axe(str);
    parse_pegtl(str);

    return EXIT_SUCCESS;
}

//
// PegTL -- a PEG parser
//

namespace pegtl_parser
{
    using namespace lol::parser;

    #define ACTION(name, code) \
        struct name : action_base<name> { \
            static void apply(std::string const &ctx, calculator &c) { \
                code \
            } \
        };

    ACTION( do_number, c.push(atof(ctx.c_str())); )
    ACTION( do_op,
        switch (ctx[0])
        {
            case '*': c.mul(); break;
            case '/': c.div(); break;
            case '+': c.add(); break;
            case '-': c.sub(); break;
        } )
    ACTION( do_success, c.pop(); )

    #undef ACTION

    // number <- <digit> +
    struct number : ifapply<plus<digit>, do_number> {};

    // term <- number | "(" expr ")"
    struct term : sor<number,
                      seq<one<'('>, struct expr, one<')'>>> {};

    // factor <- term ( "*" term | "/" term ) *
    struct factor : seq<term,
                        star<ifapply<sor<seq<one<'*'>, term>,
                                         seq<one<'/'>, term>>, do_op>>> {};

    // expr <- factor ( "+" factor | "-" factor ) *
    struct expr : seq<factor,
                      star<ifapply<sor<seq<one<'+'>, factor>,
                                       seq<one<'-'>, factor>>, do_op>>> {};

    // stmt <- expr <end>
    struct stmt : ifapply<seq<expr, eof>, do_success> {};
};

static void parse_pegtl(std::string const & str)
{
    calculator c;
    pegtl::basic_parse_string<pegtl_parser::stmt>(str, c);
}

//
// AXE -- a recursive descent parser (needs right-recursion)
//

template<typename IT>
static void parse_axe_helper(IT i1, IT i2)
{
    calculator c;
    double d;

    #define ACTION(code) e_ref([&](...) { code })

    r_rule<IT> number, term, factor, factor_tail, expr, expr_tail, stmt;

    // number ::= <double>
    number = r_double(d) >> ACTION( c.push(d); );

    // term ::= number | ( expr )
    term = number
         | '(' & expr & ')';

    // factor ::= term factor_tail
    // factor_tail ::= * term factor_tail
    //               | / term factor_tail
    //               | ɛ
    factor_tail = '*' & term >> ACTION( c.mul(); ) & factor_tail
                | '/' & term >> ACTION( c.div(); ) & factor_tail
                | r_empty();
    factor = term & factor_tail;

    // expr ::= factor expr_tail
    // expr_tail ::= + factor expr_tail
    //             | - factor expr_tail
    //             | ɛ
    expr_tail = '+' & factor >> ACTION( c.add(); ) & expr_tail
              | '-' & factor >> ACTION( c.sub(); ) & expr_tail
              | r_empty();
    expr = factor & expr_tail;

    // stmt ::= expr <end>
    //        | <fail>
    stmt = expr & r_end() >> ACTION( c.pop(); )
         | r_fail([](...) { printf("malformed expression\n"); });

    #undef ACTION

    // Evaluate expression
    stmt(i1, i2);
}

static void parse_axe(std::string const &str)
{
    return parse_axe_helper(str.begin(), str.end());
}

