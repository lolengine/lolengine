/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2010 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   lollex

/* First part of user declarations.  */

/* Line 310 of lalr1.cc  */
#line 1 "easymesh/easymesh-parser.y"

//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2013 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "easymesh/easymesh.h"

#include <string>


/* Line 310 of lalr1.cc  */
#line 65 "generated/easymesh-parser.cpp"


#include "easymesh-parser.h"

/* User implementation prologue.  */

/* Line 316 of lalr1.cc  */
#line 78 "easymesh/easymesh-parser.y"

#include "easymesh/easymesh-compiler.h"

#undef yylex
#define yylex mc.m_lexer->lex

/* HACK: workaround for Bison who insists on using exceptions */
#define try if (true)
#define catch(...) if (false)
#define throw (void)0


/* Line 316 of lalr1.cc  */
#line 87 "generated/easymesh-parser.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace lol {

/* Line 379 of lalr1.cc  */
#line 153 "generated/easymesh-parser.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  EasyMeshParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  EasyMeshParser::EasyMeshParser (class EasyMeshCompiler& mc_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      mc (mc_yyarg)
  {
  }

  EasyMeshParser::~EasyMeshParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  EasyMeshParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  EasyMeshParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  EasyMeshParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  EasyMeshParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  EasyMeshParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  EasyMeshParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  EasyMeshParser::debug_level_type
  EasyMeshParser::debug_level () const
  {
    return yydebug_;
  }

  void
  EasyMeshParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  EasyMeshParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 14:

/* Line 677 of lalr1.cc  */
#line 120 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgUnion();  mc.m_mesh.CloseBrace(); }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 121 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgSub();    mc.m_mesh.CloseBrace(); }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 122 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgSubL();   mc.m_mesh.CloseBrace(); }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 123 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgAnd();    mc.m_mesh.CloseBrace(); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 124 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgXor();    mc.m_mesh.CloseBrace(); }
    break;

  case 19:

/* Line 677 of lalr1.cc  */
#line 128 "easymesh/easymesh-parser.y"
    { mc.m_mesh.DupAndScale(vec3::one, true); }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 132 "easymesh/easymesh-parser.y"
    { mc.m_mesh.OpenBrace(); }
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 136 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CloseBrace(); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 140 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 141 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 142 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor(vec4(v) * (1.f / 255.f)); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 145 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 146 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 147 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor2(vec4(v) * (1.f / 255.f)); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 153 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].fval), 0.f, 0.f)); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 154 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, (yysemantic_stack_[(2) - (2)].fval), 0.f)); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 155 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, 0.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 156 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 157 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 158 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 159 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 160 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 161 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Rotate((yysemantic_stack_[(5) - (2)].fval), vec3((yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 162 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Rotate((yysemantic_stack_[(3) - (2)].fval), vec3((yysemantic_stack_[(3) - (3)].vval)[0], (yysemantic_stack_[(3) - (3)].vval)[1], (yysemantic_stack_[(3) - (3)].vval)[2])); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 163 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RadialJitter((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 164 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 165 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 166 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 167 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 43:

/* Line 677 of lalr1.cc  */
#line 168 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 44:

/* Line 677 of lalr1.cc  */
#line 169 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 45:

/* Line 677 of lalr1.cc  */
#line 170 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 46:

/* Line 677 of lalr1.cc  */
#line 171 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 47:

/* Line 677 of lalr1.cc  */
#line 172 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 48:

/* Line 677 of lalr1.cc  */
#line 173 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 49:

/* Line 677 of lalr1.cc  */
#line 174 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 50:

/* Line 677 of lalr1.cc  */
#line 175 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 51:

/* Line 677 of lalr1.cc  */
#line 176 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 52:

/* Line 677 of lalr1.cc  */
#line 177 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 53:

/* Line 677 of lalr1.cc  */
#line 178 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 54:

/* Line 677 of lalr1.cc  */
#line 179 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 55:

/* Line 677 of lalr1.cc  */
#line 180 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 56:

/* Line 677 of lalr1.cc  */
#line 181 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 57:

/* Line 677 of lalr1.cc  */
#line 182 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 58:

/* Line 677 of lalr1.cc  */
#line 183 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 59:

/* Line 677 of lalr1.cc  */
#line 184 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 60:

/* Line 677 of lalr1.cc  */
#line 185 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 61:

/* Line 677 of lalr1.cc  */
#line 186 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 62:

/* Line 677 of lalr1.cc  */
#line 187 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 63:

/* Line 677 of lalr1.cc  */
#line 188 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 64:

/* Line 677 of lalr1.cc  */
#line 189 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 65:

/* Line 677 of lalr1.cc  */
#line 190 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 66:

/* Line 677 of lalr1.cc  */
#line 191 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 67:

/* Line 677 of lalr1.cc  */
#line 192 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 68:

/* Line 677 of lalr1.cc  */
#line 193 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 69:

/* Line 677 of lalr1.cc  */
#line 194 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 70:

/* Line 677 of lalr1.cc  */
#line 195 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 71:

/* Line 677 of lalr1.cc  */
#line 196 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 72:

/* Line 677 of lalr1.cc  */
#line 197 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 73:

/* Line 677 of lalr1.cc  */
#line 198 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 74:

/* Line 677 of lalr1.cc  */
#line 199 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 75:

/* Line 677 of lalr1.cc  */
#line 200 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 76:

/* Line 677 of lalr1.cc  */
#line 201 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 77:

/* Line 677 of lalr1.cc  */
#line 202 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 78:

/* Line 677 of lalr1.cc  */
#line 203 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 79:

/* Line 677 of lalr1.cc  */
#line 204 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 80:

/* Line 677 of lalr1.cc  */
#line 205 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 81:

/* Line 677 of lalr1.cc  */
#line 206 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), 1.f, 1.f)); }
    break;

  case 82:

/* Line 677 of lalr1.cc  */
#line 207 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, (yysemantic_stack_[(2) - (2)].fval), 1.f)); }
    break;

  case 83:

/* Line 677 of lalr1.cc  */
#line 208 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, 1.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 84:

/* Line 677 of lalr1.cc  */
#line 209 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 85:

/* Line 677 of lalr1.cc  */
#line 210 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 86:

/* Line 677 of lalr1.cc  */
#line 211 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 87:

/* Line 677 of lalr1.cc  */
#line 212 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorX(); }
    break;

  case 88:

/* Line 677 of lalr1.cc  */
#line 213 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorY(); }
    break;

  case 89:

/* Line 677 of lalr1.cc  */
#line 214 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorZ(); }
    break;

  case 90:

/* Line 677 of lalr1.cc  */
#line 215 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Chamfer((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 91:

/* Line 677 of lalr1.cc  */
#line 216 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SplitTriangles((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 92:

/* Line 677 of lalr1.cc  */
#line 217 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SmoothMesh((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].ival), (yysemantic_stack_[(4) - (4)].ival)); }
    break;

  case 93:

/* Line 677 of lalr1.cc  */
#line 218 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleScaleWinding(); }
    break;

  case 94:

/* Line 677 of lalr1.cc  */
#line 222 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(8) - (2)].ival), (yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (4)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (6)].bval), (yysemantic_stack_[(8) - (7)].bval), (yysemantic_stack_[(8) - (8)].bval)); }
    break;

  case 95:

/* Line 677 of lalr1.cc  */
#line 223 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(7) - (2)].ival), (yysemantic_stack_[(7) - (3)].fval), (yysemantic_stack_[(7) - (4)].fval), (yysemantic_stack_[(7) - (5)].fval), (yysemantic_stack_[(7) - (6)].bval), (yysemantic_stack_[(7) - (7)].bval)); }
    break;

  case 96:

/* Line 677 of lalr1.cc  */
#line 224 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].fval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 97:

/* Line 677 of lalr1.cc  */
#line 225 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 98:

/* Line 677 of lalr1.cc  */
#line 226 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSphere((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 99:

/* Line 677 of lalr1.cc  */
#line 227 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCapsule((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 100:

/* Line 677 of lalr1.cc  */
#line 228 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendTorus((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 101:

/* Line 677 of lalr1.cc  */
#line 229 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 102:

/* Line 677 of lalr1.cc  */
#line 230 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 103:

/* Line 677 of lalr1.cc  */
#line 231 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 104:

/* Line 677 of lalr1.cc  */
#line 232 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 105:

/* Line 677 of lalr1.cc  */
#line 233 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 106:

/* Line 677 of lalr1.cc  */
#line 234 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 107:

/* Line 677 of lalr1.cc  */
#line 235 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 108:

/* Line 677 of lalr1.cc  */
#line 236 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 109:

/* Line 677 of lalr1.cc  */
#line 237 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 110:

/* Line 677 of lalr1.cc  */
#line 238 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 111:

/* Line 677 of lalr1.cc  */
#line 239 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 112:

/* Line 677 of lalr1.cc  */
#line 240 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].bval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 113:

/* Line 677 of lalr1.cc  */
#line 241 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 114:

/* Line 677 of lalr1.cc  */
#line 242 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 115:

/* Line 677 of lalr1.cc  */
#line 243 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 116:

/* Line 677 of lalr1.cc  */
#line 244 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 117:

/* Line 677 of lalr1.cc  */
#line 245 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].bval)); }
    break;

  case 118:

/* Line 677 of lalr1.cc  */
#line 246 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 119:

/* Line 677 of lalr1.cc  */
#line 247 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 120:

/* Line 677 of lalr1.cc  */
#line 248 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 121:

/* Line 677 of lalr1.cc  */
#line 249 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 122:

/* Line 677 of lalr1.cc  */
#line 250 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 123:

/* Line 677 of lalr1.cc  */
#line 251 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(11) - (2)].ival), (yysemantic_stack_[(11) - (3)].fval), (yysemantic_stack_[(11) - (4)].fval), (yysemantic_stack_[(11) - (5)].fval), (yysemantic_stack_[(11) - (6)].fval), (yysemantic_stack_[(11) - (7)].fval), (yysemantic_stack_[(11) - (8)].fval), (yysemantic_stack_[(11) - (9)].fval), (yysemantic_stack_[(11) - (10)].fval), (yysemantic_stack_[(11) - (11)].bval)); }
    break;

  case 124:

/* Line 677 of lalr1.cc  */
#line 252 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(10) - (2)].ival), (yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (4)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (6)].fval), (yysemantic_stack_[(10) - (7)].fval), (yysemantic_stack_[(10) - (8)].fval), (yysemantic_stack_[(10) - (9)].fval), (yysemantic_stack_[(10) - (10)].fval)); }
    break;

  case 125:

/* Line 677 of lalr1.cc  */
#line 253 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(9) - (2)].ival), (yysemantic_stack_[(9) - (3)].fval), (yysemantic_stack_[(9) - (4)].fval), (yysemantic_stack_[(9) - (5)].fval), (yysemantic_stack_[(9) - (6)].fval), (yysemantic_stack_[(9) - (7)].fval), (yysemantic_stack_[(9) - (8)].fval), (yysemantic_stack_[(9) - (9)].fval)); }
    break;

  case 126:

/* Line 677 of lalr1.cc  */
#line 258 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 127:

/* Line 677 of lalr1.cc  */
#line 259 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 128:

/* Line 677 of lalr1.cc  */
#line 260 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (float)(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 129:

/* Line 677 of lalr1.cc  */
#line 261 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(float)(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 130:

/* Line 677 of lalr1.cc  */
#line 265 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 131:

/* Line 677 of lalr1.cc  */
#line 266 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 132:

/* Line 677 of lalr1.cc  */
#line 267 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (int)(yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 133:

/* Line 677 of lalr1.cc  */
#line 268 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(int)(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 134:

/* Line 677 of lalr1.cc  */
#line 273 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 135:

/* Line 677 of lalr1.cc  */
#line 274 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(5) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(5) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(5) - (4)].fval); }
    break;

  case 136:

/* Line 677 of lalr1.cc  */
#line 278 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[3] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 137:

/* Line 677 of lalr1.cc  */
#line 279 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(6) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(6) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(6) - (4)].fval); (yyval.vval)[3] = (yysemantic_stack_[(6) - (5)].fval); }
    break;

  case 138:

/* Line 677 of lalr1.cc  */
#line 284 "easymesh/easymesh-parser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 139:

/* Line 677 of lalr1.cc  */
#line 285 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 140:

/* Line 677 of lalr1.cc  */
#line 286 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].fval); }
    break;



/* Line 677 of lalr1.cc  */
#line 1337 "generated/easymesh-parser.cpp"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  EasyMeshParser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char EasyMeshParser::yypact_ninf_ = -118;
  const short int
  EasyMeshParser::yypact_[] =
  {
       353,    -1,    78,     3,     3,     3,     3,     3,     3,     3,
       3,     3,  -118,     3,     3,     3,     3,     3,     3,     3,
       3,     3,  -118,     3,     3,     3,     3,     3,     3,     3,
       3,     3,  -118,     7,     3,     7,  -118,     3,    82,    82,
    -118,   -57,   -57,   -57,   -57,   -57,     3,    82,     7,     7,
       7,    82,    82,    82,    82,    82,     3,     3,    82,    82,
    -118,    20,    30,   353,  -118,   418,  -118,   -33,   353,  -118,
    -118,  -118,  -118,  -118,  -118,    96,     3,     3,  -118,  -118,
       3,  -118,  -118,  -118,     3,     3,     3,     3,     3,     3,
    -118,  -118,  -118,     3,     3,     3,     3,     3,     3,  -118,
    -118,  -118,     3,     3,     3,     3,     3,     3,  -118,     3,
       3,  -118,     7,     3,  -118,  -118,  -118,  -118,    96,  -118,
      82,   353,   353,   353,   353,   353,  -118,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,   -24,
     -24,     3,     3,  -118,  -118,  -118,  -118,   353,   -12,  -118,
    -118,    96,  -118,  -118,   -26,     3,     3,     3,  -118,     3,
       3,  -118,  -118,     3,  -118,     3,     3,  -118,  -118,     3,
    -118,     3,     3,  -118,  -118,   -11,     3,     3,  -118,     3,
    -118,  -118,    82,    -7,    11,    12,    13,    21,     3,     3,
    -118,     3,  -118,     3,  -118,  -118,     3,     3,     3,   -24,
    -118,  -118,  -118,  -118,  -118,     3,     3,   -12,  -118,  -118,
    -118,  -118,  -118,     3,     3,     3,   -24,   -24,  -118,   -24,
     -24,  -118,   -24,   -24,  -118,  -118,     3,  -118,     3,  -118,
    -118,  -118,  -118,  -118,  -118,  -118,     3,     3,     3,     3,
    -118,   -24,     3,  -118,     3,  -118,  -118,     3,  -118,  -118,
    -118,  -118,  -118,  -118,  -118,  -118,   -29,  -118,   -24,  -118,
    -118,  -118,   -24,  -118,     3,   -14,  -118,   -24,  -118,     3,
    -118,   -24,     3,  -118,     3,     3,   -24,  -118
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  EasyMeshParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,    93,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,     0,     0,     3,     5,     7,    12,     0,     0,     9,
      10,    11,   126,   128,    24,     0,     0,     0,    23,    27,
       0,    26,    28,    33,     0,    49,     0,     0,    70,    72,
      81,    29,    34,     0,    51,     0,     0,    74,    76,    82,
      30,    35,     0,    53,     0,     0,    78,    80,    83,     0,
       0,    32,     0,    86,    85,    38,   132,   130,     0,    91,
       0,     0,     0,     0,     0,     0,    90,     0,   103,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   120,
     122,     0,     0,     1,     2,     4,     8,     0,     0,   126,
     128,     0,   127,   129,     0,     0,     0,    41,    48,    56,
      64,    69,    71,    44,    50,    59,    66,    73,    75,    47,
      52,    62,    68,    77,    79,     0,     0,     0,    37,     0,
     133,   131,     0,     0,     0,     0,     0,     0,     0,     0,
     104,   107,   108,   110,   111,    98,     0,     0,     0,   118,
     140,   139,   138,   119,   121,     0,     0,     0,    21,     6,
     127,   129,   136,     0,     0,     0,    40,    55,    63,    43,
      58,    65,    46,    61,    67,   134,     0,    31,     0,    84,
      92,    14,    15,    16,    17,    18,     0,   102,     0,     0,
      99,   114,   116,   117,     0,   100,    13,     0,    22,    25,
      39,    54,    42,    57,    45,    60,     0,    36,    97,   101,
     106,   109,   113,   115,     0,     0,   135,    96,   112,     0,
     137,    95,     0,    94,     0,   125,   124,   123
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yypgoto_[] =
  {
      -118,  -118,    -4,  -118,    23,  -118,  -118,  -118,   137,  -117,
    -118,  -118,  -118,    -2,   163,     1,    95,    79
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yydefgoto_[] =
  {
        -1,    61,    62,    63,    64,    65,    66,    67,    68,   209,
      69,    70,    71,    77,   119,   111,    78,   203
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char EasyMeshParser::yytable_ninf_ = -1;
  const unsigned short int
  EasyMeshParser::yytable_[] =
  {
        80,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      60,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     143,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     144,   110,   112,   113,   147,   115,   114,    72,    73,   200,
     201,   202,   266,    75,   126,   212,   128,   130,   132,   129,
     131,   133,    72,    73,   139,   140,   208,   270,    75,   145,
     225,   231,    72,    73,   148,    74,    72,    73,    75,    76,
      72,    73,    75,   152,   154,   155,    75,   109,   156,   232,
     233,   234,   157,   158,   159,   160,   161,   162,   146,   235,
     246,   163,   164,   165,   166,   167,   168,    81,     0,     0,
     169,   170,   171,   172,   173,   174,     0,   175,   176,     0,
     177,   179,     0,   178,     0,     0,   180,   183,   184,   185,
     186,   187,     0,     0,     0,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,     0,     0,   205,
     206,    72,    73,   207,    79,   116,   117,    75,    76,   210,
       0,   118,   213,   214,   215,   216,     0,   217,   218,   149,
     150,   219,     0,   220,   221,   151,     0,   222,     0,   223,
     224,     0,     0,   226,   227,   228,     0,   229,   121,   122,
     123,   124,   125,     0,     0,     0,   236,   237,     0,   238,
       0,   239,     0,     0,   240,   241,   242,     0,     0,     0,
       0,     0,   120,   244,   245,     0,     0,     0,     0,     0,
     127,   247,   248,   249,   134,   135,   136,   137,   138,   204,
       0,   141,   142,     0,   256,     0,   257,     0,     0,     0,
       0,     0,     0,     0,   258,   259,   260,   261,   153,     0,
     263,     0,   264,     0,     0,   265,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,   272,     0,     0,
     274,     0,   275,   276,     0,     0,     0,     0,   243,     0,
       0,   181,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   250,   251,     0,   252,   253,
       0,   254,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   211,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,   268,     0,     0,     0,   230,   271,     0,     0,     0,
     273,     0,     0,     0,     0,   277,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,     0,     0,     0,     0,     0,
      60,     1,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59
  };

  /* YYCHECK.  */
  const short int
  EasyMeshParser::yycheck_[] =
  {
         2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      67,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       0,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       0,    33,    34,    35,    67,    37,    35,    63,    64,    63,
      64,    65,    71,    69,    46,    71,    48,    49,    50,    48,
      49,    50,    63,    64,    56,    57,    68,    71,    69,    63,
      71,    68,    63,    64,    68,    66,    63,    64,    69,    70,
      63,    64,    69,    75,    76,    77,    69,    70,    80,    68,
      68,    68,    84,    85,    86,    87,    88,    89,    65,    68,
     207,    93,    94,    95,    96,    97,    98,     2,    -1,    -1,
     102,   103,   104,   105,   106,   107,    -1,   109,   110,    -1,
     112,   113,    -1,   112,    -1,    -1,   118,   121,   122,   123,
     124,   125,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,   141,
     142,    63,    64,   147,    66,    63,    64,    69,    70,   151,
      -1,    69,   154,   155,   156,   157,    -1,   159,   160,    63,
      64,   163,    -1,   165,   166,    69,    -1,   169,    -1,   171,
     172,    -1,    -1,   175,   176,   177,    -1,   179,    41,    42,
      43,    44,    45,    -1,    -1,    -1,   188,   189,    -1,   191,
      -1,   193,    -1,    -1,   196,   197,   198,    -1,    -1,    -1,
      -1,    -1,    39,   205,   206,    -1,    -1,    -1,    -1,    -1,
      47,   213,   214,   215,    51,    52,    53,    54,    55,   140,
      -1,    58,    59,    -1,   226,    -1,   228,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   236,   237,   238,   239,    75,    -1,
     242,    -1,   244,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   264,    -1,    -1,    -1,    -1,   269,    -1,    -1,
     272,    -1,   274,   275,    -1,    -1,    -1,    -1,   199,    -1,
      -1,   118,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,   219,   220,
      -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
     241,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   258,    -1,    -1,
      -1,   262,    -1,    -1,    -1,   182,   267,    -1,    -1,    -1,
     271,    -1,    -1,    -1,    -1,   276,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  EasyMeshParser::yystos_[] =
  {
         0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      67,    73,    74,    75,    76,    77,    78,    79,    80,    82,
      83,    84,    63,    64,    66,    69,    70,    85,    88,    66,
      85,    88,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    70,
      85,    87,    85,    85,    87,    85,    63,    64,    69,    86,
      86,    80,    80,    80,    80,    80,    85,    86,    85,    87,
      85,    87,    85,    87,    86,    86,    86,    86,    86,    85,
      85,    86,    86,     0,     0,    74,    76,    67,    74,    63,
      64,    69,    85,    86,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    87,    85,
      85,    86,    86,    74,    74,    74,    74,    74,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      63,    64,    65,    89,    89,    85,    85,    74,    68,    81,
      85,    86,    71,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    71,    85,    85,    85,    85,
      86,    68,    68,    68,    68,    68,    85,    85,    85,    85,
      85,    85,    85,    89,    85,    85,    81,    85,    85,    85,
      89,    89,    89,    89,    89,    89,    85,    85,    85,    85,
      85,    85,    89,    85,    85,    85,    71,    89,    89,    85,
      71,    89,    85,    89,    85,    85,    85,    89
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  EasyMeshParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    91,    93,    45,
      40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  EasyMeshParser::yyr1_[] =
  {
         0,    72,    73,    74,    74,    75,    75,    76,    76,    77,
      77,    77,    77,    77,    78,    78,    78,    78,    78,    79,
      80,    81,    82,    82,    82,    82,    82,    82,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    85,    85,    85,    85,
      86,    86,    86,    86,    87,    87,    88,    88,    89,    89,
      89
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  EasyMeshParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     3,     1,     2,     1,
       1,     1,     1,     4,     4,     4,     4,     4,     4,     1,
       1,     1,     5,     2,     2,     5,     2,     2,     2,     2,
       2,     4,     2,     2,     2,     2,     5,     3,     2,     5,
       4,     3,     5,     4,     3,     5,     4,     3,     3,     2,
       3,     2,     3,     2,     5,     4,     3,     5,     4,     3,
       5,     4,     3,     4,     3,     4,     3,     4,     3,     3,
       2,     3,     2,     3,     2,     3,     2,     3,     2,     3,
       2,     2,     2,     2,     4,     2,     2,     1,     1,     1,
       2,     2,     4,     1,     8,     7,     6,     5,     3,     4,
       4,     5,     4,     2,     3,     2,     5,     3,     3,     5,
       3,     3,     6,     5,     4,     5,     4,     4,     3,     3,
       2,     3,     2,    11,    10,     9,     1,     2,     1,     2,
       1,     2,     1,     2,     3,     5,     3,     6,     1,     1,
       1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const EasyMeshParser::yytname_[] =
  {
    "T_END", "error", "$undefined", "T_COLOR", "T_BCOLOR", "T_TRANSLATEX",
  "T_ROTATEX", "T_TAPERX", "T_TWISTX", "T_SHEARX", "T_STRETCHX",
  "T_BENDXY", "T_BENDXZ", "T_SCALEX", "T_MIRRORX", "T_TRANSLATEY",
  "T_ROTATEY", "T_TAPERY", "T_TWISTY", "T_SHEARY", "T_STRETCHY",
  "T_BENDYX", "T_BENDYZ", "T_SCALEY", "T_MIRRORY", "T_TRANSLATEZ",
  "T_ROTATEZ", "T_TAPERZ", "T_TWISTZ", "T_SHEARZ", "T_STRETCHZ",
  "T_BENDZX", "T_BENDZY", "T_SCALEZ", "T_MIRRORZ", "T_TRANSLATE",
  "T_ROTATE", "T_SCALE", "T_TOGGLESCALEWINDING", "T_RADIALJITTER",
  "T_SPLITTRIANGLE", "T_SMOOTHMESH", "T_DUPLICATE", "T_CSGUNION",
  "T_CSGSUBSTRACT", "T_CSGSUBSTRACTLOSS", "T_CSGAND", "T_CSGXOR",
  "T_CHAMFER", "T_CYLINDER", "T_BOX", "T_SMOOTHCHAMFBOX", "T_FLATCHAMFBOX",
  "T_SPHERE", "T_CAPSULE", "T_STAR", "T_EXPANDEDSTAR", "T_DISC",
  "T_TRIANGLE", "T_QUAD", "T_COG", "T_TORUS", "T_ERROR", "F_NUMBER",
  "I_NUMBER", "BOOLEAN", "COLOR", "'['", "']'", "'-'", "'('", "')'",
  "$accept", "mesh_description", "mesh_expression_list", "mesh_expression",
  "mesh_command_list", "mesh_command", "post_brace_command",
  "pre_brace_command", "mesh_open", "mesh_close", "color_command",
  "transform_command", "primitive_command", "fv", "iv", "v3", "v4", "bv", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const EasyMeshParser::rhs_number_type
  EasyMeshParser::yyrhs_[] =
  {
        73,     0,    -1,    74,     0,    -1,    75,    -1,    75,    74,
      -1,    76,    -1,    80,    74,    81,    -1,    77,    -1,    77,
      76,    -1,    82,    -1,    83,    -1,    84,    -1,    78,    -1,
      79,    67,    74,    81,    -1,    43,    80,    74,    68,    -1,
      44,    80,    74,    68,    -1,    45,    80,    74,    68,    -1,
      46,    80,    74,    68,    -1,    47,    80,    74,    68,    -1,
      42,    -1,    67,    -1,    68,    -1,     3,    85,    85,    85,
      85,    -1,     3,    88,    -1,     3,    66,    -1,     4,    85,
      85,    85,    85,    -1,     4,    88,    -1,     4,    66,    -1,
       5,    85,    -1,    15,    85,    -1,    25,    85,    -1,    35,
      85,    85,    85,    -1,    35,    87,    -1,     6,    85,    -1,
      16,    85,    -1,    26,    85,    -1,    36,    85,    85,    85,
      85,    -1,    36,    85,    87,    -1,    39,    85,    -1,     7,
      85,    85,    85,    89,    -1,     7,    85,    85,    85,    -1,
       7,    85,    85,    -1,    17,    85,    85,    85,    89,    -1,
      17,    85,    85,    85,    -1,    17,    85,    85,    -1,    27,
      85,    85,    85,    89,    -1,    27,    85,    85,    85,    -1,
      27,    85,    85,    -1,     8,    85,    85,    -1,     8,    85,
      -1,    18,    85,    85,    -1,    18,    85,    -1,    28,    85,
      85,    -1,    28,    85,    -1,     9,    85,    85,    85,    89,
      -1,     9,    85,    85,    85,    -1,     9,    85,    85,    -1,
      19,    85,    85,    85,    89,    -1,    19,    85,    85,    85,
      -1,    19,    85,    85,    -1,    29,    85,    85,    85,    89,
      -1,    29,    85,    85,    85,    -1,    29,    85,    85,    -1,
      10,    85,    85,    85,    -1,    10,    85,    85,    -1,    20,
      85,    85,    85,    -1,    20,    85,    85,    -1,    30,    85,
      85,    85,    -1,    30,    85,    85,    -1,    11,    85,    85,
      -1,    11,    85,    -1,    12,    85,    85,    -1,    12,    85,
      -1,    21,    85,    85,    -1,    21,    85,    -1,    22,    85,
      85,    -1,    22,    85,    -1,    31,    85,    85,    -1,    31,
      85,    -1,    32,    85,    85,    -1,    32,    85,    -1,    13,
      85,    -1,    23,    85,    -1,    33,    85,    -1,    37,    85,
      85,    85,    -1,    37,    87,    -1,    37,    85,    -1,    14,
      -1,    24,    -1,    34,    -1,    48,    85,    -1,    40,    86,
      -1,    41,    86,    86,    86,    -1,    38,    -1,    49,    86,
      85,    85,    85,    89,    89,    89,    -1,    49,    86,    85,
      85,    85,    89,    89,    -1,    49,    86,    85,    85,    85,
      89,    -1,    49,    86,    85,    85,    85,    -1,    53,    86,
      85,    -1,    54,    86,    85,    85,    -1,    61,    86,    85,
      85,    -1,    50,    85,    85,    85,    85,    -1,    50,    85,
      85,    85,    -1,    50,    85,    -1,    50,    87,    85,    -1,
      50,    87,    -1,    51,    85,    85,    85,    85,    -1,    51,
      85,    85,    -1,    51,    87,    85,    -1,    52,    85,    85,
      85,    85,    -1,    52,    85,    85,    -1,    52,    87,    85,
      -1,    55,    86,    85,    85,    89,    89,    -1,    55,    86,
      85,    85,    89,    -1,    55,    86,    85,    85,    -1,    56,
      86,    85,    85,    85,    -1,    56,    86,    85,    85,    -1,
      57,    86,    85,    89,    -1,    57,    86,    85,    -1,    58,
      85,    89,    -1,    58,    85,    -1,    59,    85,    89,    -1,
      59,    85,    -1,    60,    86,    85,    85,    85,    85,    85,
      85,    85,    85,    89,    -1,    60,    86,    85,    85,    85,
      85,    85,    85,    85,    85,    -1,    60,    86,    85,    85,
      85,    85,    85,    85,    85,    -1,    63,    -1,    69,    85,
      -1,    64,    -1,    69,    86,    -1,    64,    -1,    69,    86,
      -1,    63,    -1,    69,    85,    -1,    70,    85,    71,    -1,
      70,    85,    85,    85,    71,    -1,    70,    85,    71,    -1,
      70,    85,    85,    85,    85,    71,    -1,    65,    -1,    64,
      -1,    63,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  EasyMeshParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    17,    19,    22,
      24,    26,    28,    30,    35,    40,    45,    50,    55,    60,
      62,    64,    66,    72,    75,    78,    84,    87,    90,    93,
      96,    99,   104,   107,   110,   113,   116,   122,   126,   129,
     135,   140,   144,   150,   155,   159,   165,   170,   174,   178,
     181,   185,   188,   192,   195,   201,   206,   210,   216,   221,
     225,   231,   236,   240,   245,   249,   254,   258,   263,   267,
     271,   274,   278,   281,   285,   288,   292,   295,   299,   302,
     306,   309,   312,   315,   318,   323,   326,   329,   331,   333,
     335,   338,   341,   346,   348,   357,   365,   372,   378,   382,
     387,   392,   398,   403,   406,   410,   413,   419,   423,   427,
     433,   437,   441,   448,   454,   459,   465,   470,   475,   479,
     483,   486,   490,   493,   505,   516,   526,   528,   531,   533,
     536,   538,   541,   543,   546,   550,   556,   560,   567,   569,
     571
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  EasyMeshParser::yyrline_[] =
  {
         0,    93,    93,    97,    98,   102,   103,   107,   108,   112,
     113,   114,   115,   116,   120,   121,   122,   123,   124,   128,
     132,   136,   140,   141,   142,   145,   146,   147,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   258,   259,   260,   261,
     265,   266,   267,   268,   273,   274,   278,   279,   284,   285,
     286
  };

  // Print the state stack on the debug stream.
  void
  EasyMeshParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  EasyMeshParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  EasyMeshParser::token_number_type
  EasyMeshParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      70,    71,     2,     2,     2,    69,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    67,     2,    68,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int EasyMeshParser::yyeof_ = 0;
  const int EasyMeshParser::yylast_ = 479;
  const int EasyMeshParser::yynnts_ = 18;
  const int EasyMeshParser::yyempty_ = -2;
  const int EasyMeshParser::yyfinal_ = 143;
  const int EasyMeshParser::yyterror_ = 1;
  const int EasyMeshParser::yyerrcode_ = 256;
  const int EasyMeshParser::yyntokens_ = 72;

  const unsigned int EasyMeshParser::yyuser_token_number_max_ = 321;
  const EasyMeshParser::token_number_type EasyMeshParser::yyundef_token_ = 2;


} // lol

/* Line 1053 of lalr1.cc  */
#line 2061 "generated/easymesh-parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 289 "easymesh/easymesh-parser.y"


void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}


