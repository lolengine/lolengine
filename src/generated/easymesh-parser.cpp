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
#line 150 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetVertColor(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 151 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetVertColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 152 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetVertColor(vec4(v) * (1.f / 255.f)); }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 158 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].fval), 0.f, 0.f)); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 159 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, (yysemantic_stack_[(2) - (2)].fval), 0.f)); }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 160 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, 0.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 161 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 162 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 163 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 164 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 165 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 166 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Rotate((yysemantic_stack_[(5) - (2)].fval), vec3((yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 167 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Rotate((yysemantic_stack_[(3) - (2)].fval), vec3((yysemantic_stack_[(3) - (3)].vval)[0], (yysemantic_stack_[(3) - (3)].vval)[1], (yysemantic_stack_[(3) - (3)].vval)[2])); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 168 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RadialJitter((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 169 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 43:

/* Line 677 of lalr1.cc  */
#line 170 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 44:

/* Line 677 of lalr1.cc  */
#line 171 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 45:

/* Line 677 of lalr1.cc  */
#line 172 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 46:

/* Line 677 of lalr1.cc  */
#line 173 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 47:

/* Line 677 of lalr1.cc  */
#line 174 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 48:

/* Line 677 of lalr1.cc  */
#line 175 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 49:

/* Line 677 of lalr1.cc  */
#line 176 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 50:

/* Line 677 of lalr1.cc  */
#line 177 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 51:

/* Line 677 of lalr1.cc  */
#line 178 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 52:

/* Line 677 of lalr1.cc  */
#line 179 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 53:

/* Line 677 of lalr1.cc  */
#line 180 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 54:

/* Line 677 of lalr1.cc  */
#line 181 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 55:

/* Line 677 of lalr1.cc  */
#line 182 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 56:

/* Line 677 of lalr1.cc  */
#line 183 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 57:

/* Line 677 of lalr1.cc  */
#line 184 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 58:

/* Line 677 of lalr1.cc  */
#line 185 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 59:

/* Line 677 of lalr1.cc  */
#line 186 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 60:

/* Line 677 of lalr1.cc  */
#line 187 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 61:

/* Line 677 of lalr1.cc  */
#line 188 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 62:

/* Line 677 of lalr1.cc  */
#line 189 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 63:

/* Line 677 of lalr1.cc  */
#line 190 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 64:

/* Line 677 of lalr1.cc  */
#line 191 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 65:

/* Line 677 of lalr1.cc  */
#line 192 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 66:

/* Line 677 of lalr1.cc  */
#line 193 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 67:

/* Line 677 of lalr1.cc  */
#line 194 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 68:

/* Line 677 of lalr1.cc  */
#line 195 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 69:

/* Line 677 of lalr1.cc  */
#line 196 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 70:

/* Line 677 of lalr1.cc  */
#line 197 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 71:

/* Line 677 of lalr1.cc  */
#line 198 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 72:

/* Line 677 of lalr1.cc  */
#line 199 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 73:

/* Line 677 of lalr1.cc  */
#line 200 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 74:

/* Line 677 of lalr1.cc  */
#line 201 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 75:

/* Line 677 of lalr1.cc  */
#line 202 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 76:

/* Line 677 of lalr1.cc  */
#line 203 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 77:

/* Line 677 of lalr1.cc  */
#line 204 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 78:

/* Line 677 of lalr1.cc  */
#line 205 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 79:

/* Line 677 of lalr1.cc  */
#line 206 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 80:

/* Line 677 of lalr1.cc  */
#line 207 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 81:

/* Line 677 of lalr1.cc  */
#line 208 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 82:

/* Line 677 of lalr1.cc  */
#line 209 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 83:

/* Line 677 of lalr1.cc  */
#line 210 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 84:

/* Line 677 of lalr1.cc  */
#line 211 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), 1.f, 1.f)); }
    break;

  case 85:

/* Line 677 of lalr1.cc  */
#line 212 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, (yysemantic_stack_[(2) - (2)].fval), 1.f)); }
    break;

  case 86:

