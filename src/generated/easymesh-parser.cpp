/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2013 Free Software Foundation, Inc.
   
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
/* Line 283 of lalr1.cc  */
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

/* HACK: workaround for Bison who insists on using exceptions */
#define try if (true)
#define catch(...) if (false)
#define throw (void)0

/* Line 283 of lalr1.cc  */
#line 68 "generated/easymesh-parser.cpp"


#include "easymesh-parser.h"

/* User implementation prologue.  */
/* Line 289 of lalr1.cc  */
#line 70 "easymesh/easymesh-parser.y"

#include "easymesh/easymesh-compiler.h"

#undef yylex
#define yylex mc.m_lexer->lex

/* Line 289 of lalr1.cc  */
#line 83 "generated/easymesh-parser.cpp"


# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

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

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


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
# define YY_SYMBOL_PRINT(Title, Type, Value, Location) YYUSE(Type)
# define YY_REDUCE_PRINT(Rule)        static_cast<void>(0)
# define YY_STACK_PRINT()             static_cast<void>(0)

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace lol {
/* Line 357 of lalr1.cc  */
#line 178 "generated/easymesh-parser.cpp"

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
    std::ostream& yyo = debug_stream ();
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    YYUSE (yytype);
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

    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    YYUSE (yytype);
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

  inline bool
  EasyMeshParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  EasyMeshParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  EasyMeshParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    // State.
    int yyn;
    int yylen = 0;
    int yystate = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    static semantic_type yyval_default;
    semantic_type yylval = yyval_default;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_.clear ();
    yysemantic_stack_.clear ();
    yylocation_stack_.clear ();
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
    if (yy_pact_value_is_default_ (yyn))
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
	if (yy_table_value_is_error_ (yyn))
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

    // Compute the default @$.
    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
          case 7:
/* Line 664 of lalr1.cc  */
#line 94 "easymesh/easymesh-parser.y"
    { mc.m_mesh.OpenBrace(); }
    break;

  case 8:
/* Line 664 of lalr1.cc  */
#line 98 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CloseBrace(); }
    break;

  case 14:
/* Line 664 of lalr1.cc  */
#line 113 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3)); }
    break;

  case 15:
/* Line 664 of lalr1.cc  */
#line 114 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                      vec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                      mc.m_mesh.SetCurColor(vec4(v) * (1.f / 255)); }
    break;

  case 16:
/* Line 664 of lalr1.cc  */
#line 117 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3)); }
    break;

  case 17:
/* Line 664 of lalr1.cc  */
#line 118 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                      vec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                      mc.m_mesh.SetCurColor2(vec4(v) * (1.f / 255)); }
    break;

  case 18:
/* Line 664 of lalr1.cc  */
#line 124 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Chamfer((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 19:
/* Line 664 of lalr1.cc  */
#line 125 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].args).f0, 0, 0)); }
    break;

  case 20:
/* Line 664 of lalr1.cc  */
#line 126 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0, (yysemantic_stack_[(2) - (2)].args).f0, 0)); }
    break;

  case 21:
/* Line 664 of lalr1.cc  */
#line 127 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0, 0, (yysemantic_stack_[(2) - (2)].args).f0)); }
    break;

  case 22:
/* Line 664 of lalr1.cc  */
#line 128 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 23:
/* Line 664 of lalr1.cc  */
#line 129 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateX((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 24:
/* Line 664 of lalr1.cc  */
#line 130 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateY((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 25:
/* Line 664 of lalr1.cc  */
#line 131 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateZ((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 26:
/* Line 664 of lalr1.cc  */
#line 132 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 27:
/* Line 664 of lalr1.cc  */
#line 133 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 28:
/* Line 664 of lalr1.cc  */
#line 134 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 29:
/* Line 664 of lalr1.cc  */
#line 135 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 30:
/* Line 664 of lalr1.cc  */
#line 136 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 31:
/* Line 664 of lalr1.cc  */
#line 137 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 32:
/* Line 664 of lalr1.cc  */
#line 138 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 33:
/* Line 664 of lalr1.cc  */
#line 139 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 34:
/* Line 664 of lalr1.cc  */
#line 140 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TwistZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 35:
/* Line 664 of lalr1.cc  */
#line 141 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 36:
/* Line 664 of lalr1.cc  */
#line 142 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 37:
/* Line 664 of lalr1.cc  */
#line 143 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 38:
/* Line 664 of lalr1.cc  */
#line 144 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 39:
/* Line 664 of lalr1.cc  */
#line 145 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 40:
/* Line 664 of lalr1.cc  */
#line 146 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ShearZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 41:
/* Line 664 of lalr1.cc  */
#line 147 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 42:
/* Line 664 of lalr1.cc  */
#line 148 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 43:
/* Line 664 of lalr1.cc  */
#line 149 "easymesh/easymesh-parser.y"
    { mc.m_mesh.StretchZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 44:
/* Line 664 of lalr1.cc  */
#line 150 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 45:
/* Line 664 of lalr1.cc  */
#line 151 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendXZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 46:
/* Line 664 of lalr1.cc  */
#line 152 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 47:
/* Line 664 of lalr1.cc  */
#line 153 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendYZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 48:
/* Line 664 of lalr1.cc  */
#line 154 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 49:
/* Line 664 of lalr1.cc  */
#line 155 "easymesh/easymesh-parser.y"
    { mc.m_mesh.BendZY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 50:
/* Line 664 of lalr1.cc  */
#line 156 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].args).f0, 1.0, 1.0)); }
    break;

  case 51:
/* Line 664 of lalr1.cc  */
#line 157 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.0, (yysemantic_stack_[(2) - (2)].args).f0, 1.0)); }
    break;

  case 52:
/* Line 664 of lalr1.cc  */
#line 158 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.0, 1.0, (yysemantic_stack_[(2) - (2)].args).f0)); }
    break;

  case 53:
/* Line 664 of lalr1.cc  */
#line 159 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 54:
/* Line 664 of lalr1.cc  */
#line 160 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorX(); }
    break;

  case 55:
/* Line 664 of lalr1.cc  */
#line 161 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorY(); }
    break;

  case 56:
/* Line 664 of lalr1.cc  */
#line 162 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorZ(); }
    break;

  case 57:
/* Line 664 of lalr1.cc  */
#line 163 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RadialJitter((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 58:
/* Line 664 of lalr1.cc  */
#line 164 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SplitTriangles((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 59:
/* Line 664 of lalr1.cc  */
#line 165 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SmoothMesh((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 60:
/* Line 664 of lalr1.cc  */
#line 166 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleScaleWinding(); }
    break;

  case 61:
/* Line 664 of lalr1.cc  */
#line 167 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgUnion(); }
    break;

  case 62:
/* Line 664 of lalr1.cc  */
#line 168 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgSubstract(); }
    break;

  case 63:
/* Line 664 of lalr1.cc  */
#line 169 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgSubstractLoss(); }
    break;

  case 64:
/* Line 664 of lalr1.cc  */
#line 170 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgAnd(); }
    break;

  case 65:
/* Line 664 of lalr1.cc  */
#line 171 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CsgXor(); }
    break;

  case 66:
/* Line 664 of lalr1.cc  */
#line 175 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                 (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3,
                                                 (int)(yysemantic_stack_[(2) - (2)].args).f4, (int)(yysemantic_stack_[(2) - (2)].args).f5, 0); }
    break;

  case 67:
/* Line 664 of lalr1.cc  */
#line 178 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                 (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3,
                                                 (int)(yysemantic_stack_[(2) - (2)].args).f4, (int)(yysemantic_stack_[(2) - (2)].args).f5, (int)(yysemantic_stack_[(2) - (2)].args).f6); }
    break;

  case 68:
