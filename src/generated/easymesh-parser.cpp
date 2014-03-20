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
#line 81 "easymesh/easymesh-parser.y"

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
    { mc.m_mesh.LoopEnd(); }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 124 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgUnion();  mc.m_mesh.CloseBrace(); }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 125 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgSub();    mc.m_mesh.CloseBrace(); }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 126 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgSubL();   mc.m_mesh.CloseBrace(); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 127 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgAnd();    mc.m_mesh.CloseBrace(); }
    break;

  case 19:

/* Line 677 of lalr1.cc  */
#line 128 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgXor();    mc.m_mesh.CloseBrace(); }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 132 "easymesh/easymesh-parser.y"
    { mc.m_mesh.LoopStart((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 136 "easymesh/easymesh-parser.y"
    { mc.m_mesh.DupAndScale(vec3(1.f), true); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 140 "easymesh/easymesh-parser.y"
    { mc.m_mesh.OpenBrace(); }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 144 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CloseBrace(); }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 148 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 149 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 150 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor(vec4(v) * (1.f / 255.f)); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 153 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColorA(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 154 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColorA(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 155 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColorA(vec4(v) * (1.f / 255.f)); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 158 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColorB(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 159 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColorB(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 160 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColorB(vec4(v) * (1.f / 255.f)); }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 163 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetVertColor(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 164 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetVertColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 165 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetVertColor(vec4(v) * (1.f / 255.f)); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 171 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].fval), 0.f, 0.f)); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 172 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, (yysemantic_stack_[(2) - (2)].fval), 0.f)); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 173 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, 0.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 174 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 175 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 176 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 177 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 43:

/* Line 677 of lalr1.cc  */
#line 178 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 44:

/* Line 677 of lalr1.cc  */
#line 179 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Rotate((yysemantic_stack_[(5) - (2)].fval), vec3((yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 45:

/* Line 677 of lalr1.cc  */
#line 180 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Rotate((yysemantic_stack_[(3) - (2)].fval), vec3((yysemantic_stack_[(3) - (3)].vval)[0], (yysemantic_stack_[(3) - (3)].vval)[1], (yysemantic_stack_[(3) - (3)].vval)[2])); }
    break;

  case 46:

/* Line 677 of lalr1.cc  */
#line 181 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RadialJitter((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 47:

/* Line 677 of lalr1.cc  */
#line 182 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 48:

/* Line 677 of lalr1.cc  */
#line 183 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 49:

/* Line 677 of lalr1.cc  */
#line 184 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 50:

/* Line 677 of lalr1.cc  */
#line 185 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 51:

/* Line 677 of lalr1.cc  */
#line 186 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 52:

/* Line 677 of lalr1.cc  */
#line 187 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 53:

/* Line 677 of lalr1.cc  */
#line 188 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 54:

/* Line 677 of lalr1.cc  */
#line 189 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 55:

/* Line 677 of lalr1.cc  */
#line 190 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 56:

/* Line 677 of lalr1.cc  */
#line 191 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 57:

/* Line 677 of lalr1.cc  */
#line 192 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 58:

/* Line 677 of lalr1.cc  */
#line 193 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 59:

/* Line 677 of lalr1.cc  */
#line 194 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 60:

/* Line 677 of lalr1.cc  */
#line 195 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 61:

/* Line 677 of lalr1.cc  */
#line 196 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 62:

/* Line 677 of lalr1.cc  */
#line 197 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 63:

/* Line 677 of lalr1.cc  */
#line 198 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 64:

/* Line 677 of lalr1.cc  */
#line 199 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 65:

/* Line 677 of lalr1.cc  */
#line 200 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 66:

/* Line 677 of lalr1.cc  */
#line 201 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 67:

/* Line 677 of lalr1.cc  */
#line 202 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 68:

/* Line 677 of lalr1.cc  */
#line 203 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 69:

/* Line 677 of lalr1.cc  */
#line 204 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 70:

/* Line 677 of lalr1.cc  */
#line 205 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 71:

/* Line 677 of lalr1.cc  */
#line 206 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 72:

/* Line 677 of lalr1.cc  */
#line 207 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 73:

/* Line 677 of lalr1.cc  */
#line 208 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 74:

/* Line 677 of lalr1.cc  */
#line 209 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 75:

/* Line 677 of lalr1.cc  */
#line 210 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 76:

/* Line 677 of lalr1.cc  */
#line 211 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 77:

/* Line 677 of lalr1.cc  */
#line 212 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 78:

/* Line 677 of lalr1.cc  */
#line 213 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 79:

/* Line 677 of lalr1.cc  */
#line 214 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 80:

/* Line 677 of lalr1.cc  */
#line 215 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 81:

/* Line 677 of lalr1.cc  */
#line 216 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 82:

/* Line 677 of lalr1.cc  */
#line 217 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 83:

/* Line 677 of lalr1.cc  */
#line 218 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 84:

/* Line 677 of lalr1.cc  */
#line 219 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 85:

/* Line 677 of lalr1.cc  */
#line 220 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 86:

/* Line 677 of lalr1.cc  */
#line 221 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 87:

/* Line 677 of lalr1.cc  */
#line 222 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 88:

/* Line 677 of lalr1.cc  */
#line 223 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 89:

/* Line 677 of lalr1.cc  */
#line 224 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), 1.f, 1.f)); }
    break;

  case 90:

/* Line 677 of lalr1.cc  */
#line 225 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, (yysemantic_stack_[(2) - (2)].fval), 1.f)); }
    break;

  case 91:

/* Line 677 of lalr1.cc  */
#line 226 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, 1.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 92:

/* Line 677 of lalr1.cc  */
#line 227 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 93:

/* Line 677 of lalr1.cc  */
#line 228 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 94:

/* Line 677 of lalr1.cc  */
#line 229 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 95:

/* Line 677 of lalr1.cc  */
#line 230 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorX(); }
    break;

  case 96:

/* Line 677 of lalr1.cc  */
#line 231 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorY(); }
    break;

  case 97:

/* Line 677 of lalr1.cc  */
#line 232 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorZ(); }
    break;

  case 98:

/* Line 677 of lalr1.cc  */
#line 233 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Chamfer((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 99:

/* Line 677 of lalr1.cc  */
#line 234 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SplitTriangles((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 100:

/* Line 677 of lalr1.cc  */
#line 235 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SmoothMesh((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].ival), (yysemantic_stack_[(4) - (4)].ival)); }
    break;

  case 101:

/* Line 677 of lalr1.cc  */
#line 236 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleScaleWinding(); }
    break;

  case 102:

/* Line 677 of lalr1.cc  */
#line 237 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleQuadWeighting(); }
    break;

  case 103:

/* Line 677 of lalr1.cc  */
#line 238 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TogglePostBuildNormal(); }
    break;

  case 104:

/* Line 677 of lalr1.cc  */
#line 239 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleVerticeNoCleanup(); }
    break;

  case 105:

/* Line 677 of lalr1.cc  */
#line 240 "easymesh/easymesh-parser.y"
    { mc.m_mesh.VerticesMerge(); }
    break;

  case 106:

/* Line 677 of lalr1.cc  */
#line 241 "easymesh/easymesh-parser.y"
    { mc.m_mesh.VerticesSeparate(); }
    break;

  case 107:

/* Line 677 of lalr1.cc  */
#line 245 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(8) - (2)].ival), (yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (4)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (6)].bval), (yysemantic_stack_[(8) - (7)].bval), (yysemantic_stack_[(8) - (8)].bval)); }
    break;

  case 108:

/* Line 677 of lalr1.cc  */
#line 246 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(7) - (2)].ival), (yysemantic_stack_[(7) - (3)].fval), (yysemantic_stack_[(7) - (4)].fval), (yysemantic_stack_[(7) - (5)].fval), (yysemantic_stack_[(7) - (6)].bval), (yysemantic_stack_[(7) - (7)].bval)); }
    break;

  case 109:

/* Line 677 of lalr1.cc  */
#line 247 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].fval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 110:

/* Line 677 of lalr1.cc  */
#line 248 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 111:

/* Line 677 of lalr1.cc  */
#line 249 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 112:

/* Line 677 of lalr1.cc  */
#line 250 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSphere((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 113:

/* Line 677 of lalr1.cc  */
#line 251 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCapsule((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 114:

/* Line 677 of lalr1.cc  */
#line 252 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendTorus((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 115:

/* Line 677 of lalr1.cc  */
#line 253 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 116:

/* Line 677 of lalr1.cc  */
#line 254 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 117:

/* Line 677 of lalr1.cc  */
#line 255 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 118:

/* Line 677 of lalr1.cc  */
#line 256 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 119:

/* Line 677 of lalr1.cc  */
#line 257 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 120:

/* Line 677 of lalr1.cc  */
#line 258 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 121:

/* Line 677 of lalr1.cc  */
#line 259 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 122:

/* Line 677 of lalr1.cc  */
#line 260 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 123:

/* Line 677 of lalr1.cc  */
#line 261 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 124:

/* Line 677 of lalr1.cc  */
#line 262 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 125:

/* Line 677 of lalr1.cc  */
#line 263 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 126:

/* Line 677 of lalr1.cc  */
#line 264 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].bval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 127:

/* Line 677 of lalr1.cc  */
#line 265 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 128:

/* Line 677 of lalr1.cc  */
#line 266 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 129:

/* Line 677 of lalr1.cc  */
#line 267 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 130:

/* Line 677 of lalr1.cc  */
#line 268 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 131:

/* Line 677 of lalr1.cc  */
#line 269 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].bval)); }
    break;

  case 132:

/* Line 677 of lalr1.cc  */
#line 270 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 133:

/* Line 677 of lalr1.cc  */
#line 271 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 134:

/* Line 677 of lalr1.cc  */
#line 272 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 135:

/* Line 677 of lalr1.cc  */
#line 273 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 136:

/* Line 677 of lalr1.cc  */
#line 274 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 137:

/* Line 677 of lalr1.cc  */
#line 275 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(11) - (2)].ival), (yysemantic_stack_[(11) - (3)].fval), (yysemantic_stack_[(11) - (4)].fval), (yysemantic_stack_[(11) - (5)].fval), (yysemantic_stack_[(11) - (6)].fval), (yysemantic_stack_[(11) - (7)].fval), (yysemantic_stack_[(11) - (8)].fval), (yysemantic_stack_[(11) - (9)].fval), (yysemantic_stack_[(11) - (10)].fval), (yysemantic_stack_[(11) - (11)].bval)); }
    break;

  case 138:

/* Line 677 of lalr1.cc  */
#line 276 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(10) - (2)].ival), (yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (4)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (6)].fval), (yysemantic_stack_[(10) - (7)].fval), (yysemantic_stack_[(10) - (8)].fval), (yysemantic_stack_[(10) - (9)].fval), (yysemantic_stack_[(10) - (10)].fval)); }
    break;

  case 139:

/* Line 677 of lalr1.cc  */
#line 277 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(9) - (2)].ival), (yysemantic_stack_[(9) - (3)].fval), (yysemantic_stack_[(9) - (4)].fval), (yysemantic_stack_[(9) - (5)].fval), (yysemantic_stack_[(9) - (6)].fval), (yysemantic_stack_[(9) - (7)].fval), (yysemantic_stack_[(9) - (8)].fval), (yysemantic_stack_[(9) - (9)].fval)); }
    break;

  case 140:

/* Line 677 of lalr1.cc  */
#line 278 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(8) - (2)].ival), (yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (4)].vval)[0], (yysemantic_stack_[(8) - (4)].vval)[1], (yysemantic_stack_[(8) - (5)].vval)[0], (yysemantic_stack_[(8) - (5)].vval)[1], (yysemantic_stack_[(8) - (6)].vval)[0], (yysemantic_stack_[(8) - (6)].vval)[1], (yysemantic_stack_[(8) - (7)].fval), (yysemantic_stack_[(8) - (8)].bval)); }
    break;

  case 141:

/* Line 677 of lalr1.cc  */
#line 279 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(7) - (2)].ival), (yysemantic_stack_[(7) - (3)].fval), (yysemantic_stack_[(7) - (4)].vval)[0], (yysemantic_stack_[(7) - (4)].vval)[1], (yysemantic_stack_[(7) - (5)].vval)[0], (yysemantic_stack_[(7) - (5)].vval)[1], (yysemantic_stack_[(7) - (6)].vval)[0], (yysemantic_stack_[(7) - (6)].vval)[1], (yysemantic_stack_[(7) - (7)].fval)); }
    break;

  case 142:

/* Line 677 of lalr1.cc  */
#line 280 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].vval)[0], (yysemantic_stack_[(6) - (4)].vval)[1], (yysemantic_stack_[(6) - (5)].vval)[0], (yysemantic_stack_[(6) - (5)].vval)[1], (yysemantic_stack_[(6) - (6)].vval)[0], (yysemantic_stack_[(6) - (6)].vval)[1]); }
    break;

  case 143:

/* Line 677 of lalr1.cc  */
#line 285 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 144:

/* Line 677 of lalr1.cc  */
#line 286 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 145:

/* Line 677 of lalr1.cc  */
#line 287 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (float)(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 146:

/* Line 677 of lalr1.cc  */
#line 288 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(float)(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 147:

/* Line 677 of lalr1.cc  */
#line 292 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 148:

/* Line 677 of lalr1.cc  */
#line 293 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 149:

/* Line 677 of lalr1.cc  */
#line 294 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (int)(yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 150:

/* Line 677 of lalr1.cc  */
#line 295 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(int)(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 151:

/* Line 677 of lalr1.cc  */
#line 300 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 152:

/* Line 677 of lalr1.cc  */
#line 301 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(4) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(4) - (3)].fval); }
    break;

  case 153:

/* Line 677 of lalr1.cc  */
#line 305 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 154:

/* Line 677 of lalr1.cc  */
#line 306 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(5) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(5) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(5) - (4)].fval); }
    break;

  case 155:

/* Line 677 of lalr1.cc  */
#line 310 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[3] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 156:

/* Line 677 of lalr1.cc  */
#line 311 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(6) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(6) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(6) - (4)].fval); (yyval.vval)[3] = (yysemantic_stack_[(6) - (5)].fval); }
    break;

  case 157:

/* Line 677 of lalr1.cc  */
#line 316 "easymesh/easymesh-parser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 158:

/* Line 677 of lalr1.cc  */
#line 317 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 159:

/* Line 677 of lalr1.cc  */
#line 318 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].fval); }
    break;



/* Line 677 of lalr1.cc  */
#line 1474 "generated/easymesh-parser.cpp"
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
  const short int EasyMeshParser::yypact_ninf_ = -266;
  const short int
  EasyMeshParser::yypact_[] =
  {
       412,   -14,    17,    89,   108,   146,  -266,  -266,  -266,  -266,
    -266,  -266,    87,    87,    87,    87,    87,    87,    87,    87,
      87,  -266,    87,    87,    87,    87,    87,    87,    87,    87,
      87,  -266,    87,    87,    87,    87,    87,    87,    87,    87,
      87,  -266,   164,    87,   164,    87,   -14,   -14,  -266,   -57,
     -57,   -57,   -57,   -57,    87,   -14,   164,   164,   164,   -14,
     -14,   -14,   -14,   -14,    87,    87,   -14,   -14,  -266,    38,
      43,   412,  -266,   485,  -266,   -26,   -19,   412,  -266,  -266,
    -266,  -266,  -266,   129,  -266,  -266,  -266,  -266,   129,    87,
      87,  -266,  -266,    87,  -266,  -266,    87,  -266,  -266,    87,
    -266,  -266,  -266,    87,    87,    87,    87,    87,    87,  -266,
    -266,  -266,    87,    87,    87,    87,    87,    87,  -266,  -266,
    -266,    87,    87,    87,    87,    87,    87,  -266,    87,    87,
    -266,   164,    87,  -266,  -266,  -266,   -14,   412,   412,   412,
     412,   412,  -266,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,   -25,   -25,    87,    87,  -266,
    -266,  -266,  -266,   412,   412,   -17,  -266,  -266,   129,  -266,
    -266,  -266,  -266,   -27,    87,    87,    87,    87,    87,  -266,
      87,    87,  -266,  -266,    87,  -266,    87,    87,  -266,  -266,
      87,  -266,    87,    87,  -266,  -266,    -5,    87,    87,  -266,
      87,   -14,   -16,    -8,     6,     8,    16,    87,    87,  -266,
      87,  -266,    87,  -266,  -266,    87,    87,    87,   -25,  -266,
    -266,  -266,  -266,  -266,   166,    87,    30,   -17,  -266,  -266,
    -266,  -266,  -266,    87,    87,    87,    87,    87,   -25,   -25,
    -266,   -25,   -25,  -266,   -25,   -25,  -266,  -266,    87,  -266,
      87,  -266,  -266,  -266,  -266,  -266,  -266,  -266,    87,    87,
      87,    87,  -266,   -25,    87,  -266,    87,    87,    -9,  -266,
    -266,  -266,    87,  -266,  -266,  -266,  -266,  -266,  -266,  -266,
    -266,  -266,  -266,    -6,  -266,   -25,  -266,  -266,  -266,   -25,
    -266,     4,    87,    -9,    28,  -266,   -25,  -266,  -266,    29,
      87,    87,  -266,   -25,  -266,    87,   -25,  -266,    87,  -266,
      87,   -25,  -266
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  EasyMeshParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,   101,   102,   103,   104,
     105,   106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     3,     5,     7,    12,     0,     0,     0,     9,    10,
      11,   149,   147,     0,    20,   143,   145,    26,     0,     0,
       0,    25,    29,     0,    28,    32,     0,    31,    35,     0,
      34,    36,    41,     0,    57,     0,     0,    78,    80,    89,
      37,    42,     0,    59,     0,     0,    82,    84,    90,    38,
      43,     0,    61,     0,     0,    86,    88,    91,     0,     0,
      40,     0,    94,    93,    46,    99,     0,     0,     0,     0,
       0,     0,    98,     0,   117,   119,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   134,   136,     0,     0,     1,
       2,     4,     8,     0,     0,     0,   143,   145,     0,   150,
     148,   144,   146,     0,     0,     0,     0,     0,    49,    56,
      64,    72,    77,    79,    52,    58,    67,    74,    81,    83,
      55,    60,    70,    76,    85,    87,     0,     0,     0,    45,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   118,
     121,   122,   124,   125,   112,     0,     0,     0,   132,   159,
     158,   157,   133,   135,     0,     0,     0,     0,    23,     6,
     144,   146,   155,     0,     0,     0,     0,     0,    48,    63,
      71,    51,    66,    73,    54,    69,    75,   153,     0,    39,
       0,    92,   100,    15,    16,    17,    18,    19,   111,   116,
       0,     0,   113,   128,   130,   131,     0,     0,     0,   114,
      14,    13,     0,    24,    27,    30,    33,    47,    62,    50,
      65,    53,    68,     0,    44,   110,   115,   120,   123,   127,
     129,     0,     0,     0,     0,   154,   109,   126,   151,     0,
       0,   142,   156,   108,   152,     0,   141,   107,     0,   140,
     139,   138,   137
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yypgoto_[] =
  {
      -266,  -266,    -7,  -266,    -2,  -266,  -266,  -266,  -266,   -45,
    -112,  -266,  -266,  -266,    -3,   213,  -265,    21,    94,    72
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yydefgoto_[] =
  {
        -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
     229,    78,    79,    80,    90,    84,   268,   130,    91,   222
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char EasyMeshParser::yytable_ninf_ = -1;
  const unsigned short int
  EasyMeshParser::yytable_[] =
  {
        93,    96,    99,   293,   137,   138,   139,   140,   141,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    68,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   301,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   159,   129,
     131,   132,   134,   160,    85,    86,   219,   220,   221,   163,
      88,   142,   232,   144,   146,   148,   164,    81,    82,   228,
     253,   155,   156,    83,   161,   133,    85,    86,   254,   266,
     165,   162,    88,   295,   247,    85,    86,   145,   147,   149,
     169,    88,   255,   298,   256,   171,   173,   174,    85,    86,
     175,    87,   257,   176,    88,    89,   177,    94,    97,   100,
     178,   179,   180,   181,   182,   183,   270,   302,   304,   184,
     185,   186,   187,   188,   189,   271,     0,     0,   190,   191,
     192,   193,   194,   195,     0,   196,   197,     0,   198,   200,
     202,   203,   204,   205,   206,     0,     0,     0,     0,     0,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   199,     0,   224,   225,   226,   227,    85,    86,
      85,    86,     0,    92,    88,   230,    88,    89,     0,     0,
     233,   234,   235,   236,   237,   238,     0,   239,   240,    85,
      86,   241,    95,   242,   243,    88,    89,   244,     0,   245,
     246,     0,     0,   248,   249,   250,     0,   251,     0,     0,
     166,   167,     0,     0,   258,   259,   168,   260,     0,   261,
       0,     0,   262,   263,   264,     0,     0,    85,    86,     0,
      98,   267,   269,    88,    89,     0,     0,     0,   223,     0,
     272,   273,   274,   275,   276,    85,    86,    85,    86,     0,
       0,    88,   128,    88,   266,   283,     0,   284,     0,     0,
       0,     0,     0,     0,     0,   285,   286,   287,   288,   135,
     136,   290,     0,   291,   292,     0,     0,     0,   143,   294,
       0,     0,   150,   151,   152,   153,   154,     0,     0,   157,
     158,     0,     0,     0,     0,     0,     0,     0,   299,   300,
     265,     0,     0,     0,     0,     0,   170,   305,   306,     0,
       0,   172,   308,     0,     0,   310,     0,   311,     0,     0,
     277,   278,     0,   279,   280,     0,   281,   282,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   201,
       0,     0,     0,     0,     0,     0,     0,   296,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,   303,     0,
       0,     0,     0,     0,     0,   307,     0,     0,   309,     0,
       0,   231,     0,   312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   252,     1,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,     0,     0,     0,     0,     0,    68,     1,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67
  };

  /* YYCHECK.  */
  const short int
  EasyMeshParser::yycheck_[] =
  {
         3,     4,     5,   268,    49,    50,    51,    52,    53,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    75,    22,
      23,    24,    25,    26,    27,    28,    29,    30,   293,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    42,
      43,    44,    45,     0,    71,    72,    71,    72,    73,    75,
      77,    54,    79,    56,    57,    58,    75,    71,    72,    76,
      76,    64,    65,    77,    71,    44,    71,    72,    76,    78,
      77,    73,    77,    79,    79,    71,    72,    56,    57,    58,
      83,    77,    76,    79,    76,    88,    89,    90,    71,    72,
      93,    74,    76,    96,    77,    78,    99,     3,     4,     5,
     103,   104,   105,   106,   107,   108,    76,    79,    79,   112,
     113,   114,   115,   116,   117,   227,    -1,    -1,   121,   122,
     123,   124,   125,   126,    -1,   128,   129,    -1,   131,   132,
     137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   131,    -1,   157,   158,   163,   164,    71,    72,
      71,    72,    -1,    74,    77,   168,    77,    78,    -1,    -1,
     173,   174,   175,   176,   177,   178,    -1,   180,   181,    71,
      72,   184,    74,   186,   187,    77,    78,   190,    -1,   192,
     193,    -1,    -1,   196,   197,   198,    -1,   200,    -1,    -1,
      71,    72,    -1,    -1,   207,   208,    77,   210,    -1,   212,
      -1,    -1,   215,   216,   217,    -1,    -1,    71,    72,    -1,
      74,   224,   225,    77,    78,    -1,    -1,    -1,   156,    -1,
     233,   234,   235,   236,   237,    71,    72,    71,    72,    -1,
      -1,    77,    78,    77,    78,   248,    -1,   250,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   258,   259,   260,   261,    46,
      47,   264,    -1,   266,   267,    -1,    -1,    -1,    55,   272,
      -1,    -1,    59,    60,    61,    62,    63,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,
     218,    -1,    -1,    -1,    -1,    -1,    83,   300,   301,    -1,
      -1,    88,   305,    -1,    -1,   308,    -1,   310,    -1,    -1,
     238,   239,    -1,   241,   242,    -1,   244,   245,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   263,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,
      -1,   289,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,   306,    -1,
      -1,   168,    -1,   311,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    75,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69
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
      62,    63,    64,    65,    66,    67,    68,    69,    75,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    91,    92,
      93,    71,    72,    77,    95,    71,    72,    74,    77,    78,
      94,    98,    74,    94,    98,    74,    94,    98,    74,    94,
      98,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    78,    94,
      97,    94,    94,    97,    94,    95,    95,    89,    89,    89,
      89,    89,    94,    95,    94,    97,    94,    97,    94,    97,
      95,    95,    95,    95,    95,    94,    94,    95,    95,     0,
       0,    82,    84,    75,    75,    82,    71,    72,    77,    94,
      95,    94,    95,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    97,
      94,    95,    82,    82,    82,    82,    82,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    71,
      72,    73,    99,    99,    94,    94,    82,    82,    76,    90,
      94,    95,    79,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    79,    94,    94,
      94,    94,    95,    76,    76,    76,    76,    76,    94,    94,
      94,    94,    94,    94,    94,    99,    78,    94,    96,    94,
      76,    90,    94,    94,    94,    94,    94,    99,    99,    99,
      99,    99,    99,    94,    94,    94,    94,    94,    94,    99,
      94,    94,    94,    96,    94,    79,    99,    99,    79,    94,
      94,    96,    79,    99,    79,    94,    94,    99,    94,    99,
      94,    94,    99
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,    91,    93,    45,    40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  EasyMeshParser::yyr1_[] =
  {
         0,    80,    81,    82,    82,    83,    83,    84,    84,    85,
      85,    85,    85,    85,    85,    86,    86,    86,    86,    86,
      87,    88,    89,    90,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    94,    94,    94,    94,    95,    95,    95,
      95,    96,    96,    97,    97,    98,    98,    99,    99,    99
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  EasyMeshParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     3,     1,     2,     1,
       1,     1,     1,     4,     4,     4,     4,     4,     4,     4,
       2,     1,     1,     1,     5,     2,     2,     5,     2,     2,
       5,     2,     2,     5,     2,     2,     2,     2,     2,     4,
       2,     2,     2,     2,     5,     3,     2,     5,     4,     3,
       5,     4,     3,     5,     4,     3,     3,     2,     3,     2,
       3,     2,     5,     4,     3,     5,     4,     3,     5,     4,
       3,     4,     3,     4,     3,     4,     3,     3,     2,     3,
       2,     3,     2,     3,     2,     3,     2,     3,     2,     2,
       2,     2,     4,     2,     2,     1,     1,     1,     2,     2,
       4,     1,     1,     1,     1,     1,     1,     8,     7,     6,
       5,     4,     3,     4,     4,     5,     4,     2,     3,     2,
       5,     3,     3,     5,     3,     3,     6,     5,     4,     5,
       4,     4,     3,     3,     2,     3,     2,    11,    10,     9,
       8,     7,     6,     1,     2,     1,     2,     1,     2,     1,
       2,     3,     4,     3,     5,     3,     6,     1,     1,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const EasyMeshParser::yytname_[] =
  {
    "T_END", "error", "$undefined", "T_LOOP", "T_COLOR", "T_ACOLOR",
  "T_BCOLOR", "T_VCOLOR", "T_TOGGLESCALEWINDING", "T_TOGGLEQUADWEIGHTING",
  "T_TOGGLEPOSTBUILDNORMAL", "T_TOGGLEVERTNOCLEANUP", "T_VERTMERGE",
  "T_VERTSEPARATE", "T_TRANSLATEX", "T_ROTATEX", "T_TAPERX", "T_TWISTX",
  "T_SHEARX", "T_STRETCHX", "T_BENDXY", "T_BENDXZ", "T_SCALEX",
  "T_MIRRORX", "T_TRANSLATEY", "T_ROTATEY", "T_TAPERY", "T_TWISTY",
  "T_SHEARY", "T_STRETCHY", "T_BENDYX", "T_BENDYZ", "T_SCALEY",
  "T_MIRRORY", "T_TRANSLATEZ", "T_ROTATEZ", "T_TAPERZ", "T_TWISTZ",
  "T_SHEARZ", "T_STRETCHZ", "T_BENDZX", "T_BENDZY", "T_SCALEZ",
  "T_MIRRORZ", "T_TRANSLATE", "T_ROTATE", "T_SCALE", "T_RADIALJITTER",
  "T_SPLITTRIANGLE", "T_SMOOTHMESH", "T_DUPLICATE", "T_CSGUNION",
  "T_CSGSUBSTRACT", "T_CSGSUBSTRACTLOSS", "T_CSGAND", "T_CSGXOR",
  "T_CHAMFER", "T_CYLINDER", "T_BOX", "T_SMOOTHCHAMFBOX", "T_FLATCHAMFBOX",
  "T_SPHERE", "T_CAPSULE", "T_STAR", "T_EXPANDEDSTAR", "T_DISC",
  "T_TRIANGLE", "T_QUAD", "T_COG", "T_TORUS", "T_ERROR", "F_NUMBER",
  "I_NUMBER", "BOOLEAN", "COLOR", "'['", "']'", "'-'", "'('", "')'",
  "$accept", "mesh_description", "mesh_expression_list", "mesh_expression",
  "mesh_command_list", "mesh_command", "post_brace_command", "doloop",
  "pre_brace_command", "mesh_open", "mesh_close", "color_command",
  "transform_command", "primitive_command", "fv", "iv", "v2", "v3", "v4",
  "bv", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const EasyMeshParser::rhs_number_type
  EasyMeshParser::yyrhs_[] =
  {
        81,     0,    -1,    82,     0,    -1,    83,    -1,    83,    82,
      -1,    84,    -1,    89,    82,    90,    -1,    85,    -1,    85,
      84,    -1,    91,    -1,    92,    -1,    93,    -1,    86,    -1,
      88,    75,    82,    90,    -1,    87,    75,    82,    76,    -1,
      51,    89,    82,    76,    -1,    52,    89,    82,    76,    -1,
      53,    89,    82,    76,    -1,    54,    89,    82,    76,    -1,
      55,    89,    82,    76,    -1,     3,    95,    -1,    50,    -1,
      75,    -1,    76,    -1,     4,    94,    94,    94,    94,    -1,
       4,    98,    -1,     4,    74,    -1,     5,    94,    94,    94,
      94,    -1,     5,    98,    -1,     5,    74,    -1,     6,    94,
      94,    94,    94,    -1,     6,    98,    -1,     6,    74,    -1,
       7,    94,    94,    94,    94,    -1,     7,    98,    -1,     7,
      74,    -1,    14,    94,    -1,    24,    94,    -1,    34,    94,
      -1,    44,    94,    94,    94,    -1,    44,    97,    -1,    15,
      94,    -1,    25,    94,    -1,    35,    94,    -1,    45,    94,
      94,    94,    94,    -1,    45,    94,    97,    -1,    47,    94,
      -1,    16,    94,    94,    94,    99,    -1,    16,    94,    94,
      94,    -1,    16,    94,    94,    -1,    26,    94,    94,    94,
      99,    -1,    26,    94,    94,    94,    -1,    26,    94,    94,
      -1,    36,    94,    94,    94,    99,    -1,    36,    94,    94,
      94,    -1,    36,    94,    94,    -1,    17,    94,    94,    -1,
      17,    94,    -1,    27,    94,    94,    -1,    27,    94,    -1,
      37,    94,    94,    -1,    37,    94,    -1,    18,    94,    94,
      94,    99,    -1,    18,    94,    94,    94,    -1,    18,    94,
      94,    -1,    28,    94,    94,    94,    99,    -1,    28,    94,
      94,    94,    -1,    28,    94,    94,    -1,    38,    94,    94,
      94,    99,    -1,    38,    94,    94,    94,    -1,    38,    94,
      94,    -1,    19,    94,    94,    94,    -1,    19,    94,    94,
      -1,    29,    94,    94,    94,    -1,    29,    94,    94,    -1,
      39,    94,    94,    94,    -1,    39,    94,    94,    -1,    20,
      94,    94,    -1,    20,    94,    -1,    21,    94,    94,    -1,
      21,    94,    -1,    30,    94,    94,    -1,    30,    94,    -1,
      31,    94,    94,    -1,    31,    94,    -1,    40,    94,    94,
      -1,    40,    94,    -1,    41,    94,    94,    -1,    41,    94,
      -1,    22,    94,    -1,    32,    94,    -1,    42,    94,    -1,
      46,    94,    94,    94,    -1,    46,    97,    -1,    46,    94,
      -1,    23,    -1,    33,    -1,    43,    -1,    56,    94,    -1,
      48,    95,    -1,    49,    95,    95,    95,    -1,     8,    -1,
       9,    -1,    10,    -1,    11,    -1,    12,    -1,    13,    -1,
      57,    95,    94,    94,    94,    99,    99,    99,    -1,    57,
      95,    94,    94,    94,    99,    99,    -1,    57,    95,    94,
      94,    94,    99,    -1,    57,    95,    94,    94,    94,    -1,
      57,    95,    94,    94,    -1,    61,    95,    94,    -1,    62,
      95,    94,    94,    -1,    69,    95,    94,    94,    -1,    58,
      94,    94,    94,    94,    -1,    58,    94,    94,    94,    -1,
      58,    94,    -1,    58,    97,    94,    -1,    58,    97,    -1,
      59,    94,    94,    94,    94,    -1,    59,    94,    94,    -1,
      59,    97,    94,    -1,    60,    94,    94,    94,    94,    -1,
      60,    94,    94,    -1,    60,    97,    94,    -1,    63,    95,
      94,    94,    99,    99,    -1,    63,    95,    94,    94,    99,
      -1,    63,    95,    94,    94,    -1,    64,    95,    94,    94,
      94,    -1,    64,    95,    94,    94,    -1,    65,    95,    94,
      99,    -1,    65,    95,    94,    -1,    66,    94,    99,    -1,
      66,    94,    -1,    67,    94,    99,    -1,    67,    94,    -1,
      68,    95,    94,    94,    94,    94,    94,    94,    94,    94,
      99,    -1,    68,    95,    94,    94,    94,    94,    94,    94,
      94,    94,    -1,    68,    95,    94,    94,    94,    94,    94,
      94,    94,    -1,    68,    95,    94,    96,    96,    96,    94,
      99,    -1,    68,    95,    94,    96,    96,    96,    94,    -1,
      68,    95,    94,    96,    96,    96,    -1,    71,    -1,    77,
      94,    -1,    72,    -1,    77,    95,    -1,    72,    -1,    77,
      95,    -1,    71,    -1,    77,    94,    -1,    78,    94,    79,
      -1,    78,    94,    94,    79,    -1,    78,    94,    79,    -1,
      78,    94,    94,    94,    79,    -1,    78,    94,    79,    -1,
      78,    94,    94,    94,    94,    79,    -1,    73,    -1,    72,
      -1,    71,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  EasyMeshParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    17,    19,    22,
      24,    26,    28,    30,    35,    40,    45,    50,    55,    60,
      65,    68,    70,    72,    74,    80,    83,    86,    92,    95,
      98,   104,   107,   110,   116,   119,   122,   125,   128,   131,
     136,   139,   142,   145,   148,   154,   158,   161,   167,   172,
     176,   182,   187,   191,   197,   202,   206,   210,   213,   217,
     220,   224,   227,   233,   238,   242,   248,   253,   257,   263,
     268,   272,   277,   281,   286,   290,   295,   299,   303,   306,
     310,   313,   317,   320,   324,   327,   331,   334,   338,   341,
     344,   347,   350,   355,   358,   361,   363,   365,   367,   370,
     373,   378,   380,   382,   384,   386,   388,   390,   399,   407,
     414,   420,   425,   429,   434,   439,   445,   450,   453,   457,
     460,   466,   470,   474,   480,   484,   488,   495,   501,   506,
     512,   517,   522,   526,   530,   533,   537,   540,   552,   563,
     573,   582,   590,   597,   599,   602,   604,   607,   609,   612,
     614,   617,   621,   626,   630,   636,   640,   647,   649,   651
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  EasyMeshParser::yyrline_[] =
  {
         0,    96,    96,   100,   101,   105,   106,   110,   111,   115,
     116,   117,   118,   119,   120,   124,   125,   126,   127,   128,
     132,   136,   140,   144,   148,   149,   150,   153,   154,   155,
     158,   159,   160,   163,   164,   165,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   285,   286,   287,   288,   292,   293,   294,
     295,   300,   301,   305,   306,   310,   311,   316,   317,   318
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
      78,    79,     2,     2,     2,    77,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    75,     2,    76,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int EasyMeshParser::yyeof_ = 0;
  const int EasyMeshParser::yylast_ = 554;
  const int EasyMeshParser::yynnts_ = 20;
  const int EasyMeshParser::yyempty_ = -2;
  const int EasyMeshParser::yyfinal_ = 159;
  const int EasyMeshParser::yyterror_ = 1;
  const int EasyMeshParser::yyerrcode_ = 256;
  const int EasyMeshParser::yyntokens_ = 80;

  const unsigned int EasyMeshParser::yyuser_token_number_max_ = 329;
  const EasyMeshParser::token_number_type EasyMeshParser::yyundef_token_ = 2;


} // lol

/* Line 1053 of lalr1.cc  */
#line 2241 "generated/easymesh-parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 321 "easymesh/easymesh-parser.y"


void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}