/* Line 677 of lalr1.cc  */
#line 213 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, 1.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 87:

/* Line 677 of lalr1.cc  */
#line 214 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 88:

/* Line 677 of lalr1.cc  */
#line 215 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 89:

/* Line 677 of lalr1.cc  */
#line 216 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 90:

/* Line 677 of lalr1.cc  */
#line 217 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorX(); }
    break;

  case 91:

/* Line 677 of lalr1.cc  */
#line 218 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorY(); }
    break;

  case 92:

/* Line 677 of lalr1.cc  */
#line 219 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorZ(); }
    break;

  case 93:

/* Line 677 of lalr1.cc  */
#line 220 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Chamfer((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 94:

/* Line 677 of lalr1.cc  */
#line 221 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SplitTriangles((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 95:

/* Line 677 of lalr1.cc  */
#line 222 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SmoothMesh((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].ival), (yysemantic_stack_[(4) - (4)].ival)); }
    break;

  case 96:

/* Line 677 of lalr1.cc  */
#line 223 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleScaleWinding(); }
    break;

  case 97:

/* Line 677 of lalr1.cc  */
#line 227 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(8) - (2)].ival), (yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (4)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (6)].bval), (yysemantic_stack_[(8) - (7)].bval), (yysemantic_stack_[(8) - (8)].bval)); }
    break;

  case 98:

/* Line 677 of lalr1.cc  */
#line 228 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(7) - (2)].ival), (yysemantic_stack_[(7) - (3)].fval), (yysemantic_stack_[(7) - (4)].fval), (yysemantic_stack_[(7) - (5)].fval), (yysemantic_stack_[(7) - (6)].bval), (yysemantic_stack_[(7) - (7)].bval)); }
    break;

  case 99:

/* Line 677 of lalr1.cc  */
#line 229 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].fval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 100:

/* Line 677 of lalr1.cc  */
#line 230 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 101:

/* Line 677 of lalr1.cc  */
#line 231 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSphere((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 102:

/* Line 677 of lalr1.cc  */
#line 232 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCapsule((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 103:

/* Line 677 of lalr1.cc  */
#line 233 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendTorus((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 104:

/* Line 677 of lalr1.cc  */
#line 234 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 105:

/* Line 677 of lalr1.cc  */
#line 235 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 106:

/* Line 677 of lalr1.cc  */
#line 236 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 107:

/* Line 677 of lalr1.cc  */
#line 237 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 108:

/* Line 677 of lalr1.cc  */
#line 238 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 109:

/* Line 677 of lalr1.cc  */
#line 239 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 110:

/* Line 677 of lalr1.cc  */
#line 240 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 111:

/* Line 677 of lalr1.cc  */
#line 241 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 112:

/* Line 677 of lalr1.cc  */
#line 242 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 113:

/* Line 677 of lalr1.cc  */
#line 243 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 114:

/* Line 677 of lalr1.cc  */
#line 244 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 115:

/* Line 677 of lalr1.cc  */
#line 245 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].bval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 116:

/* Line 677 of lalr1.cc  */
#line 246 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 117:

/* Line 677 of lalr1.cc  */
#line 247 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 118:

/* Line 677 of lalr1.cc  */
#line 248 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 119:

/* Line 677 of lalr1.cc  */
#line 249 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 120:

/* Line 677 of lalr1.cc  */
#line 250 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].bval)); }
    break;

  case 121:

/* Line 677 of lalr1.cc  */
#line 251 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 122:

/* Line 677 of lalr1.cc  */
#line 252 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 123:

/* Line 677 of lalr1.cc  */
#line 253 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 124:

/* Line 677 of lalr1.cc  */
#line 254 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 125:

/* Line 677 of lalr1.cc  */
#line 255 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 126:

/* Line 677 of lalr1.cc  */
#line 256 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(11) - (2)].ival), (yysemantic_stack_[(11) - (3)].fval), (yysemantic_stack_[(11) - (4)].fval), (yysemantic_stack_[(11) - (5)].fval), (yysemantic_stack_[(11) - (6)].fval), (yysemantic_stack_[(11) - (7)].fval), (yysemantic_stack_[(11) - (8)].fval), (yysemantic_stack_[(11) - (9)].fval), (yysemantic_stack_[(11) - (10)].fval), (yysemantic_stack_[(11) - (11)].bval)); }
    break;

  case 127:

/* Line 677 of lalr1.cc  */
#line 257 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(10) - (2)].ival), (yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (4)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (6)].fval), (yysemantic_stack_[(10) - (7)].fval), (yysemantic_stack_[(10) - (8)].fval), (yysemantic_stack_[(10) - (9)].fval), (yysemantic_stack_[(10) - (10)].fval)); }
    break;

  case 128:

/* Line 677 of lalr1.cc  */
#line 258 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(9) - (2)].ival), (yysemantic_stack_[(9) - (3)].fval), (yysemantic_stack_[(9) - (4)].fval), (yysemantic_stack_[(9) - (5)].fval), (yysemantic_stack_[(9) - (6)].fval), (yysemantic_stack_[(9) - (7)].fval), (yysemantic_stack_[(9) - (8)].fval), (yysemantic_stack_[(9) - (9)].fval)); }
    break;

  case 129:

/* Line 677 of lalr1.cc  */
#line 263 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 130:

/* Line 677 of lalr1.cc  */
#line 264 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 131:

/* Line 677 of lalr1.cc  */
#line 265 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (float)(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 132:

/* Line 677 of lalr1.cc  */
#line 266 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(float)(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 133:

/* Line 677 of lalr1.cc  */
#line 270 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 134:

/* Line 677 of lalr1.cc  */
#line 271 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 135:

/* Line 677 of lalr1.cc  */
#line 272 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (int)(yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 136:

/* Line 677 of lalr1.cc  */
#line 273 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(int)(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 137:

/* Line 677 of lalr1.cc  */
#line 278 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 138:

/* Line 677 of lalr1.cc  */
#line 279 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(5) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(5) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(5) - (4)].fval); }
    break;

  case 139:

/* Line 677 of lalr1.cc  */
#line 283 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[3] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 140:

/* Line 677 of lalr1.cc  */
#line 284 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(6) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(6) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(6) - (4)].fval); (yyval.vval)[3] = (yysemantic_stack_[(6) - (5)].fval); }
    break;

  case 141:

/* Line 677 of lalr1.cc  */
#line 289 "easymesh/easymesh-parser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 142:

/* Line 677 of lalr1.cc  */
#line 290 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 143:

/* Line 677 of lalr1.cc  */
#line 291 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].fval); }
    break;