/* Line 664 of lalr1.cc  */
#line 181 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 69:
/* Line 664 of lalr1.cc  */
#line 182 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                            (yysemantic_stack_[(2) - (2)].args).f2), (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 70:
/* Line 664 of lalr1.cc  */
#line 184 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                          (yysemantic_stack_[(2) - (2)].args).f2), (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 71:
/* Line 664 of lalr1.cc  */
#line 186 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSphere((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 72:
/* Line 664 of lalr1.cc  */
#line 187 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCapsule((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 73:
/* Line 664 of lalr1.cc  */
#line 188 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendTorus((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 74:
/* Line 664 of lalr1.cc  */
#line 189 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2,
                                             (int)(yysemantic_stack_[(2) - (2)].args).f3, (int)(yysemantic_stack_[(2) - (2)].args).f4); }
    break;

  case 75:
/* Line 664 of lalr1.cc  */
#line 191 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                     (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 76:
/* Line 664 of lalr1.cc  */
#line 193 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (int)(yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 77:
/* Line 664 of lalr1.cc  */
#line 194 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(2) - (2)].args).f0, (int)(yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 78:
/* Line 664 of lalr1.cc  */
#line 195 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(2) - (2)].args).f0, (int)(yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 79:
/* Line 664 of lalr1.cc  */
#line 196 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                 (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3, (yysemantic_stack_[(2) - (2)].args).f4, (yysemantic_stack_[(2) - (2)].args).f5, (yysemantic_stack_[(2) - (2)].args).f6,
                                 (yysemantic_stack_[(2) - (2)].args).f7, (yysemantic_stack_[(2) - (2)].args).f8, (int)(yysemantic_stack_[(2) - (2)].args).f9); }
    break;

  case 80:
/* Line 664 of lalr1.cc  */
#line 201 "easymesh/easymesh-parser.y"
    { (yyval.args).f0 = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 81:
/* Line 664 of lalr1.cc  */
#line 202 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f1 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 82:
/* Line 664 of lalr1.cc  */
#line 203 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f2 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 83:
/* Line 664 of lalr1.cc  */
#line 204 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f3 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 84:
/* Line 664 of lalr1.cc  */
#line 205 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f4 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 85:
/* Line 664 of lalr1.cc  */
#line 206 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f5 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 86:
/* Line 664 of lalr1.cc  */
#line 207 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f6 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 87:
/* Line 664 of lalr1.cc  */
#line 208 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f7 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 88:
/* Line 664 of lalr1.cc  */
#line 209 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f8 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 89:
/* Line 664 of lalr1.cc  */
#line 210 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f9 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 90:
/* Line 664 of lalr1.cc  */
#line 213 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 91:
/* Line 664 of lalr1.cc  */
#line 214 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;


/* Line 664 of lalr1.cc  */
#line 974 "generated/easymesh-parser.cpp"
      default:
        break;
      }

    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
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
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
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

    yyerror_range[1] = yylocation_stack_[yylen - 1];
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
	if (!yy_pact_value_is_default_ (yyn))
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

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
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
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystate_stack_.height ())
      {
        yydestruct_ ("Cleanup: popping",
                     yystos_[yystate_stack_[0]],
                     &yysemantic_stack_[0],
                     &yylocation_stack_[0]);
        yypop_ ();
      }

    return yyresult;
    }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (yychar != yyempty_)
          {
            /* Make sure we have latest lookahead translation.  See
               comments at user semantic actions for why this is
               necessary.  */
            yytoken = yytranslate_ (yychar);
            yydestruct_ (YY_NULL, yytoken, &yylval, &yylloc);
          }

        while (1 < yystate_stack_.height ())
          {
            yydestruct_ (YY_NULL,
                         yystos_[yystate_stack_[0]],
                         &yysemantic_stack_[0],
                         &yylocation_stack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  // Generate an error message.
  std::string
  EasyMeshParser::yysyntax_error_ (int yystate, int yytoken)
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yychar.
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULL;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char EasyMeshParser::yypact_ninf_ = -71;
  const signed char
  EasyMeshParser::yypact_[] =
  {
        62,   -42,   -25,   -49,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -71,   -49,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -71,   -49,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -71,   -49,   -49,   -71,   -49,   -49,   -49,   -71,
     -71,   -71,   -71,   -71,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,   -71,     4,
       6,    62,    62,   123,   -71,   -71,   -71,   -71,   -71,   -71,
     -49,   -49,   -49,   -49,   -71,   -71,   -71,   -71,   -71,   -71,
     -49,   -71,   -71,   -49,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -49,   -71,   -71,   -49,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -49,   -71,   -71,   -49,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -49,   -49,   -49,
     -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -49,   -49,   -49,   -49,   -71,   -71,   -71,   -71,   -71,
     -55,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  EasyMeshParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,    60,     0,     0,     0,    61,
      62,    63,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     0,
       0,     3,     0,     5,     9,    11,    12,    13,    90,    15,
       0,     0,     0,     0,    14,    80,    17,    16,    19,    23,
      26,    27,    32,    35,    36,    41,    44,    45,    50,    20,
      24,    28,    29,    33,    37,    38,    42,    46,    47,    51,
      21,    25,    30,    31,    34,    39,    40,    43,    48,    49,
      52,    22,    53,    57,    58,    59,    18,     0,     0,    66,
      67,    68,    69,    70,    71,    72,    74,    75,    76,    77,
      78,     0,     0,     0,     0,    79,    73,     1,     2,     4,
       0,    10,    91,    81,    82,    83,    84,    85,    86,    87,
      88,    89,     8,     6
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yypgoto_[] =
  {
       -71,   -71,   -32,   -71,   -71,   -71,   -71,   -52,   -71,   -71,
     -71,   232,   184,     0,   182,   -37,   -34,   -30,   -71,   -71,
     -71,   -70
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  EasyMeshParser::yydefgoto_[] =
  {
        -1,    59,    60,    61,    62,   153,    63,    64,    65,    66,
      67,    71,    72,    73,   117,   118,   119,   120,   133,   134,
     135,    75
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char EasyMeshParser::yytable_ninf_ = -1;
  const unsigned char
  EasyMeshParser::yytable_[] =
  {
       142,   143,   144,   145,   137,    80,   138,    83,    85,   152,
     145,   141,    68,   145,   126,    91,    70,    94,    96,    68,
      69,   145,   131,    70,   145,   102,   132,   105,   107,   139,
     140,     0,   145,   111,   112,   145,    68,    76,   115,     0,
      70,     0,     0,     0,     0,     0,   121,   146,   147,   148,
     125,     0,     0,   128,     0,     0,     0,   136,     0,     0,
       0,   148,   149,   150,   151,     1,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,     0,     0,    58,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    74,    77,     0,     0,    81,     0,    84,
      82,     0,     0,    86,    87,     0,     0,    92,     0,    95,
      93,     0,     0,    97,    98,     0,     0,   103,     0,   106,
     104,     0,     0,   108,   109,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   122,
     123,     0,     0,   124,   127,    78,    79,     0,   129,   130,
       0,     0,     0,    88,     0,    89,    90,     0,     0,     0,
       0,     0,     0,    99,     0,   100,   101,     0,     0,     0,
       0,     0,     0,   110,     0,     0,     0,     0,   113,   114,
       0,     0,     0,     0,     0,     0,   116
  };

  /* YYCHECK.  */
  const short int
  EasyMeshParser::yycheck_[] =
  {
        70,    71,    72,    73,     0,     5,     0,     7,     8,    64,
      80,    63,    61,    83,    51,    15,    65,    17,    18,    61,
      62,    91,    56,    65,    94,    25,    56,    27,    28,    61,
      62,    -1,   102,    33,    34,   105,    61,    62,    38,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    46,   117,   118,   119,
      50,    -1,    -1,    53,    -1,    -1,    -1,    57,    -1,    -1,
      -1,   131,   132,   133,   134,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    -1,    -1,    63,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,     1,     2,    -1,    -1,     5,    -1,     7,
       6,    -1,    -1,     9,    10,    -1,    -1,    15,    -1,    17,
      16,    -1,    -1,    19,    20,    -1,    -1,    25,    -1,    27,
      26,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      48,    -1,    -1,    49,    52,     3,     4,    -1,    54,    55,
      -1,    -1,    -1,    11,    -1,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    44
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
      52,    53,    54,    55,    56,    57,    58,    59,    63,    67,
      68,    69,    70,    72,    73,    74,    75,    76,    61,    62,
      65,    77,    78,    79,    80,    87,    62,    80,    77,    77,
      79,    80,    78,    79,    80,    79,    78,    78,    77,    77,
      77,    79,    80,    78,    79,    80,    79,    78,    78,    77,
      77,    77,    79,    80,    78,    79,    80,    79,    78,    78,
      77,    79,    79,    77,    77,    79,    77,    80,    81,    82,
      83,    79,    80,    80,    78,    79,    81,    80,    79,    78,
      78,    82,    83,    84,    85,    86,    79,     0,     0,    68,
      68,    73,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    64,    71
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
     315,   316,   317,    91,    93,    45
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  EasyMeshParser::yyr1_[] =
  {
         0,    66,    67,    68,    68,    69,    69,    70,    71,    72,
      72,    73,    73,    73,    74,    74,    74,    74,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    87
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  EasyMeshParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     3,     1,     1,     1,
       2,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     2
  };


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
  "T_ERROR", "NUMBER", "COLOR", "'['", "']'", "'-'", "$accept",
  "mesh_description", "mesh_expression_list", "mesh_expression",
  "mesh_open", "mesh_close", "mesh_command_list", "mesh_command",
  "color_command", "transform_command", "primitive_command", "args1",
  "args2", "args3", "args4", "args5", "args6", "args7", "args8", "args9",
  "args10", "number", YY_NULL
  };

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const EasyMeshParser::rhs_number_type
  EasyMeshParser::yyrhs_[] =
  {
        67,     0,    -1,    68,     0,    -1,    69,    -1,    69,    68,
      -1,    72,    -1,    70,    68,    71,    -1,    63,    -1,    64,
      -1,    73,    -1,    72,    73,    -1,    74,    -1,    75,    -1,
      76,    -1,     3,    80,    -1,     3,    62,    -1,     4,    80,
      -1,     4,    62,    -1,    46,    77,    -1,     5,    77,    -1,
      15,    77,    -1,    25,    77,    -1,    35,    79,    -1,     6,
      77,    -1,    16,    77,    -1,    26,    77,    -1,     7,    79,
      -1,     7,    80,    -1,    17,    79,    -1,    17,    80,    -1,
      27,    79,    -1,    27,    80,    -1,     8,    78,    -1,    18,
      78,    -1,    28,    78,    -1,     9,    79,    -1,     9,    80,
      -1,    19,    79,    -1,    19,    80,    -1,    29,    79,    -1,
      29,    80,    -1,    10,    79,    -1,    20,    79,    -1,    30,
      79,    -1,    11,    78,    -1,    12,    78,    -1,    21,    78,
      -1,    22,    78,    -1,    31,    78,    -1,    32,    78,    -1,
      13,    77,    -1,    23,    77,    -1,    33,    77,    -1,    36,
      79,    -1,    14,    -1,    24,    -1,    34,    -1,    38,    77,
      -1,    39,    77,    -1,    40,    79,    -1,    37,    -1,    41,
      -1,    42,    -1,    43,    -1,    44,    -1,    45,    -1,    47,
      82,    -1,    47,    83,    -1,    48,    79,    -1,    49,    80,
      -1,    50,    80,    -1,    51,    78,    -1,    52,    79,    -1,
      59,    79,    -1,    53,    81,    -1,    54,    80,    -1,    55,
      79,    -1,    56,    78,    -1,    57,    78,    -1,    58,    86,
      -1,    87,    -1,    77,    87,    -1,    78,    87,    -1,    79,
      87,    -1,    80,    87,    -1,    81,    87,    -1,    82,    87,
      -1,    83,    87,    -1,    84,    87,    -1,    85,    87,    -1,
      61,    -1,    65,    87,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  EasyMeshParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    17,    19,    21,
      23,    26,    28,    30,    32,    35,    38,    41,    44,    47,
      50,    53,    56,    59,    62,    65,    68,    71,    74,    77,
      80,    83,    86,    89,    92,    95,    98,   101,   104,   107,
     110,   113,   116,   119,   122,   125,   128,   131,   134,   137,
     140,   143,   146,   149,   152,   154,   156,   158,   161,   164,
     167,   169,   171,   173,   175,   177,   179,   182,   185,   188,
     191,   194,   197,   200,   203,   206,   209,   212,   215,   218,
     221,   223,   226,   229,   232,   235,   238,   241,   244,   247,
     250,   252
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  EasyMeshParser::yyrline_[] =
  {
         0,    80,    80,    84,    85,    89,    90,    94,    98,   102,
     103,   107,   108,   109,   113,   114,   117,   118,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   175,   178,   181,   182,
     184,   186,   187,   188,   189,   191,   193,   194,   195,   196,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     213,   214
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
       2,     2,     2,     2,     2,    65,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    63,     2,    64,     2,     2,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int EasyMeshParser::yyeof_ = 0;
  const int EasyMeshParser::yylast_ = 276;
  const int EasyMeshParser::yynnts_ = 22;
  const int EasyMeshParser::yyempty_ = -2;
  const int EasyMeshParser::yyfinal_ = 137;
  const int EasyMeshParser::yyterror_ = 1;
  const int EasyMeshParser::yyerrcode_ = 256;
  const int EasyMeshParser::yyntokens_ = 66;

  const unsigned int EasyMeshParser::yyuser_token_number_max_ = 317;
  const EasyMeshParser::token_number_type EasyMeshParser::yyundef_token_ = 2;


} // lol
/* Line 1135 of lalr1.cc  */
#line 1663 "generated/easymesh-parser.cpp"
/* Line 1136 of lalr1.cc  */
#line 217 "easymesh/easymesh-parser.y"


void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}

