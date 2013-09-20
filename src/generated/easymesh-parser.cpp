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
#line 76 "easymesh/easymesh-parser.y"

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
	  case 7:

/* Line 677 of lalr1.cc  */
#line 105 "easymesh/easymesh-parser.y"
    { mc.m_mesh.OpenBrace(); }
    break;

  case 8:

/* Line 677 of lalr1.cc  */
#line 109 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CloseBrace(); }
    break;

  case 14:

/* Line 677 of lalr1.cc  */
#line 124 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 125 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 126 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor(vec4(v) * (1.f / 255.f)); }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 129 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval))); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 130 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 19:

/* Line 677 of lalr1.cc  */
#line 131 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                              ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                              mc.m_mesh.SetCurColor2(vec4(v) * (1.f / 255.f)); }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 137 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Chamfer((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 138 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].fval), 0.f, 0.f)); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 139 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, (yysemantic_stack_[(2) - (2)].fval), 0.f)); }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 140 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0.f, 0.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 141 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 142 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 143 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 144 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 145 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 146 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 147 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 148 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 149 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 150 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 151 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 152 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 153 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 154 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 155 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 156 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 157 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 158 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 159 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 43:

/* Line 677 of lalr1.cc  */
#line 160 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 44:

/* Line 677 of lalr1.cc  */
#line 161 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 45:

/* Line 677 of lalr1.cc  */
#line 162 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 46:

/* Line 677 of lalr1.cc  */
#line 163 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 47:

/* Line 677 of lalr1.cc  */
#line 164 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 48:

/* Line 677 of lalr1.cc  */
#line 165 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 49:

/* Line 677 of lalr1.cc  */
#line 166 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 50:

/* Line 677 of lalr1.cc  */
#line 167 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 51:

/* Line 677 of lalr1.cc  */
#line 168 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 52:

/* Line 677 of lalr1.cc  */
#line 169 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 53:

/* Line 677 of lalr1.cc  */
#line 170 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 54:

/* Line 677 of lalr1.cc  */
#line 171 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 55:

/* Line 677 of lalr1.cc  */
#line 172 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 56:

/* Line 677 of lalr1.cc  */
#line 173 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 57:

/* Line 677 of lalr1.cc  */
#line 174 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 58:

/* Line 677 of lalr1.cc  */
#line 175 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 59:

/* Line 677 of lalr1.cc  */
#line 176 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 60:

/* Line 677 of lalr1.cc  */
#line 177 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 61:

/* Line 677 of lalr1.cc  */
#line 178 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 62:

/* Line 677 of lalr1.cc  */
#line 179 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 63:

/* Line 677 of lalr1.cc  */
#line 180 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 64:

/* Line 677 of lalr1.cc  */
#line 181 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 65:

/* Line 677 of lalr1.cc  */
#line 182 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 66:

/* Line 677 of lalr1.cc  */
#line 183 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 67:

/* Line 677 of lalr1.cc  */
#line 184 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 68:

/* Line 677 of lalr1.cc  */
#line 185 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 69:

/* Line 677 of lalr1.cc  */
#line 186 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 70:

/* Line 677 of lalr1.cc  */
#line 187 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 71:

/* Line 677 of lalr1.cc  */
#line 188 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), 1.f, 1.f)); }
    break;

  case 72:

/* Line 677 of lalr1.cc  */
#line 189 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, (yysemantic_stack_[(2) - (2)].fval), 1.f)); }
    break;

  case 73:

/* Line 677 of lalr1.cc  */
#line 190 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.f, 1.f, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 74:

/* Line 677 of lalr1.cc  */
#line 191 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 75:

/* Line 677 of lalr1.cc  */
#line 192 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 76:

/* Line 677 of lalr1.cc  */
#line 193 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 77:

/* Line 677 of lalr1.cc  */
#line 194 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorX(); }
    break;

  case 78:

/* Line 677 of lalr1.cc  */
#line 195 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorY(); }
    break;

  case 79:

/* Line 677 of lalr1.cc  */
#line 196 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorZ(); }
    break;

  case 80:

/* Line 677 of lalr1.cc  */
#line 197 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RadialJitter((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 81:

/* Line 677 of lalr1.cc  */
#line 198 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SplitTriangles((yysemantic_stack_[(2) - (2)].ival)); }
    break;

  case 82:

/* Line 677 of lalr1.cc  */
#line 199 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SmoothMesh((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].ival), (yysemantic_stack_[(4) - (4)].ival)); }
    break;

  case 83:

/* Line 677 of lalr1.cc  */
#line 200 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleScaleWinding(); }
    break;

  case 84:

/* Line 677 of lalr1.cc  */
#line 201 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgUnion(); }
    break;

  case 85:

/* Line 677 of lalr1.cc  */
#line 202 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgSubstract(); }
    break;

  case 86:

/* Line 677 of lalr1.cc  */
#line 203 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgSubstractLoss(); }
    break;

  case 87:

/* Line 677 of lalr1.cc  */
#line 204 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgAnd(); }
    break;

  case 88:

/* Line 677 of lalr1.cc  */
#line 205 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgXor(); }
    break;

  case 89:

/* Line 677 of lalr1.cc  */
#line 209 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(8) - (2)].ival), (yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (4)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (6)].bval), (yysemantic_stack_[(8) - (7)].bval), (yysemantic_stack_[(8) - (8)].bval)); }
    break;

  case 90:

/* Line 677 of lalr1.cc  */
#line 210 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(7) - (2)].ival), (yysemantic_stack_[(7) - (3)].fval), (yysemantic_stack_[(7) - (4)].fval), (yysemantic_stack_[(7) - (5)].fval), (yysemantic_stack_[(7) - (6)].bval), (yysemantic_stack_[(7) - (7)].bval)); }
    break;

  case 91:

/* Line 677 of lalr1.cc  */
#line 211 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].fval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 92:

/* Line 677 of lalr1.cc  */
#line 212 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 93:

/* Line 677 of lalr1.cc  */
#line 213 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 94:

/* Line 677 of lalr1.cc  */
#line 214 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval))); }
    break;

  case 95:

/* Line 677 of lalr1.cc  */
#line 215 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval), (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 96:

/* Line 677 of lalr1.cc  */
#line 216 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 97:

/* Line 677 of lalr1.cc  */
#line 217 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 98:

/* Line 677 of lalr1.cc  */
#line 218 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 99:

/* Line 677 of lalr1.cc  */
#line 219 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 100:

/* Line 677 of lalr1.cc  */
#line 220 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 101:

/* Line 677 of lalr1.cc  */
#line 221 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval)), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 102:

/* Line 677 of lalr1.cc  */
#line 222 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (2)].fval)), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 103:

/* Line 677 of lalr1.cc  */
#line 223 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(3) - (2)].vval)[0], (yysemantic_stack_[(3) - (2)].vval)[1], (yysemantic_stack_[(3) - (2)].vval)[2]), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 104:

/* Line 677 of lalr1.cc  */
#line 224 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSphere((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 105:

/* Line 677 of lalr1.cc  */
#line 225 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCapsule((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 106:

/* Line 677 of lalr1.cc  */
#line 226 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendTorus((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 107:

/* Line 677 of lalr1.cc  */
#line 227 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(6) - (2)].ival), (yysemantic_stack_[(6) - (3)].fval), (yysemantic_stack_[(6) - (4)].fval), (yysemantic_stack_[(6) - (5)].bval), (yysemantic_stack_[(6) - (6)].bval)); }
    break;

  case 108:

/* Line 677 of lalr1.cc  */
#line 228 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].bval)); }
    break;

  case 109:

/* Line 677 of lalr1.cc  */
#line 229 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 110:

/* Line 677 of lalr1.cc  */
#line 230 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(5) - (2)].ival), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 111:

/* Line 677 of lalr1.cc  */
#line 231 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 112:

/* Line 677 of lalr1.cc  */
#line 232 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(4) - (2)].ival), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].bval)); }
    break;

  case 113:

/* Line 677 of lalr1.cc  */
#line 233 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((yysemantic_stack_[(3) - (2)].ival), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 114:

/* Line 677 of lalr1.cc  */
#line 234 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 115:

/* Line 677 of lalr1.cc  */
#line 235 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 116:

/* Line 677 of lalr1.cc  */
#line 236 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].bval)); }
    break;

  case 117:

/* Line 677 of lalr1.cc  */
#line 237 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 118:

/* Line 677 of lalr1.cc  */
#line 238 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(11) - (2)].ival), (yysemantic_stack_[(11) - (3)].fval), (yysemantic_stack_[(11) - (4)].fval), (yysemantic_stack_[(11) - (5)].fval), (yysemantic_stack_[(11) - (6)].fval), (yysemantic_stack_[(11) - (7)].fval), (yysemantic_stack_[(11) - (8)].fval), (yysemantic_stack_[(11) - (9)].fval), (yysemantic_stack_[(11) - (10)].fval), (yysemantic_stack_[(11) - (11)].bval)); }
    break;

  case 119:

/* Line 677 of lalr1.cc  */
#line 239 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(10) - (2)].ival), (yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (4)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (6)].fval), (yysemantic_stack_[(10) - (7)].fval), (yysemantic_stack_[(10) - (8)].fval), (yysemantic_stack_[(10) - (9)].fval), (yysemantic_stack_[(10) - (10)].fval)); }
    break;

  case 120:

/* Line 677 of lalr1.cc  */
#line 240 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((yysemantic_stack_[(9) - (2)].ival), (yysemantic_stack_[(9) - (3)].fval), (yysemantic_stack_[(9) - (4)].fval), (yysemantic_stack_[(9) - (5)].fval), (yysemantic_stack_[(9) - (6)].fval), (yysemantic_stack_[(9) - (7)].fval), (yysemantic_stack_[(9) - (8)].fval), (yysemantic_stack_[(9) - (9)].fval)); }
    break;

  case 121:

/* Line 677 of lalr1.cc  */
#line 245 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 122:

/* Line 677 of lalr1.cc  */
#line 246 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 123:

/* Line 677 of lalr1.cc  */
#line 247 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (float)(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 124:

/* Line 677 of lalr1.cc  */
#line 248 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(float)(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 125:

/* Line 677 of lalr1.cc  */
#line 252 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 126:

/* Line 677 of lalr1.cc  */
#line 253 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 127:

/* Line 677 of lalr1.cc  */
#line 254 "easymesh/easymesh-parser.y"
    { (yyval.ival) = (int)(yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 128:

/* Line 677 of lalr1.cc  */
#line 255 "easymesh/easymesh-parser.y"
    { (yyval.ival) = -(int)(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 129:

/* Line 677 of lalr1.cc  */
#line 260 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 130:

/* Line 677 of lalr1.cc  */
#line 261 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(5) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(5) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(5) - (4)].fval); }
    break;

  case 131:

/* Line 677 of lalr1.cc  */
#line 265 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[3] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 132:

/* Line 677 of lalr1.cc  */
#line 266 "easymesh/easymesh-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(6) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(6) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(6) - (4)].fval); (yyval.vval)[3] = (yysemantic_stack_[(6) - (5)].fval); }
    break;

  case 133:

/* Line 677 of lalr1.cc  */
#line 271 "easymesh/easymesh-parser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 134:

/* Line 677 of lalr1.cc  */
#line 272 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 135:

/* Line 677 of lalr1.cc  */
#line 273 "easymesh/easymesh-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].fval); }
    break;



/* Line 677 of lalr1.cc  */
#line 1316 "generated/easymesh-parser.cpp"
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
  const signed char EasyMeshParser::yypact_ninf_ = -37;
  const short int
  EasyMeshParser::yypact_[] =
  {
       317,    46,    72,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -37,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -37,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -37,     5,     5,   -37,   -24,    70,    70,   -37,
     -37,   -37,   -37,   -37,   -24,    70,     5,     5,     5,    70,
      70,    70,    70,    70,   -24,   -24,    70,    70,   -37,    10,
      20,   317,   317,   380,   -37,   -37,   -37,   -37,   -37,   -37,
     -37,    87,   -24,   -24,   -37,   -37,   -24,   -37,   -37,   -37,
     -24,   -24,   -24,   -24,   -24,   -24,   -37,   -37,   -37,   -24,
     -24,   -24,   -24,   -24,   -24,   -37,   -37,   -37,   -24,   -24,
     -24,   -24,   -24,   -24,   -37,   -24,   -24,   -37,   -24,   -37,
     -37,   -37,   -37,    87,   -37,    70,   -37,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -22,
     -22,   -24,   -24,   -37,   -37,   -37,   -36,   -37,   -37,   -37,
      87,   -37,   -37,   -11,   -24,   -24,   -24,   -37,   -24,   -24,
     -37,   -37,   -24,   -37,   -24,   -24,   -37,   -37,   -24,   -37,
     -24,   -24,   -37,   -37,    -7,   -24,   -24,   -37,   -37,    70,
     -24,   -24,   -37,   -24,   -37,   -24,   -37,   -37,   -24,   -24,
     -24,   -22,   -37,   -37,   -37,   -37,   -37,   -24,   -24,   -37,
     -37,   -37,   -37,   -37,   -24,   -24,   -24,   -22,   -22,   -37,
     -22,   -22,   -37,   -22,   -22,   -37,   -37,   -24,   -37,   -37,
     -37,   -24,   -24,   -24,   -24,   -37,   -22,   -24,   -37,   -24,
     -37,   -24,   -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,
     -33,   -22,   -37,   -37,   -37,   -22,   -37,   -24,   -12,   -37,
     -22,   -37,   -24,   -37,   -22,   -24,   -37,   -24,   -24,   -22,
     -37
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  EasyMeshParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,    83,     0,     0,     0,    84,
      85,    86,    87,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     0,
       0,     3,     0,     5,     9,    11,    12,    13,   121,   123,
      16,     0,     0,     0,    15,    19,     0,    18,    21,    26,
       0,    39,     0,     0,    60,    62,    71,    22,    27,     0,
      41,     0,     0,    64,    66,    72,    23,    28,     0,    43,
       0,     0,    68,    70,    73,     0,     0,    25,    76,    75,
      80,   127,   125,     0,    81,     0,    20,     0,    95,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
     117,     0,     0,     1,     2,     4,     0,    10,   121,   123,
       0,   122,   124,     0,     0,     0,    31,    38,    46,    54,
      59,    61,    34,    40,    49,    56,    63,    65,    37,    42,
      52,    58,    67,    69,     0,     0,     0,   128,   126,     0,
       0,     0,    96,    99,   100,   102,   103,   104,     0,     0,
       0,   113,   135,   134,   133,   114,   116,     0,     0,     8,
       6,   122,   124,   131,     0,     0,     0,    30,    45,    53,
      33,    48,    55,    36,    51,    57,   129,     0,    24,    74,
      82,     0,    94,     0,     0,   105,   109,   111,   112,     0,
     106,     0,    14,    17,    29,    44,    32,    47,    35,    50,
       0,    92,    93,    98,   101,   108,   110,     0,     0,   130,
      91,   107,     0,   132,    90,     0,    89,     0,   120,   119,
     118
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yypgoto_[] =
  {
       -37,   -37,     2,   -37,   -37,   -37,   -37,   -30,   -37,   -37,
     -37,    -2,   150,     1,    57,    51
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yydefgoto_[] =
  {
        -1,    59,    60,    61,    62,   190,    63,    64,    65,    66,
      67,    73,   114,   107,    74,   185
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char EasyMeshParser::yytable_ninf_ = -1;
  const unsigned char
  EasyMeshParser::yytable_[] =
  {
        76,    78,    79,    80,    81,    82,    83,    84,    85,    86,
     133,    87,    88,    89,    90,    91,    92,    93,    94,    95,
     134,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     189,   106,   108,   137,   110,   109,   239,    68,    69,   182,
     183,   184,   116,    71,   118,   120,   122,   119,   121,   123,
      68,    69,   129,   130,    68,    69,    71,   243,   193,    77,
      71,     0,   206,   135,   136,     0,    68,    69,     0,   141,
     143,   144,    71,   105,   145,     0,     0,     0,   146,   147,
     148,   149,   150,   151,     0,     0,     0,   152,   153,   154,
     155,   156,   157,     0,     0,     0,   158,   159,   160,   161,
     162,   163,     0,   164,   165,     0,   166,    68,    69,     0,
      70,   167,     0,    71,    72,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,   187,
     188,   111,   112,    68,    69,     0,    75,   113,   191,    71,
      72,   194,   195,   196,   197,     0,   198,   199,   138,   139,
     200,     0,   201,   202,   140,     0,   203,     0,   204,   205,
       0,     0,   207,   208,   209,     0,     0,     0,   211,   212,
       0,   213,     0,   214,     0,     0,   215,   216,   217,     0,
       0,   186,     0,     0,     0,   219,   220,     0,   115,     0,
       0,     0,   221,   222,   223,   117,     0,     0,     0,   124,
     125,   126,   127,   128,     0,   230,   131,   132,     0,   231,
     232,   233,   234,     0,     0,   236,     0,   237,     0,   238,
       0,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   218,     0,     0,   242,     0,     0,     0,     0,
     245,     0,     0,   247,     0,   248,   249,     0,   224,   225,
       0,   226,   227,     0,   228,   229,     0,     0,     0,     0,
       0,     0,     0,   168,     0,   169,     0,   235,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   240,     0,     0,     0,   241,     0,     0,     0,
     192,   244,     0,     0,     0,   246,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   210,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,     0,     0,     0,
       0,     0,    58,     1,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57
  };

  /* YYCHECK.  */
  const short int
  EasyMeshParser::yycheck_[] =
  {
         2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
       0,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       0,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      66,    33,    34,    63,    36,    34,    69,    61,    62,    61,
      62,    63,    44,    67,    46,    47,    48,    46,    47,    48,
      61,    62,    54,    55,    61,    62,    67,    69,    69,     2,
      67,    -1,    69,    61,    62,    -1,    61,    62,    -1,    71,
      72,    73,    67,    68,    76,    -1,    -1,    -1,    80,    81,
      82,    83,    84,    85,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,   106,    -1,   108,    61,    62,    -1,
      64,   113,    -1,    67,    68,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,   131,
     132,    61,    62,    61,    62,    -1,    64,    67,   140,    67,
      68,   143,   144,   145,   146,    -1,   148,   149,    61,    62,
     152,    -1,   154,   155,    67,    -1,   158,    -1,   160,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,   171,
      -1,   173,    -1,   175,    -1,    -1,   178,   179,   180,    -1,
      -1,   130,    -1,    -1,    -1,   187,   188,    -1,    38,    -1,
      -1,    -1,   194,   195,   196,    45,    -1,    -1,    -1,    49,
      50,    51,    52,    53,    -1,   207,    56,    57,    -1,   211,
     212,   213,   214,    -1,    -1,   217,    -1,   219,    -1,   221,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,   237,    -1,    -1,    -1,    -1,
     242,    -1,    -1,   245,    -1,   247,   248,    -1,   197,   198,
      -1,   200,   201,    -1,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,   115,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   231,    -1,    -1,    -1,   235,    -1,    -1,    -1,
     140,   240,    -1,    -1,    -1,   244,    -1,    -1,    -1,    -1,
     249,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59
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
      52,    53,    54,    55,    56,    57,    58,    59,    65,    71,
      72,    73,    74,    76,    77,    78,    79,    80,    61,    62,
      64,    67,    68,    81,    84,    64,    81,    84,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    68,    81,    83,    81,    83,
      81,    61,    62,    67,    82,    82,    81,    82,    81,    83,
      81,    83,    81,    83,    82,    82,    82,    82,    82,    81,
      81,    82,    82,     0,     0,    72,    72,    77,    61,    62,
      67,    81,    82,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    82,    82,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    61,    62,    63,    85,    85,    81,    81,    66,
      75,    81,    82,    69,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    69,    81,    81,    81,
      82,    81,    81,    81,    81,    81,    81,    81,    85,    81,
      81,    81,    81,    81,    85,    85,    85,    85,    85,    85,
      81,    81,    81,    81,    81,    85,    81,    81,    81,    69,
      85,    85,    81,    69,    85,    81,    85,    81,    81,    81,
      85
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
     315,   316,   317,   318,   319,    91,    93,    45,    40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  EasyMeshParser::yyr1_[] =
  {
         0,    70,    71,    72,    72,    73,    73,    74,    75,    76,
      76,    77,    77,    77,    78,    78,    78,    78,    78,    78,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    81,    81,    81,    81,    82,    82,    82,    82,    83,
      83,    84,    84,    85,    85,    85
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  EasyMeshParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     3,     1,     1,     1,
       2,     1,     1,     1,     5,     2,     2,     5,     2,     2,
       2,     2,     2,     2,     4,     2,     2,     2,     2,     5,
       4,     3,     5,     4,     3,     5,     4,     3,     3,     2,
       3,     2,     3,     2,     5,     4,     3,     5,     4,     3,
       5,     4,     3,     4,     3,     4,     3,     4,     3,     3,
       2,     3,     2,     3,     2,     3,     2,     3,     2,     3,
       2,     2,     2,     2,     4,     2,     2,     1,     1,     1,
       2,     2,     4,     1,     1,     1,     1,     1,     1,     8,
       7,     6,     5,     5,     4,     2,     3,     2,     5,     3,
       3,     5,     3,     3,     3,     4,     4,     6,     5,     4,
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
    "T_END", "error", "$undefined", "T_COLOR", "T_BGCOLOR", "T_TRANSLATEX",
  "T_ROTATEX", "T_TAPERX", "T_TWISTX", "T_SHEARX", "T_STRETCHX",
  "T_BENDXY", "T_BENDXZ", "T_SCALEX", "T_MIRRORX", "T_TRANSLATEY",
  "T_ROTATEY", "T_TAPERY", "T_TWISTY", "T_SHEARY", "T_STRETCHY",
  "T_BENDYX", "T_BENDYZ", "T_SCALEY", "T_MIRRORY", "T_TRANSLATEZ",
  "T_ROTATEZ", "T_TAPERZ", "T_TWISTZ", "T_SHEARZ", "T_STRETCHZ",
  "T_BENDZX", "T_BENDZY", "T_SCALEZ", "T_MIRRORZ", "T_TRANSLATE",
  "T_SCALE", "T_TOGGLESCALEWINDING", "T_RADIALJITTER", "T_SPLITTRIANGLE",
  "T_SMOOTHMESH", "T_CSGUNION", "T_CSGSUBSTRACT", "T_CSGSUBSTRACTLOSS",
  "T_CSGAND", "T_CSGXOR", "T_CHAMFER", "T_CYLINDER", "T_BOX",
  "T_SMOOTHCHAMFBOX", "T_FLATCHAMFBOX", "T_SPHERE", "T_CAPSULE", "T_STAR",
  "T_EXPANDEDSTAR", "T_DISC", "T_TRIANGLE", "T_QUAD", "T_COG", "T_TORUS",
  "T_ERROR", "F_NUMBER", "I_NUMBER", "BOOLEAN", "COLOR", "'['", "']'",
  "'-'", "'('", "')'", "$accept", "mesh_description",
  "mesh_expression_list", "mesh_expression", "mesh_open", "mesh_close",
  "mesh_command_list", "mesh_command", "color_command",
  "transform_command", "primitive_command", "fv", "iv", "v3", "v4", "bv", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const EasyMeshParser::rhs_number_type
  EasyMeshParser::yyrhs_[] =
  {
        71,     0,    -1,    72,     0,    -1,    73,    -1,    73,    72,
      -1,    76,    -1,    74,    72,    75,    -1,    65,    -1,    66,
      -1,    77,    -1,    76,    77,    -1,    78,    -1,    79,    -1,
      80,    -1,     3,    81,    81,    81,    81,    -1,     3,    84,
      -1,     3,    64,    -1,     4,    81,    81,    81,    81,    -1,
       4,    84,    -1,     4,    64,    -1,    46,    81,    -1,     5,
      81,    -1,    15,    81,    -1,    25,    81,    -1,    35,    81,
      81,    81,    -1,    35,    83,    -1,     6,    81,    -1,    16,
      81,    -1,    26,    81,    -1,     7,    81,    81,    81,    85,
      -1,     7,    81,    81,    81,    -1,     7,    81,    81,    -1,
      17,    81,    81,    81,    85,    -1,    17,    81,    81,    81,
      -1,    17,    81,    81,    -1,    27,    81,    81,    81,    85,
      -1,    27,    81,    81,    81,    -1,    27,    81,    81,    -1,
       8,    81,    81,    -1,     8,    81,    -1,    18,    81,    81,
      -1,    18,    81,    -1,    28,    81,    81,    -1,    28,    81,
      -1,     9,    81,    81,    81,    85,    -1,     9,    81,    81,
      81,    -1,     9,    81,    81,    -1,    19,    81,    81,    81,
      85,    -1,    19,    81,    81,    81,    -1,    19,    81,    81,
      -1,    29,    81,    81,    81,    85,    -1,    29,    81,    81,
      81,    -1,    29,    81,    81,    -1,    10,    81,    81,    81,
      -1,    10,    81,    81,    -1,    20,    81,    81,    81,    -1,
      20,    81,    81,    -1,    30,    81,    81,    81,    -1,    30,
      81,    81,    -1,    11,    81,    81,    -1,    11,    81,    -1,
      12,    81,    81,    -1,    12,    81,    -1,    21,    81,    81,
      -1,    21,    81,    -1,    22,    81,    81,    -1,    22,    81,
      -1,    31,    81,    81,    -1,    31,    81,    -1,    32,    81,
      81,    -1,    32,    81,    -1,    13,    81,    -1,    23,    81,
      -1,    33,    81,    -1,    36,    81,    81,    81,    -1,    36,
      83,    -1,    36,    81,    -1,    14,    -1,    24,    -1,    34,
      -1,    38,    81,    -1,    39,    82,    -1,    40,    82,    82,
      82,    -1,    37,    -1,    41,    -1,    42,    -1,    43,    -1,
      44,    -1,    45,    -1,    47,    82,    81,    81,    81,    85,
      85,    85,    -1,    47,    82,    81,    81,    81,    85,    85,
      -1,    47,    82,    81,    81,    81,    85,    -1,    47,    82,
      81,    81,    81,    -1,    48,    81,    81,    81,    81,    -1,
      48,    81,    81,    81,    -1,    48,    81,    -1,    48,    83,
      81,    -1,    48,    83,    -1,    49,    81,    81,    81,    81,
      -1,    49,    81,    81,    -1,    49,    83,    81,    -1,    50,
      81,    81,    81,    81,    -1,    50,    81,    81,    -1,    50,
      83,    81,    -1,    51,    82,    81,    -1,    52,    82,    81,
      81,    -1,    59,    82,    81,    81,    -1,    53,    82,    81,
      81,    85,    85,    -1,    53,    82,    81,    81,    85,    -1,
      53,    82,    81,    81,    -1,    54,    82,    81,    81,    81,
      -1,    54,    82,    81,    81,    -1,    55,    82,    81,    85,
      -1,    55,    82,    81,    -1,    56,    81,    85,    -1,    56,
      81,    -1,    57,    81,    85,    -1,    57,    81,    -1,    58,
      82,    81,    81,    81,    81,    81,    81,    81,    81,    85,
      -1,    58,    82,    81,    81,    81,    81,    81,    81,    81,
      81,    -1,    58,    82,    81,    81,    81,    81,    81,    81,
      81,    -1,    61,    -1,    67,    81,    -1,    62,    -1,    67,
      82,    -1,    62,    -1,    67,    82,    -1,    61,    -1,    67,
      81,    -1,    68,    81,    69,    -1,    68,    81,    81,    81,
      69,    -1,    68,    81,    69,    -1,    68,    81,    81,    81,
      81,    69,    -1,    63,    -1,    62,    -1,    61,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  EasyMeshParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    17,    19,    21,
      23,    26,    28,    30,    32,    38,    41,    44,    50,    53,
      56,    59,    62,    65,    68,    73,    76,    79,    82,    85,
      91,    96,   100,   106,   111,   115,   121,   126,   130,   134,
     137,   141,   144,   148,   151,   157,   162,   166,   172,   177,
     181,   187,   192,   196,   201,   205,   210,   214,   219,   223,
     227,   230,   234,   237,   241,   244,   248,   251,   255,   258,
     262,   265,   268,   271,   274,   279,   282,   285,   287,   289,
     291,   294,   297,   302,   304,   306,   308,   310,   312,   314,
     323,   331,   338,   344,   350,   355,   358,   362,   365,   371,
     375,   379,   385,   389,   393,   397,   402,   407,   414,   420,
     425,   431,   436,   441,   445,   449,   452,   456,   459,   471,
     482,   492,   494,   497,   499,   502,   504,   507,   509,   512,
     516,   522,   526,   533,   535,   537
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  EasyMeshParser::yyrline_[] =
  {
         0,    91,    91,    95,    96,   100,   101,   105,   109,   113,
     114,   118,   119,   120,   124,   125,   126,   129,   130,   131,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   245,   246,   247,   248,   252,   253,   254,   255,   260,
     261,   265,   266,   271,   272,   273
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
      68,    69,     2,     2,     2,    67,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    65,     2,    66,     2,     2,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int EasyMeshParser::yyeof_ = 0;
  const int EasyMeshParser::yylast_ = 439;
  const int EasyMeshParser::yynnts_ = 16;
  const int EasyMeshParser::yyempty_ = -2;
  const int EasyMeshParser::yyfinal_ = 133;
  const int EasyMeshParser::yyterror_ = 1;
  const int EasyMeshParser::yyerrcode_ = 256;
  const int EasyMeshParser::yyntokens_ = 70;

  const unsigned int EasyMeshParser::yyuser_token_number_max_ = 319;
  const EasyMeshParser::token_number_type EasyMeshParser::yyundef_token_ = 2;


} // lol

/* Line 1053 of lalr1.cc  */
#line 2015 "generated/easymesh-parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 276 "easymesh/easymesh-parser.y"


void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}


