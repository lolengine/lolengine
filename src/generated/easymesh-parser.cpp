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
#line 125 "easymesh/easymesh-parser.y"
    { mc.m_mesh.LoopEnd(); }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 129 "easymesh/easymesh-parser.y"
    { mc.m_mesh.LoopStart((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 131 "easymesh/easymesh-parser.y"
    { mc.m_mesh.DupAndScale(vec3::one, true); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 135 "easymesh/easymesh-parser.y"
    { mc.m_mesh.OpenBrace(); }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 139 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CloseBrace(); }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 143 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 144 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 145 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor(vec4(v) * (1.f / 255.f)); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 148 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 149 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 150 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor2(vec4(v) * (1.f / 255.f)); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 153 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetVertColor(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 154 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetVertColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 155 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetVertColor(vec4(v) * (1.f / 255.f)); }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 161 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].fval), 0.f, 0.f)); }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 162 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, (yysemantic_stack_[(2) - (2)].fval), 0.f)); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 163 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, 0.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 164 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 165 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 166 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 167 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 168 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 169 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Rotate((yysemantic_stack_[(5) - (2)].fval), vec3((yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 170 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Rotate((yysemantic_stack_[(3) - (2)].fval), vec3((yysemantic_stack_[(3) - (3)].vval)[0], (yysemantic_stack_[(3) - (3)].vval)[1], (yysemantic_stack_[(3) - (3)].vval)[2])); }
    break;

  case 43:

/* Line 677 of lalr1.cc  */
#line 171 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RadialJitter((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 44:

/* Line 677 of lalr1.cc  */
#line 172 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 45:

/* Line 677 of lalr1.cc  */
#line 173 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 46:

/* Line 677 of lalr1.cc  */
#line 174 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 47:

/* Line 677 of lalr1.cc  */
#line 175 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 48:

/* Line 677 of lalr1.cc  */
#line 176 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 49:

/* Line 677 of lalr1.cc  */
#line 177 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 50:

/* Line 677 of lalr1.cc  */
#line 178 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 51:

/* Line 677 of lalr1.cc  */
#line 179 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 52:

/* Line 677 of lalr1.cc  */
#line 180 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 53:

/* Line 677 of lalr1.cc  */
#line 181 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 54:

/* Line 677 of lalr1.cc  */
#line 182 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 55:

/* Line 677 of lalr1.cc  */
#line 183 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 56:

/* Line 677 of lalr1.cc  */
#line 184 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 57:

/* Line 677 of lalr1.cc  */
#line 185 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 58:

/* Line 677 of lalr1.cc  */
#line 186 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 59:

/* Line 677 of lalr1.cc  */
#line 187 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 60:

/* Line 677 of lalr1.cc  */
#line 188 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 61:

/* Line 677 of lalr1.cc  */
#line 189 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 62:

/* Line 677 of lalr1.cc  */
#line 190 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 63:

/* Line 677 of lalr1.cc  */
#line 191 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 64:

/* Line 677 of lalr1.cc  */
#line 192 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 65:

/* Line 677 of lalr1.cc  */
#line 193 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 66:

/* Line 677 of lalr1.cc  */
#line 194 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 67:

/* Line 677 of lalr1.cc  */
#line 195 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 68:

/* Line 677 of lalr1.cc  */
#line 196 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 69:

/* Line 677 of lalr1.cc  */
#line 197 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 70:

/* Line 677 of lalr1.cc  */
#line 198 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 71:

/* Line 677 of lalr1.cc  */
#line 199 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 72:

/* Line 677 of lalr1.cc  */
#line 200 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 73:

/* Line 677 of lalr1.cc  */
#line 201 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 74:

/* Line 677 of lalr1.cc  */
#line 202 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 75:

/* Line 677 of lalr1.cc  */
#line 203 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 76:

/* Line 677 of lalr1.cc  */
#line 204 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 77:

/* Line 677 of lalr1.cc  */
#line 205 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 78:

/* Line 677 of lalr1.cc  */
#line 206 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 79:

/* Line 677 of lalr1.cc  */
#line 207 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 80:

/* Line 677 of lalr1.cc  */
#line 208 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 81:

/* Line 677 of lalr1.cc  */
#line 209 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 82:

/* Line 677 of lalr1.cc  */
#line 210 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 83:

/* Line 677 of lalr1.cc  */
#line 211 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 84:

/* Line 677 of lalr1.cc  */
#line 212 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 85:

/* Line 677 of lalr1.cc  */
#line 213 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 86:

/* Line 677 of lalr1.cc  */
#line 214 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), 1.f, 1.f)); }
    break;

  case 87:

/* Line 677 of lalr1.cc  */
#line 215 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, (yysemantic_stack_[(2) - (2)].fval), 1.f)); }
    break;

  case 88:

/* Line 677 of lalr1.cc  */
#line 216 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, 1.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 89:

/* Line 677 of lalr1.cc  */
#line 217 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 90:

/* Line 677 of lalr1.cc  */
#line 218 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 91:

/* Line 677 of lalr1.cc  */
#line 219 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 92:

/* Line 677 of lalr1.cc  */
#line 220 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorX(); }
    break;

  case 93:

/* Line 677 of lalr1.cc  */
#line 221 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorY(); }
    break;

  case 94:

/* Line 677 of lalr1.cc  */
#line 222 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorZ(); }
    break;

  case 95:

/* Line 677 of lalr1.cc  */
#line 223 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Chamfer((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 96:

/* Line 677 of lalr1.cc  */
#line 224 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SplitTriangles((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 97:

/* Line 677 of lalr1.cc  */
#line 225 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SmoothMesh((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].ival), (yysemantic_stack_[(4) - (4)].ival)); }
    break;

  case 98:

/* Line 677 of lalr1.cc  */
#line 226 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleScaleWinding(); }
    break;

  case 99:

/* Line 677 of lalr1.cc  */
#line 230 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(8) - (2)].ival), (yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (4)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (6)].bval), (yysemantic_stack_[(8) - (7)].bval), (yysemantic_stack_[(8) - (8)].bval)); }
    break;

  case 100:

/* Line 677 of lalr1.cc  */
#line 231 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(7) - (2)].ival), (yysemantic_stack_[(7) - (3)].fval), (yysemantic_stack_[(7) - (4)].fval), (yysemantic_stack_[(7) - (5)].fval), (yysemantic_stack_[(7) - (6)].bval), (yysemantic_stack_[(7) - (7)].bval)); }
    break;

  case 101:

/* Line 677 of lalr1.cc  */
#line 232 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].fval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 102:

/* Line 677 of lalr1.cc  */
#line 233 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 103:

/* Line 677 of lalr1.cc  */
#line 234 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSphere((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 104:

/* Line 677 of lalr1.cc  */
#line 235 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCapsule((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 105:

/* Line 677 of lalr1.cc  */
#line 236 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendTorus((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 106:

/* Line 677 of lalr1.cc  */
#line 237 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 107:

/* Line 677 of lalr1.cc  */
#line 238 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 108:

/* Line 677 of lalr1.cc  */
#line 239 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 109:

/* Line 677 of lalr1.cc  */
#line 240 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 110:

/* Line 677 of lalr1.cc  */
#line 241 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 111:

/* Line 677 of lalr1.cc  */
#line 242 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 112:

/* Line 677 of lalr1.cc  */
#line 243 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 113:

/* Line 677 of lalr1.cc  */
#line 244 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 114:

/* Line 677 of lalr1.cc  */
#line 245 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 115:

/* Line 677 of lalr1.cc  */
#line 246 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 116:

/* Line 677 of lalr1.cc  */
#line 247 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 117:

/* Line 677 of lalr1.cc  */
#line 248 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].bval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 118:

/* Line 677 of lalr1.cc  */
#line 249 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 119:

/* Line 677 of lalr1.cc  */
#line 250 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 120:

/* Line 677 of lalr1.cc  */
#line 251 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 121:

/* Line 677 of lalr1.cc  */
#line 252 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 122:

/* Line 677 of lalr1.cc  */
#line 253 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].bval)); }
    break;

  case 123:

/* Line 677 of lalr1.cc  */
#line 254 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 124:

/* Line 677 of lalr1.cc  */
#line 255 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 125:

/* Line 677 of lalr1.cc  */
#line 256 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 126:

/* Line 677 of lalr1.cc  */
#line 257 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 127:

/* Line 677 of lalr1.cc  */
#line 258 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 128:

/* Line 677 of lalr1.cc  */
#line 259 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(11) - (2)].ival), (yysemantic_stack_[(11) - (3)].fval), (yysemantic_stack_[(11) - (4)].fval), (yysemantic_stack_[(11) - (5)].fval), (yysemantic_stack_[(11) - (6)].fval), (yysemantic_stack_[(11) - (7)].fval), (yysemantic_stack_[(11) - (8)].fval), (yysemantic_stack_[(11) - (9)].fval), (yysemantic_stack_[(11) - (10)].fval), (yysemantic_stack_[(11) - (11)].bval)); }
    break;

  case 129:

/* Line 677 of lalr1.cc  */
#line 260 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(10) - (2)].ival), (yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (4)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (6)].fval), (yysemantic_stack_[(10) - (7)].fval), (yysemantic_stack_[(10) - (8)].fval), (yysemantic_stack_[(10) - (9)].fval), (yysemantic_stack_[(10) - (10)].fval)); }
    break;

  case 130:

/* Line 677 of lalr1.cc  */
#line 261 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(9) - (2)].ival), (yysemantic_stack_[(9) - (3)].fval), (yysemantic_stack_[(9) - (4)].fval), (yysemantic_stack_[(9) - (5)].fval), (yysemantic_stack_[(9) - (6)].fval), (yysemantic_stack_[(9) - (7)].fval), (yysemantic_stack_[(9) - (8)].fval), (yysemantic_stack_[(9) - (9)].fval)); }
    break;

  case 131:

/* Line 677 of lalr1.cc  */
#line 266 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 132:

/* Line 677 of lalr1.cc  */
#line 267 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 133:

/* Line 677 of lalr1.cc  */
#line 268 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (float)(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 134:

/* Line 677 of lalr1.cc  */
#line 269 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(float)(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 135:

/* Line 677 of lalr1.cc  */
#line 273 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 136:

/* Line 677 of lalr1.cc  */
#line 274 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 137:

/* Line 677 of lalr1.cc  */
#line 275 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (int)(yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 138:

/* Line 677 of lalr1.cc  */
#line 276 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(int)(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 139:

/* Line 677 of lalr1.cc  */
#line 281 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 140:

/* Line 677 of lalr1.cc  */
#line 282 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(5) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(5) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(5) - (4)].fval); }
    break;

  case 141:

/* Line 677 of lalr1.cc  */
#line 286 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[3] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 142:

/* Line 677 of lalr1.cc  */
#line 287 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(6) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(6) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(6) - (4)].fval); (yyval.vval)[3] = (yysemantic_stack_[(6) - (5)].fval); }
    break;

  case 143:

/* Line 677 of lalr1.cc  */
#line 292 "easymesh/easymesh-parser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 144:

/* Line 677 of lalr1.cc  */
#line 293 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 145:

/* Line 677 of lalr1.cc  */
#line 294 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].fval); }
    break;



/* Line 677 of lalr1.cc  */
#line 1374 "generated/easymesh-parser.cpp"
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
  const short int EasyMeshParser::yypact_ninf_ = -153;
  const short int
  EasyMeshParser::yypact_[] =
  {
       426,   104,    -5,     5,    86,   125,   125,   125,   125,   125,
     125,   125,   125,   125,  -153,   125,   125,   125,   125,   125,
     125,   125,   125,   125,  -153,   125,   125,   125,   125,   125,
     125,   125,   125,   125,  -153,    78,   125,    78,  -153,   125,
     104,   104,  -153,   -58,   -58,   -58,   -58,   -58,   125,   104,
      78,    78,    78,   104,   104,   104,   104,   104,   125,   125,
     104,   104,  -153,    21,    35,   426,  -153,   493,  -153,   -38,
     -32,   426,  -153,  -153,  -153,  -153,  -153,   127,  -153,  -153,
    -153,  -153,   127,   125,   125,  -153,  -153,   125,  -153,  -153,
     125,  -153,  -153,  -153,   125,   125,   125,   125,   125,   125,
    -153,  -153,  -153,   125,   125,   125,   125,   125,   125,  -153,
    -153,  -153,   125,   125,   125,   125,   125,   125,  -153,   125,
     125,  -153,    78,   125,  -153,  -153,  -153,   104,   426,   426,
     426,   426,   426,  -153,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   -25,   -25,   125,   125,
    -153,  -153,  -153,  -153,   426,   426,   -20,  -153,  -153,   127,
    -153,  -153,  -153,  -153,   -27,   125,   125,   125,   125,  -153,
     125,   125,  -153,  -153,   125,  -153,   125,   125,  -153,  -153,
     125,  -153,   125,   125,  -153,  -153,   -14,   125,   125,  -153,
     125,   104,    -8,    -2,    -1,     2,     8,   125,   125,  -153,
     125,  -153,   125,  -153,  -153,   125,   125,   125,   -25,  -153,
    -153,  -153,  -153,  -153,   125,   125,    12,   -20,  -153,  -153,
    -153,  -153,  -153,   125,   125,   125,   125,   -25,   -25,  -153,
     -25,   -25,  -153,   -25,   -25,  -153,  -153,   125,  -153,   125,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,   125,   125,   125,
     125,  -153,   -25,   125,  -153,   125,  -153,  -153,  -153,   125,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,   -30,
    -153,   -25,  -153,  -153,  -153,   -25,  -153,   125,    10,  -153,
     -25,  -153,   125,  -153,   -25,   125,  -153,   125,   125,   -25,
    -153
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  EasyMeshParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,     0,    98,     0,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     3,     5,     7,    12,     0,
       0,     0,     9,    10,    11,   137,   135,     0,    20,   131,
     133,    26,     0,     0,     0,    25,    29,     0,    28,    32,
       0,    31,    33,    38,     0,    54,     0,     0,    75,    77,
      86,    34,    39,     0,    56,     0,     0,    79,    81,    87,
      35,    40,     0,    58,     0,     0,    83,    85,    88,     0,
       0,    37,     0,    91,    90,    43,    96,     0,     0,     0,
       0,     0,     0,    95,     0,   108,   110,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,   127,     0,     0,
       1,     2,     4,     8,     0,     0,     0,   131,   133,     0,
     138,   136,   132,   134,     0,     0,     0,     0,    46,    53,
      61,    69,    74,    76,    49,    55,    64,    71,    78,    80,
      52,    57,    67,    73,    82,    84,     0,     0,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
     112,   113,   115,   116,   103,     0,     0,     0,   123,   145,
     144,   143,   124,   126,     0,     0,     0,     0,    23,     6,
     132,   134,   141,     0,     0,     0,     0,    45,    60,    68,
      48,    63,    70,    51,    66,    72,   139,     0,    36,     0,
      89,    97,    14,    15,    16,    17,    18,     0,   107,     0,
       0,   104,   119,   121,   122,     0,   105,    19,    13,     0,
      24,    27,    30,    44,    59,    47,    62,    50,    65,     0,
      41,   102,   106,   111,   114,   118,   120,     0,     0,   140,
     101,   117,     0,   142,   100,     0,    99,     0,   130,   129,
     128
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yypgoto_[] =
  {
      -153,  -153,    -7,  -153,    18,  -153,  -153,  -153,  -153,    83,
    -152,  -153,  -153,  -153,    -3,   237,    38,    50,    31
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yydefgoto_[] =
  {
        -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
     219,    72,    73,    74,    84,    78,   121,    85,   212
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char EasyMeshParser::yytable_ninf_ = -1;
  const unsigned short int
  EasyMeshParser::yytable_[] =
  {
        87,    90,    92,    93,    94,    95,    96,    97,    98,    99,
     100,    62,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   150,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   154,   120,   122,   123,   151,   125,   155,    79,    80,
     209,   210,   211,   279,    82,   133,   222,   135,   137,   139,
     218,    79,    80,    88,    91,   146,   147,    82,   152,   236,
      79,    80,   242,    81,   156,   258,    82,    83,   243,   244,
      79,    80,   245,    86,   160,   124,    82,    83,   246,   162,
     164,   165,   257,   283,   166,   153,     0,   167,   136,   138,
     140,   168,   169,   170,   171,   172,   173,     0,     0,     0,
     174,   175,   176,   177,   178,   179,     0,     0,     0,   180,
     181,   182,   183,   184,   185,     0,   186,   187,     0,   188,
     190,   192,   193,   194,   195,   196,   128,   129,   130,   131,
     132,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,    79,    80,   214,   215,   216,   217,    82,
     119,    79,    80,     0,    89,     0,   220,    82,    83,     0,
     189,   223,   224,   225,   226,   227,     0,   228,   229,    75,
      76,   230,     0,   231,   232,    77,     0,   233,   213,   234,
     235,     0,     0,   237,   238,   239,     0,   240,     0,     0,
      79,    80,   157,   158,   247,   248,    82,   249,   159,   250,
       0,     0,   251,   252,   253,     0,     0,     0,     0,     0,
       0,   255,   256,     0,     0,     0,     0,     0,     0,     0,
     259,   260,   261,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   269,     0,   270,     0,     0,   254,
       0,     0,     0,     0,   271,   272,   273,   274,     0,     0,
     276,     0,   277,     0,     0,     0,   278,     0,   263,   264,
       0,   265,   266,     0,   267,   268,     0,     0,     0,     0,
       0,     0,     0,     0,   282,     0,     0,   126,   127,   285,
       0,     0,   287,   275,   288,   289,   134,     0,     0,     0,
     141,   142,   143,   144,   145,     0,     0,   148,   149,     0,
       0,     0,   280,     0,     0,     0,   281,     0,     0,     0,
       0,   284,     0,     0,   161,   286,     0,     0,     0,   163,
     290,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
       0,     0,     0,     0,     0,    62,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61
  };

  /* YYCHECK.  */
  const short int
  EasyMeshParser::yycheck_[] =
  {
         3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    69,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    69,    35,    36,    37,     0,    39,    69,    65,    66,
      65,    66,    67,    73,    71,    48,    73,    50,    51,    52,
      70,    65,    66,     3,     4,    58,    59,    71,    65,    73,
      65,    66,    70,    68,    71,   217,    71,    72,    70,    70,
      65,    66,    70,    68,    77,    37,    71,    72,    70,    82,
      83,    84,    70,    73,    87,    67,    -1,    90,    50,    51,
      52,    94,    95,    96,    97,    98,    99,    -1,    -1,    -1,
     103,   104,   105,   106,   107,   108,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,    -1,   122,
     123,   128,   129,   130,   131,   132,    43,    44,    45,    46,
      47,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    65,    66,   148,   149,   154,   155,    71,
      72,    65,    66,    -1,    68,    -1,   159,    71,    72,    -1,
     122,   164,   165,   166,   167,   168,    -1,   170,   171,    65,
      66,   174,    -1,   176,   177,    71,    -1,   180,   147,   182,
     183,    -1,    -1,   186,   187,   188,    -1,   190,    -1,    -1,
      65,    66,    65,    66,   197,   198,    71,   200,    71,   202,
      -1,    -1,   205,   206,   207,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     223,   224,   225,   226,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   237,    -1,   239,    -1,    -1,   208,
      -1,    -1,    -1,    -1,   247,   248,   249,   250,    -1,    -1,
     253,    -1,   255,    -1,    -1,    -1,   259,    -1,   227,   228,
      -1,   230,   231,    -1,   233,   234,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   277,    -1,    -1,    40,    41,   282,
      -1,    -1,   285,   252,   287,   288,    49,    -1,    -1,    -1,
      53,    54,    55,    56,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,   271,    -1,    -1,    -1,   275,    -1,    -1,    -1,
      -1,   280,    -1,    -1,    77,   284,    -1,    -1,    -1,    82,
     289,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    -1,    -1,    -1,    -1,    69,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63
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
      62,    63,    69,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    85,    86,    87,    65,    66,    71,    89,    65,
      66,    68,    71,    72,    88,    91,    68,    88,    91,    68,
      88,    91,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    72,
      88,    90,    88,    88,    90,    88,    89,    89,    83,    83,
      83,    83,    83,    88,    89,    88,    90,    88,    90,    88,
      90,    89,    89,    89,    89,    89,    88,    88,    89,    89,
       0,     0,    76,    78,    69,    69,    76,    65,    66,    71,
      88,    89,    88,    89,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    90,
      88,    89,    76,    76,    76,    76,    76,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    65,
      66,    67,    92,    92,    88,    88,    76,    76,    70,    84,
      88,    89,    73,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    73,    88,    88,    88,
      88,    89,    70,    70,    70,    70,    70,    88,    88,    88,
      88,    88,    88,    88,    92,    88,    88,    70,    84,    88,
      88,    88,    88,    92,    92,    92,    92,    92,    92,    88,
      88,    88,    88,    88,    88,    92,    88,    88,    88,    73,
      92,    92,    88,    73,    92,    88,    92,    88,    88,    88,
      92
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,    91,
      93,    45,    40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  EasyMeshParser::yyr1_[] =
  {
         0,    74,    75,    76,    76,    77,    77,    78,    78,    79,
      79,    79,    79,    79,    80,    80,    80,    80,    80,    80,
      81,    82,    83,    84,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    88,    88,    88,    88,    89,    89,    89,    89,    90,
      90,    91,    91,    92,    92,    92
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  EasyMeshParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     3,     1,     2,     1,
       1,     1,     1,     4,     4,     4,     4,     4,     4,     4,
       2,     1,     1,     1,     5,     2,     2,     5,     2,     2,
       5,     2,     2,     2,     2,     2,     4,     2,     2,     2,
       2,     5,     3,     2,     5,     4,     3,     5,     4,     3,
       5,     4,     3,     3,     2,     3,     2,     3,     2,     5,
       4,     3,     5,     4,     3,     5,     4,     3,     4,     3,
       4,     3,     4,     3,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     2,     2,     2,     4,
       2,     2,     1,     1,     1,     2,     2,     4,     1,     8,
       7,     6,     5,     3,     4,     4,     5,     4,     2,     3,
       2,     5,     3,     3,     5,     3,     3,     6,     5,     4,
       5,     4,     4,     3,     3,     2,     3,     2,    11,    10,
       9,     1,     2,     1,     2,     1,     2,     1,     2,     3,
       5,     3,     6,     1,     1,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const EasyMeshParser::yytname_[] =
  {
    "T_END", "error", "$undefined", "T_LOOP", "T_COLOR", "T_BCOLOR",
  "T_VCOLOR", "T_TRANSLATEX", "T_ROTATEX", "T_TAPERX", "T_TWISTX",
  "T_SHEARX", "T_STRETCHX", "T_BENDXY", "T_BENDXZ", "T_SCALEX",
  "T_MIRRORX", "T_TRANSLATEY", "T_ROTATEY", "T_TAPERY", "T_TWISTY",
  "T_SHEARY", "T_STRETCHY", "T_BENDYX", "T_BENDYZ", "T_SCALEY",
  "T_MIRRORY", "T_TRANSLATEZ", "T_ROTATEZ", "T_TAPERZ", "T_TWISTZ",
  "T_SHEARZ", "T_STRETCHZ", "T_BENDZX", "T_BENDZY", "T_SCALEZ",
  "T_MIRRORZ", "T_TRANSLATE", "T_ROTATE", "T_SCALE",
  "T_TOGGLESCALEWINDING", "T_RADIALJITTER", "T_SPLITTRIANGLE",
  "T_SMOOTHMESH", "T_DUPLICATE", "T_CSGUNION", "T_CSGSUBSTRACT",
  "T_CSGSUBSTRACTLOSS", "T_CSGAND", "T_CSGXOR", "T_CHAMFER", "T_CYLINDER",
  "T_BOX", "T_SMOOTHCHAMFBOX", "T_FLATCHAMFBOX", "T_SPHERE", "T_CAPSULE",
  "T_STAR", "T_EXPANDEDSTAR", "T_DISC", "T_TRIANGLE", "T_QUAD", "T_COG",
  "T_TORUS", "T_ERROR", "F_NUMBER", "I_NUMBER", "BOOLEAN", "COLOR", "'['",
  "']'", "'-'", "'('", "')'", "$accept", "mesh_description",
  "mesh_expression_list", "mesh_expression", "mesh_command_list",
  "mesh_command", "post_brace_command", "doloop", "pre_brace_command",
  "mesh_open", "mesh_close", "color_command", "transform_command",
  "primitive_command", "fv", "iv", "v3", "v4", "bv", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const EasyMeshParser::rhs_number_type
  EasyMeshParser::yyrhs_[] =
  {
        75,     0,    -1,    76,     0,    -1,    77,    -1,    77,    76,
      -1,    78,    -1,    83,    76,    84,    -1,    79,    -1,    79,
      78,    -1,    85,    -1,    86,    -1,    87,    -1,    80,    -1,
      82,    69,    76,    84,    -1,    45,    83,    76,    70,    -1,
      46,    83,    76,    70,    -1,    47,    83,    76,    70,    -1,
      48,    83,    76,    70,    -1,    49,    83,    76,    70,    -1,
      81,    69,    76,    70,    -1,     3,    89,    -1,    44,    -1,
      69,    -1,    70,    -1,     4,    88,    88,    88,    88,    -1,
       4,    91,    -1,     4,    68,    -1,     5,    88,    88,    88,
      88,    -1,     5,    91,    -1,     5,    68,    -1,     6,    88,
      88,    88,    88,    -1,     6,    91,    -1,     6,    68,    -1,
       7,    88,    -1,    17,    88,    -1,    27,    88,    -1,    37,
      88,    88,    88,    -1,    37,    90,    -1,     8,    88,    -1,
      18,    88,    -1,    28,    88,    -1,    38,    88,    88,    88,
      88,    -1,    38,    88,    90,    -1,    41,    88,    -1,     9,
      88,    88,    88,    92,    -1,     9,    88,    88,    88,    -1,
       9,    88,    88,    -1,    19,    88,    88,    88,    92,    -1,
      19,    88,    88,    88,    -1,    19,    88,    88,    -1,    29,
      88,    88,    88,    92,    -1,    29,    88,    88,    88,    -1,
      29,    88,    88,    -1,    10,    88,    88,    -1,    10,    88,
      -1,    20,    88,    88,    -1,    20,    88,    -1,    30,    88,
      88,    -1,    30,    88,    -1,    11,    88,    88,    88,    92,
      -1,    11,    88,    88,    88,    -1,    11,    88,    88,    -1,
      21,    88,    88,    88,    92,    -1,    21,    88,    88,    88,
      -1,    21,    88,    88,    -1,    31,    88,    88,    88,    92,
      -1,    31,    88,    88,    88,    -1,    31,    88,    88,    -1,
      12,    88,    88,    88,    -1,    12,    88,    88,    -1,    22,
      88,    88,    88,    -1,    22,    88,    88,    -1,    32,    88,
      88,    88,    -1,    32,    88,    88,    -1,    13,    88,    88,
      -1,    13,    88,    -1,    14,    88,    88,    -1,    14,    88,
      -1,    23,    88,    88,    -1,    23,    88,    -1,    24,    88,
      88,    -1,    24,    88,    -1,    33,    88,    88,    -1,    33,
      88,    -1,    34,    88,    88,    -1,    34,    88,    -1,    15,
      88,    -1,    25,    88,    -1,    35,    88,    -1,    39,    88,
      88,    88,    -1,    39,    90,    -1,    39,    88,    -1,    16,
      -1,    26,    -1,    36,    -1,    50,    88,    -1,    42,    89,
      -1,    43,    89,    89,    89,    -1,    40,    -1,    51,    89,
      88,    88,    88,    92,    92,    92,    -1,    51,    89,    88,
      88,    88,    92,    92,    -1,    51,    89,    88,    88,    88,
      92,    -1,    51,    89,    88,    88,    88,    -1,    55,    89,
      88,    -1,    56,    89,    88,    88,    -1,    63,    89,    88,
      88,    -1,    52,    88,    88,    88,    88,    -1,    52,    88,
      88,    88,    -1,    52,    88,    -1,    52,    90,    88,    -1,
      52,    90,    -1,    53,    88,    88,    88,    88,    -1,    53,
      88,    88,    -1,    53,    90,    88,    -1,    54,    88,    88,
      88,    88,    -1,    54,    88,    88,    -1,    54,    90,    88,
      -1,    57,    89,    88,    88,    92,    92,    -1,    57,    89,
      88,    88,    92,    -1,    57,    89,    88,    88,    -1,    58,
      89,    88,    88,    88,    -1,    58,    89,    88,    88,    -1,
      59,    89,    88,    92,    -1,    59,    89,    88,    -1,    60,
      88,    92,    -1,    60,    88,    -1,    61,    88,    92,    -1,
      61,    88,    -1,    62,    89,    88,    88,    88,    88,    88,
      88,    88,    88,    92,    -1,    62,    89,    88,    88,    88,
      88,    88,    88,    88,    88,    -1,    62,    89,    88,    88,
      88,    88,    88,    88,    88,    -1,    65,    -1,    71,    88,
      -1,    66,    -1,    71,    89,    -1,    66,    -1,    71,    89,
      -1,    65,    -1,    71,    88,    -1,    72,    88,    73,    -1,
      72,    88,    88,    88,    73,    -1,    72,    88,    73,    -1,
      72,    88,    88,    88,    88,    73,    -1,    67,    -1,    66,
      -1,    65,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  EasyMeshParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    17,    19,    22,
      24,    26,    28,    30,    35,    40,    45,    50,    55,    60,
      65,    68,    70,    72,    74,    80,    83,    86,    92,    95,
      98,   104,   107,   110,   113,   116,   119,   124,   127,   130,
     133,   136,   142,   146,   149,   155,   160,   164,   170,   175,
     179,   185,   190,   194,   198,   201,   205,   208,   212,   215,
     221,   226,   230,   236,   241,   245,   251,   256,   260,   265,
     269,   274,   278,   283,   287,   291,   294,   298,   301,   305,
     308,   312,   315,   319,   322,   326,   329,   332,   335,   338,
     343,   346,   349,   351,   353,   355,   358,   361,   366,   368,
     377,   385,   392,   398,   402,   407,   412,   418,   423,   426,
     430,   433,   439,   443,   447,   453,   457,   461,   468,   474,
     479,   485,   490,   495,   499,   503,   506,   510,   513,   525,
     536,   546,   548,   551,   553,   556,   558,   561,   563,   566,
     570,   576,   580,   587,   589,   591
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  EasyMeshParser::yyrline_[] =
  {
         0,    93,    93,    97,    98,   102,   103,   107,   108,   112,
     113,   114,   115,   116,   120,   121,   122,   123,   124,   125,
     129,   131,   135,   139,   143,   144,   145,   148,   149,   150,
     153,   154,   155,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   266,   267,   268,   269,   273,   274,   275,   276,   281,
     282,   286,   287,   292,   293,   294
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
      72,    73,     2,     2,     2,    71,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    69,     2,    70,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int EasyMeshParser::yyeof_ = 0;
  const int EasyMeshParser::yylast_ = 556;
  const int EasyMeshParser::yynnts_ = 19;
  const int EasyMeshParser::yyempty_ = -2;
  const int EasyMeshParser::yyfinal_ = 150;
  const int EasyMeshParser::yyterror_ = 1;
  const int EasyMeshParser::yyerrcode_ = 256;
  const int EasyMeshParser::yyntokens_ = 74;

  const unsigned int EasyMeshParser::yyuser_token_number_max_ = 323;
  const EasyMeshParser::token_number_type EasyMeshParser::yyundef_token_ = 2;


} // lol

/* Line 1053 of lalr1.cc  */
#line 2123 "generated/easymesh-parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 297 "easymesh/easymesh-parser.y"


void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}