/* Line 677 of lalr1.cc  */
#line 1360 "generated/easymesh-parser.cpp"
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
  const signed char EasyMeshParser::yypact_ninf_ = -112;
  const short int
  EasyMeshParser::yypact_[] =
  {
       390,    -1,    13,    81,     3,     3,     3,     3,     3,     3,
       3,     3,     3,  -112,     3,     3,     3,     3,     3,     3,
       3,     3,     3,  -112,     3,     3,     3,     3,     3,     3,
       3,     3,     3,  -112,   100,     3,   100,  -112,     3,    85,
      85,  -112,   -57,   -57,   -57,   -57,   -57,     3,    85,   100,
     100,   100,    85,    85,    85,    85,    85,     3,     3,    85,
      85,  -112,    21,    31,   390,  -112,   456,  -112,   -33,   390,
    -112,  -112,  -112,  -112,  -112,  -112,   119,     3,     3,  -112,
    -112,     3,  -112,  -112,     3,  -112,  -112,  -112,     3,     3,
       3,     3,     3,     3,  -112,  -112,  -112,     3,     3,     3,
       3,     3,     3,  -112,  -112,  -112,     3,     3,     3,     3,
       3,     3,  -112,     3,     3,  -112,   100,     3,  -112,  -112,
    -112,  -112,   119,  -112,    85,   390,   390,   390,   390,   390,
    -112,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,   -24,   -24,     3,     3,  -112,  -112,  -112,
    -112,   390,    -7,  -112,  -112,   119,  -112,  -112,   -26,     3,
       3,     3,     3,  -112,     3,     3,  -112,  -112,     3,  -112,
       3,     3,  -112,  -112,     3,  -112,     3,     3,  -112,  -112,
     -11,     3,     3,  -112,     3,  -112,  -112,    85,     2,    12,
      16,    23,    24,     3,     3,  -112,     3,  -112,     3,  -112,
    -112,     3,     3,     3,   -24,  -112,  -112,  -112,  -112,  -112,
       3,     3,    -7,  -112,  -112,  -112,  -112,  -112,     3,     3,
       3,     3,   -24,   -24,  -112,   -24,   -24,  -112,   -24,   -24,
    -112,  -112,     3,  -112,     3,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,     3,     3,     3,     3,  -112,   -24,     3,  -112,
       3,  -112,  -112,     3,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,   -29,  -112,   -24,  -112,  -112,  -112,   -24,
    -112,     3,     0,  -112,   -24,  -112,     3,  -112,   -24,     3,
    -112,     3,     3,   -24,  -112
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  EasyMeshParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,    96,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    20,     0,     0,     3,     5,     7,    12,     0,     0,
       9,    10,    11,   129,   131,    24,     0,     0,     0,    23,
      27,     0,    26,    30,     0,    29,    31,    36,     0,    52,
       0,     0,    73,    75,    84,    32,    37,     0,    54,     0,
       0,    77,    79,    85,    33,    38,     0,    56,     0,     0,
      81,    83,    86,     0,     0,    35,     0,    89,    88,    41,
     135,   133,     0,    94,     0,     0,     0,     0,     0,     0,
      93,     0,   106,   108,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   123,   125,     0,     0,     1,     2,     4,
       8,     0,     0,   129,   131,     0,   130,   132,     0,     0,
       0,     0,    44,    51,    59,    67,    72,    74,    47,    53,
      62,    69,    76,    78,    50,    55,    65,    71,    80,    82,
       0,     0,     0,    40,     0,   136,   134,     0,     0,     0,
       0,     0,     0,     0,     0,   107,   110,   111,   113,   114,
     101,     0,     0,     0,   121,   143,   142,   141,   122,   124,
       0,     0,     0,    21,     6,   130,   132,   139,     0,     0,
       0,     0,    43,    58,    66,    46,    61,    68,    49,    64,
      70,   137,     0,    34,     0,    87,    95,    14,    15,    16,
      17,    18,     0,   105,     0,     0,   102,   117,   119,   120,
       0,   103,    13,     0,    22,    25,    28,    42,    57,    45,
      60,    48,    63,     0,    39,   100,   104,   109,   112,   116,
     118,     0,     0,   138,    99,   115,     0,   140,    98,     0,
      97,     0,   128,   127,   126
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yypgoto_[] =
  {
      -112,  -112,    -4,  -112,    28,  -112,  -112,  -112,   160,  -111,
    -112,  -112,  -112,    -2,   205,     1,    55,    63
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yydefgoto_[] =
  {
        -1,    62,    63,    64,    65,    66,    67,    68,    69,   214,
      70,    71,    72,    78,   123,   115,    79,   208
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char EasyMeshParser::yytable_ninf_ = -1;
  const unsigned short int
  EasyMeshParser::yytable_[] =
  {
        81,    84,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    61,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   147,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   148,   114,   116,   117,   151,   119,   118,    73,    74,
     205,   206,   207,   273,    76,   130,   217,   132,   134,   136,
     133,   135,   137,    73,    74,   143,   144,    82,    85,    76,
     149,   231,   213,    73,    74,   152,    75,    73,    74,    76,
      77,   237,   277,    76,   156,   158,   159,    73,    74,   160,
      80,   238,   161,    76,    77,   239,   162,   163,   164,   165,
     166,   167,   240,   241,   150,   168,   169,   170,   171,   172,
     173,   252,     0,     0,   174,   175,   176,   177,   178,   179,
       0,   180,   181,     0,   182,   184,     0,   183,     0,     0,
     185,   188,   189,   190,   191,   192,     0,     0,     0,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,     0,     0,   210,   211,    73,    74,   212,    83,   120,
     121,    76,    77,   215,     0,   122,   218,   219,   220,   221,
     222,     0,   223,   224,    73,    74,   225,     0,   226,   227,
      76,   113,   228,     0,   229,   230,     0,     0,   232,   233,
     234,     0,   235,   153,   154,     0,     0,     0,     0,   155,
       0,   242,   243,     0,   244,     0,   245,     0,     0,   246,
     247,   248,   125,   126,   127,   128,   129,   209,   250,   251,
       0,     0,     0,     0,     0,     0,   253,   254,   255,   256,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     263,     0,   264,     0,     0,     0,     0,     0,     0,     0,
     265,   266,   267,   268,     0,   124,   270,     0,   271,     0,
       0,   272,     0,   131,     0,     0,     0,   138,   139,   140,
     141,   142,     0,     0,   145,   146,     0,   249,     0,   276,
       0,     0,     0,     0,   279,     0,     0,   281,     0,   282,
     283,   157,     0,     0,     0,   257,   258,     0,   259,   260,
       0,   261,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   186,   274,   187,
       0,     0,   275,     0,     0,     0,     0,   278,     0,     0,
       0,   280,     0,     0,     0,     0,   284,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     216,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,     1,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     0,    61,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60
  };

  /* YYCHECK.  */
  const short int
  EasyMeshParser::yycheck_[] =
  {
         2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    68,    14,    15,    16,    17,    18,    19,    20,    21,
      22,     0,    24,    25,    26,    27,    28,    29,    30,    31,
      32,     0,    34,    35,    36,    68,    38,    36,    64,    65,
      64,    65,    66,    72,    70,    47,    72,    49,    50,    51,
      49,    50,    51,    64,    65,    57,    58,     2,     3,    70,
      64,    72,    69,    64,    65,    69,    67,    64,    65,    70,
      71,    69,    72,    70,    76,    77,    78,    64,    65,    81,
      67,    69,    84,    70,    71,    69,    88,    89,    90,    91,
      92,    93,    69,    69,    66,    97,    98,    99,   100,   101,
     102,   212,    -1,    -1,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,    -1,   116,   117,    -1,   116,    -1,    -1,
     122,   125,   126,   127,   128,   129,    -1,    -1,    -1,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,    -1,    -1,   145,   146,    64,    65,   151,    67,    64,
      65,    70,    71,   155,    -1,    70,   158,   159,   160,   161,
     162,    -1,   164,   165,    64,    65,   168,    -1,   170,   171,
      70,    71,   174,    -1,   176,   177,    -1,    -1,   180,   181,
     182,    -1,   184,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,   193,   194,    -1,   196,    -1,   198,    -1,    -1,   201,
     202,   203,    42,    43,    44,    45,    46,   144,   210,   211,
      -1,    -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     232,    -1,   234,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     242,   243,   244,   245,    -1,    40,   248,    -1,   250,    -1,
      -1,   253,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    -1,    -1,    59,    60,    -1,   204,    -1,   271,
      -1,    -1,    -1,    -1,   276,    -1,    -1,   279,    -1,   281,
     282,    76,    -1,    -1,    -1,   222,   223,    -1,   225,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   265,   124,
      -1,    -1,   269,    -1,    -1,    -1,    -1,   274,    -1,    -1,
      -1,   278,    -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    -1,    -1,    68,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62
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
      62,    68,    74,    75,    76,    77,    78,    79,    80,    81,
      83,    84,    85,    64,    65,    67,    70,    71,    86,    89,
      67,    86,    89,    67,    86,    89,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    71,    86,    88,    86,    86,    88,    86,
      64,    65,    70,    87,    87,    81,    81,    81,    81,    81,
      86,    87,    86,    88,    86,    88,    86,    88,    87,    87,
      87,    87,    87,    86,    86,    87,    87,     0,     0,    75,
      77,    68,    75,    64,    65,    70,    86,    87,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    88,    86,    86,    87,    87,    75,    75,
      75,    75,    75,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    64,    65,    66,    90,    90,
      86,    86,    75,    69,    82,    86,    87,    72,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    72,    86,    86,    86,    86,    87,    69,    69,    69,
      69,    69,    86,    86,    86,    86,    86,    86,    86,    90,
      86,    86,    82,    86,    86,    86,    86,    90,    90,    90,
      90,    90,    90,    86,    86,    86,    86,    86,    86,    90,
      86,    86,    86,    72,    90,    90,    86,    72,    90,    86,
      90,    86,    86,    86,    90
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
     315,   316,   317,   318,   319,   320,   321,   322,    91,    93,
      45,    40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  EasyMeshParser::yyr1_[] =
  {
         0,    73,    74,    75,    75,    76,    76,    77,    77,    78,
      78,    78,    78,    78,    79,    79,    79,    79,    79,    80,
      81,    82,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    86,
      86,    86,    86,    87,    87,    87,    87,    88,    88,    89,
      89,    90,    90,    90
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  EasyMeshParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     3,     1,     2,     1,
       1,     1,     1,     4,     4,     4,     4,     4,     4,     1,
       1,     1,     5,     2,     2,     5,     2,     2,     5,     2,
       2,     2,     2,     2,     4,     2,     2,     2,     2,     5,
       3,     2,     5,     4,     3,     5,     4,     3,     5,     4,
       3,     3,     2,     3,     2,     3,     2,     5,     4,     3,
       5,     4,     3,     5,     4,     3,     4,     3,     4,     3,
       4,     3,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     2,     2,     2,     4,     2,     2,
       1,     1,     1,     2,     2,     4,     1,     8,     7,     6,
       5,     3,     4,     4,     5,     4,     2,     3,     2,     5,
       3,     3,     5,     3,     3,     6,     5,     4,     5,     4,
       4,     3,     3,     2,     3,     2,    11,    10,     9,     1,
       2,     1,     2,     1,     2,     1,     2,     3,     5,     3,
       6,     1,     1,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const EasyMeshParser::yytname_[] =
  {
    "T_END", "error", "$undefined", "T_COLOR", "T_BCOLOR", "T_VCOLOR",
  "T_TRANSLATEX", "T_ROTATEX", "T_TAPERX", "T_TWISTX", "T_SHEARX",
  "T_STRETCHX", "T_BENDXY", "T_BENDXZ", "T_SCALEX", "T_MIRRORX",
  "T_TRANSLATEY", "T_ROTATEY", "T_TAPERY", "T_TWISTY", "T_SHEARY",
  "T_STRETCHY", "T_BENDYX", "T_BENDYZ", "T_SCALEY", "T_MIRRORY",
  "T_TRANSLATEZ", "T_ROTATEZ", "T_TAPERZ", "T_TWISTZ", "T_SHEARZ",
  "T_STRETCHZ", "T_BENDZX", "T_BENDZY", "T_SCALEZ", "T_MIRRORZ",
  "T_TRANSLATE", "T_ROTATE", "T_SCALE", "T_TOGGLESCALEWINDING",
  "T_RADIALJITTER", "T_SPLITTRIANGLE", "T_SMOOTHMESH", "T_DUPLICATE",
  "T_CSGUNION", "T_CSGSUBSTRACT", "T_CSGSUBSTRACTLOSS", "T_CSGAND",
  "T_CSGXOR", "T_CHAMFER", "T_CYLINDER", "T_BOX", "T_SMOOTHCHAMFBOX",
  "T_FLATCHAMFBOX", "T_SPHERE", "T_CAPSULE", "T_STAR", "T_EXPANDEDSTAR",
  "T_DISC", "T_TRIANGLE", "T_QUAD", "T_COG", "T_TORUS", "T_ERROR",
  "F_NUMBER", "I_NUMBER", "BOOLEAN", "COLOR", "'['", "']'", "'-'", "'('",
  "')'", "$accept", "mesh_description", "mesh_expression_list",
  "mesh_expression", "mesh_command_list", "mesh_command",
  "post_brace_command", "pre_brace_command", "mesh_open", "mesh_close",
  "color_command", "transform_command", "primitive_command", "fv", "iv",
  "v3", "v4", "bv", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const EasyMeshParser::rhs_number_type
  EasyMeshParser::yyrhs_[] =
  {
        74,     0,    -1,    75,     0,    -1,    76,    -1,    76,    75,
      -1,    77,    -1,    81,    75,    82,    -1,    78,    -1,    78,
      77,    -1,    83,    -1,    84,    -1,    85,    -1,    79,    -1,
      80,    68,    75,    82,    -1,    44,    81,    75,    69,    -1,
      45,    81,    75,    69,    -1,    46,    81,    75,    69,    -1,
      47,    81,    75,    69,    -1,    48,    81,    75,    69,    -1,
      43,    -1,    68,    -1,    69,    -1,     3,    86,    86,    86,
      86,    -1,     3,    89,    -1,     3,    67,    -1,     4,    86,
      86,    86,    86,    -1,     4,    89,    -1,     4,    67,    -1,
       5,    86,    86,    86,    86,    -1,     5,    89,    -1,     5,
      67,    -1,     6,    86,    -1,    16,    86,    -1,    26,    86,
      -1,    36,    86,    86,    86,    -1,    36,    88,    -1,     7,
      86,    -1,    17,    86,    -1,    27,    86,    -1,    37,    86,
      86,    86,    86,    -1,    37,    86,    88,    -1,    40,    86,
      -1,     8,    86,    86,    86,    90,    -1,     8,    86,    86,
      86,    -1,     8,    86,    86,    -1,    18,    86,    86,    86,
      90,    -1,    18,    86,    86,    86,    -1,    18,    86,    86,
      -1,    28,    86,    86,    86,    90,    -1,    28,    86,    86,
      86,    -1,    28,    86,    86,    -1,     9,    86,    86,    -1,
       9,    86,    -1,    19,    86,    86,    -1,    19,    86,    -1,
      29,    86,    86,    -1,    29,    86,    -1,    10,    86,    86,
      86,    90,    -1,    10,    86,    86,    86,    -1,    10,    86,
      86,    -1,    20,    86,    86,    86,    90,    -1,    20,    86,
      86,    86,    -1,    20,    86,    86,    -1,    30,    86,    86,
      86,    90,    -1,    30,    86,    86,    86,    -1,    30,    86,
      86,    -1,    11,    86,    86,    86,    -1,    11,    86,    86,
      -1,    21,    86,    86,    86,    -1,    21,    86,    86,    -1,
      31,    86,    86,    86,    -1,    31,    86,    86,    -1,    12,
      86,    86,    -1,    12,    86,    -1,    13,    86,    86,    -1,
      13,    86,    -1,    22,    86,    86,    -1,    22,    86,    -1,
      23,    86,    86,    -1,    23,    86,    -1,    32,    86,    86,
      -1,    32,    86,    -1,    33,    86,    86,    -1,    33,    86,
      -1,    14,    86,    -1,    24,    86,    -1,    34,    86,    -1,
      38,    86,    86,    86,    -1,    38,    88,    -1,    38,    86,
      -1,    15,    -1,    25,    -1,    35,    -1,    49,    86,    -1,
      41,    87,    -1,    42,    87,    87,    87,    -1,    39,    -1,
      50,    87,    86,    86,    86,    90,    90,    90,    -1,    50,
      87,    86,    86,    86,    90,    90,    -1,    50,    87,    86,
      86,    86,    90,    -1,    50,    87,    86,    86,    86,    -1,
      54,    87,    86,    -1,    55,    87,    86,    86,    -1,    62,
      87,    86,    86,    -1,    51,    86,    86,    86,    86,    -1,
      51,    86,    86,    86,    -1,    51,    86,    -1,    51,    88,
      86,    -1,    51,    88,    -1,    52,    86,    86,    86,    86,
      -1,    52,    86,    86,    -1,    52,    88,    86,    -1,    53,
      86,    86,    86,    86,    -1,    53,    86,    86,    -1,    53,
      88,    86,    -1,    56,    87,    86,    86,    90,    90,    -1,
      56,    87,    86,    86,    90,    -1,    56,    87,    86,    86,
      -1,    57,    87,    86,    86,    86,    -1,    57,    87,    86,
      86,    -1,    58,    87,    86,    90,    -1,    58,    87,    86,
      -1,    59,    86,    90,    -1,    59,    86,    -1,    60,    86,
      90,    -1,    60,    86,    -1,    61,    87,    86,    86,    86,
      86,    86,    86,    86,    86,    90,    -1,    61,    87,    86,
      86,    86,    86,    86,    86,    86,    86,    -1,    61,    87,
      86,    86,    86,    86,    86,    86,    86,    -1,    64,    -1,
      70,    86,    -1,    65,    -1,    70,    87,    -1,    65,    -1,
      70,    87,    -1,    64,    -1,    70,    86,    -1,    71,    86,
      72,    -1,    71,    86,    86,    86,    72,    -1,    71,    86,
      72,    -1,    71,    86,    86,    86,    86,    72,    -1,    66,
      -1,    65,    -1,    64,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  EasyMeshParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    17,    19,    22,
      24,    26,    28,    30,    35,    40,    45,    50,    55,    60,
      62,    64,    66,    72,    75,    78,    84,    87,    90,    96,
      99,   102,   105,   108,   111,   116,   119,   122,   125,   128,
     134,   138,   141,   147,   152,   156,   162,   167,   171,   177,
     182,   186,   190,   193,   197,   200,   204,   207,   213,   218,
     222,   228,   233,   237,   243,   248,   252,   257,   261,   266,
     270,   275,   279,   283,   286,   290,   293,   297,   300,   304,
     307,   311,   314,   318,   321,   324,   327,   330,   335,   338,
     341,   343,   345,   347,   350,   353,   358,   360,   369,   377,
     384,   390,   394,   399,   404,   410,   415,   418,   422,   425,
     431,   435,   439,   445,   449,   453,   460,   466,   471,   477,
     482,   487,   491,   495,   498,   502,   505,   517,   528,   538,
     540,   543,   545,   548,   550,   553,   555,   558,   562,   568,
     572,   579,   581,   583
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  EasyMeshParser::yyrline_[] =
  {
         0,    93,    93,    97,    98,   102,   103,   107,   108,   112,
     113,   114,   115,   116,   120,   121,   122,   123,   124,   128,
     132,   136,   140,   141,   142,   145,   146,   147,   150,   151,
     152,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   263,
     264,   265,   266,   270,   271,   272,   273,   278,   279,   283,
     284,   289,   290,   291
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
      71,    72,     2,     2,     2,    70,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    68,     2,    69,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int EasyMeshParser::yyeof_ = 0;
  const int EasyMeshParser::yylast_ = 518;
  const int EasyMeshParser::yynnts_ = 18;
  const int EasyMeshParser::yyempty_ = -2;
  const int EasyMeshParser::yyfinal_ = 147;
  const int EasyMeshParser::yyterror_ = 1;
  const int EasyMeshParser::yyerrcode_ = 256;
  const int EasyMeshParser::yyntokens_ = 73;

  const unsigned int EasyMeshParser::yyuser_token_number_max_ = 322;
  const EasyMeshParser::token_number_type EasyMeshParser::yyundef_token_ = 2;


} // lol

/* Line 1053 of lalr1.cc  */
#line 2097 "generated/easymesh-parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 294 "easymesh/easymesh-parser.y"


void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}


