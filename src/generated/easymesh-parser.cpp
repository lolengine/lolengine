/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
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

/* Line 293 of lalr1.cc  */
#line 1 "easymesh/easymesh-parser.y"

//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2012 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2012 Benjamin Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "easymesh/easymesh.h"

#include <string>



/* Line 293 of lalr1.cc  */
#line 66 "generated/easymesh-parser.cpp"


#include "easymesh-parser.h"

/* User implementation prologue.  */

/* Line 299 of lalr1.cc  */
#line 65 "easymesh/easymesh-parser.y"

#include "easymesh/easymesh-compiler.h"

#undef yylex
#define yylex mc.m_lexer->lex


/* Line 299 of lalr1.cc  */
#line 83 "generated/easymesh-parser.cpp"

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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                               \
 do                                                                    \
   if (N)                                                              \
     {                                                                 \
       (Current).begin = YYRHSLOC (Rhs, 1).begin;                      \
       (Current).end   = YYRHSLOC (Rhs, N).end;                        \
     }                                                                 \
   else                                                                \
     {                                                                 \
       (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;        \
     }                                                                 \
 while (false)
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

/* Line 382 of lalr1.cc  */
#line 169 "generated/easymesh-parser.cpp"

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
    location_type yyerror_range[3];

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

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 7:

/* Line 690 of lalr1.cc  */
#line 89 "easymesh/easymesh-parser.y"
    { mc.m_mesh.OpenBrace(); }
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 93 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CloseBrace(); }
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 108 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3)); }
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 109 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                      vec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                      mc.m_mesh.SetCurColor(vec4(v) * (1. / 255)); }
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 112 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3)); }
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 113 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                      vec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                      mc.m_mesh.SetCurColor2(vec4(v) * (1. / 255)); }
    break;

  case 18:

/* Line 690 of lalr1.cc  */
#line 119 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Chamfer((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 19:

/* Line 690 of lalr1.cc  */
#line 120 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].args).f0, 0, 0)); }
    break;

  case 20:

/* Line 690 of lalr1.cc  */
#line 121 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0, (yysemantic_stack_[(2) - (2)].args).f0, 0)); }
    break;

  case 21:

/* Line 690 of lalr1.cc  */
#line 122 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0, 0, (yysemantic_stack_[(2) - (2)].args).f0)); }
    break;

  case 22:

/* Line 690 of lalr1.cc  */
#line 123 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 23:

/* Line 690 of lalr1.cc  */
#line 124 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateX((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 24:

/* Line 690 of lalr1.cc  */
#line 125 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateY((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 25:

/* Line 690 of lalr1.cc  */
#line 126 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateZ((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 26:

/* Line 690 of lalr1.cc  */
#line 127 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 27:

/* Line 690 of lalr1.cc  */
#line 128 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 28:

/* Line 690 of lalr1.cc  */
#line 129 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 29:

/* Line 690 of lalr1.cc  */
#line 130 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].args).f0, 0, 0)); }
    break;

  case 30:

/* Line 690 of lalr1.cc  */
#line 131 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(0, (yysemantic_stack_[(2) - (2)].args).f0, 0)); }
    break;

  case 31:

/* Line 690 of lalr1.cc  */
#line 132 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(0, 0, (yysemantic_stack_[(2) - (2)].args).f0)); }
    break;

  case 32:

/* Line 690 of lalr1.cc  */
#line 133 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 33:

/* Line 690 of lalr1.cc  */
#line 134 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorX(); }
    break;

  case 34:

/* Line 690 of lalr1.cc  */
#line 135 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorY(); }
    break;

  case 35:

/* Line 690 of lalr1.cc  */
#line 136 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorZ(); }
    break;

  case 36:

/* Line 690 of lalr1.cc  */
#line 140 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                 (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3,
                                                 (int)(yysemantic_stack_[(2) - (2)].args).f4, (int)(yysemantic_stack_[(2) - (2)].args).f5); }
    break;

  case 37:

/* Line 690 of lalr1.cc  */
#line 143 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 38:

/* Line 690 of lalr1.cc  */
#line 144 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                            (yysemantic_stack_[(2) - (2)].args).f2), (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 39:

/* Line 690 of lalr1.cc  */
#line 146 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                          (yysemantic_stack_[(2) - (2)].args).f2), (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 40:

/* Line 690 of lalr1.cc  */
#line 148 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSphere((yysemantic_stack_[(2) - (2)].args).f0,
                                               vec3((yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3)); }
    break;

  case 41:

/* Line 690 of lalr1.cc  */
#line 150 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2,
                                             (int)(yysemantic_stack_[(2) - (2)].args).f3, (int)(yysemantic_stack_[(2) - (2)].args).f4); }
    break;

  case 42:

/* Line 690 of lalr1.cc  */
#line 152 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                     (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 43:

/* Line 690 of lalr1.cc  */
#line 154 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (int)(yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 44:

/* Line 690 of lalr1.cc  */
#line 155 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(2) - (2)].args).f0, (int)(yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 45:

/* Line 690 of lalr1.cc  */
#line 156 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(2) - (2)].args).f0, (int)(yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 46:

/* Line 690 of lalr1.cc  */
#line 157 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3,
                                            (yysemantic_stack_[(2) - (2)].args).f4, (yysemantic_stack_[(2) - (2)].args).f5, (yysemantic_stack_[(2) - (2)].args).f6, (int)(yysemantic_stack_[(2) - (2)].args).f7); }
    break;

  case 47:

/* Line 690 of lalr1.cc  */
#line 161 "easymesh/easymesh-parser.y"
    { (yyval.args).f0 = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 48:

/* Line 690 of lalr1.cc  */
#line 162 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f1 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 49:

/* Line 690 of lalr1.cc  */
#line 163 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f2 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 50:

/* Line 690 of lalr1.cc  */
#line 164 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f3 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 51:

/* Line 690 of lalr1.cc  */
#line 165 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f4 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 52:

/* Line 690 of lalr1.cc  */
#line 166 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f5 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 53:

/* Line 690 of lalr1.cc  */
#line 167 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f6 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 54:

/* Line 690 of lalr1.cc  */
#line 168 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f7 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 55:

/* Line 690 of lalr1.cc  */
#line 171 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 56:

/* Line 690 of lalr1.cc  */
#line 172 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;



/* Line 690 of lalr1.cc  */
#line 797 "generated/easymesh-parser.cpp"
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

    char const* yyformat = 0;
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
  const signed char EasyMeshParser::yypact_ninf_ = -45;
  const signed char
  EasyMeshParser::yypact_[] =
  {
        34,   -29,    70,    -6,    -6,    -6,    -6,   -45,    -6,    -6,
      -6,    -6,   -45,    -6,    -6,    -6,    -6,   -45,    -6,    -6,
      -6,    -6,    -6,    -6,    -6,    -6,    -6,    -6,    -6,    -6,
      -6,    -6,   -45,    12,    15,    34,    34,    69,   -45,   -45,
     -45,   -45,   -45,   -45,    -6,    -6,    -6,    -6,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,    -6,    -6,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,    -6,
      -6,   -45,   -45,   -45,   -45,   -21,   -45,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  EasyMeshParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,    34,     0,     0,     0,     0,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     0,     0,     3,     0,     5,     9,    11,
      12,    13,    55,    15,     0,     0,     0,     0,    14,    47,
      17,    16,    19,    23,    26,    29,    20,    24,    27,    30,
      21,    25,    28,    31,    22,    32,    18,     0,     0,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     0,
       0,    46,     1,     2,     4,     0,    10,    56,    48,    49,
      50,    51,    52,    53,    54,     8,     6
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  EasyMeshParser::yypgoto_[] =
  {
       -45,   -45,    -4,   -45,   -45,   -45,   -45,   -17,   -45,   -45,
     -45,     5,    39,    98,     3,     8,    -9,   -45,   -45,   -44
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  EasyMeshParser::yydefgoto_[] =
  {
        -1,    33,    34,    35,    36,    96,    37,    38,    39,    40,
      41,    45,    46,    47,    67,    68,    69,    80,    81,    49
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char EasyMeshParser::yytable_ninf_ = -1;
  const unsigned char
  EasyMeshParser::yytable_[] =
  {
        87,    88,    89,    90,    48,    51,    42,    43,    52,    53,
      44,    55,    82,    56,    57,    83,    59,    95,    60,    61,
      86,    63,    79,    91,    92,    66,    71,    72,    73,    42,
      75,    84,    85,    44,    74,    93,    94,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    77,    78,
       0,    32,     1,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    54,     0,    42,    50,     0,    58,    44,
       0,     0,     0,    62,     0,     0,    64,    65,     0,     0,
      70,     0,     0,     0,     0,     0,    76
  };

  /* YYCHECK.  */
  const signed char
  EasyMeshParser::yycheck_[] =
  {
        44,    45,    46,    47,     1,     2,    35,    36,     3,     4,
      39,     6,     0,     8,     9,     0,    11,    38,    13,    14,
      37,    16,    31,    67,    68,    20,    23,    24,    25,    35,
      27,    35,    36,    39,    26,    79,    80,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    29,    30,
      -1,    37,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,     5,    -1,    35,    36,    -1,    10,    39,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    28
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  EasyMeshParser::yystos_[] =
  {
         0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    37,    41,    42,    43,    44,    46,    47,    48,
      49,    50,    35,    36,    39,    51,    52,    53,    54,    59,
      36,    54,    51,    51,    53,    51,    51,    51,    53,    51,
      51,    51,    53,    51,    53,    53,    51,    54,    55,    56,
      53,    54,    54,    54,    55,    54,    53,    52,    52,    56,
      57,    58,     0,     0,    42,    42,    47,    59,    59,    59,
      59,    59,    59,    59,    59,    38,    45
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
     285,   286,   287,   288,   289,   290,   291,    91,    93,    45
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  EasyMeshParser::yyr1_[] =
  {
         0,    40,    41,    42,    42,    43,    43,    44,    45,    46,
      46,    47,    47,    47,    48,    48,    48,    48,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    59
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  EasyMeshParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     3,     1,     1,     1,
       2,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     1,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const EasyMeshParser::yytname_[] =
  {
    "T_END", "error", "$undefined", "T_COLOR", "T_BGCOLOR", "T_TRANSLATEX",
  "T_ROTATEX", "T_TAPERX", "T_SCALEX", "T_MIRRORX", "T_TRANSLATEY",
  "T_ROTATEY", "T_TAPERY", "T_SCALEY", "T_MIRRORY", "T_TRANSLATEZ",
  "T_ROTATEZ", "T_TAPERZ", "T_SCALEZ", "T_MIRRORZ", "T_TRANSLATE",
  "T_SCALE", "T_CHAMFER", "T_CYLINDER", "T_BOX", "T_SMOOTHCHAMFBOX",
  "T_FLATCHAMFBOX", "T_SPHERE", "T_STAR", "T_EXPANDEDSTAR", "T_DISC",
  "T_TRIANGLE", "T_QUAD", "T_COG", "T_ERROR", "NUMBER", "COLOR", "'['",
  "']'", "'-'", "$accept", "mesh_description", "mesh_expression_list",
  "mesh_expression", "mesh_open", "mesh_close", "mesh_command_list",
  "mesh_command", "color_command", "transform_command",
  "primitive_command", "args1", "args2", "args3", "args4", "args5",
  "args6", "args7", "args8", "number", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const EasyMeshParser::rhs_number_type
  EasyMeshParser::yyrhs_[] =
  {
        41,     0,    -1,    42,     0,    -1,    43,    -1,    43,    42,
      -1,    46,    -1,    44,    42,    45,    -1,    37,    -1,    38,
      -1,    47,    -1,    46,    47,    -1,    48,    -1,    49,    -1,
      50,    -1,     3,    54,    -1,     3,    36,    -1,     4,    54,
      -1,     4,    36,    -1,    22,    51,    -1,     5,    51,    -1,
      10,    51,    -1,    15,    51,    -1,    20,    53,    -1,     6,
      51,    -1,    11,    51,    -1,    16,    51,    -1,     7,    53,
      -1,    12,    53,    -1,    17,    53,    -1,     8,    51,    -1,
      13,    51,    -1,    18,    51,    -1,    21,    53,    -1,     9,
      -1,    14,    -1,    19,    -1,    23,    56,    -1,    24,    53,
      -1,    25,    54,    -1,    26,    54,    -1,    27,    54,    -1,
      28,    55,    -1,    29,    54,    -1,    30,    53,    -1,    31,
      52,    -1,    32,    52,    -1,    33,    58,    -1,    59,    -1,
      51,    59,    -1,    52,    59,    -1,    53,    59,    -1,    54,
      59,    -1,    55,    59,    -1,    56,    59,    -1,    57,    59,
      -1,    35,    -1,    39,    59,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  EasyMeshParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    17,    19,    21,
      23,    26,    28,    30,    32,    35,    38,    41,    44,    47,
      50,    53,    56,    59,    62,    65,    68,    71,    74,    77,
      80,    83,    86,    89,    91,    93,    95,    98,   101,   104,
     107,   110,   113,   116,   119,   122,   125,   128,   130,   133,
     136,   139,   142,   145,   148,   151,   153
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  EasyMeshParser::yyrline_[] =
  {
         0,    75,    75,    79,    80,    84,    85,    89,    93,    97,
      98,   102,   103,   104,   108,   109,   112,   113,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   140,   143,   144,   146,
     148,   150,   152,   154,   155,   156,   157,   161,   162,   163,
     164,   165,   166,   167,   168,   171,   172
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
       2,     2,     2,     2,     2,    39,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,     2,     2,     2,     2,     2,     2,
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
      35,    36
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int EasyMeshParser::yyeof_ = 0;
  const int EasyMeshParser::yylast_ = 126;
  const int EasyMeshParser::yynnts_ = 20;
  const int EasyMeshParser::yyempty_ = -2;
  const int EasyMeshParser::yyfinal_ = 82;
  const int EasyMeshParser::yyterror_ = 1;
  const int EasyMeshParser::yyerrcode_ = 256;
  const int EasyMeshParser::yyntokens_ = 40;

  const unsigned int EasyMeshParser::yyuser_token_number_max_ = 291;
  const EasyMeshParser::token_number_type EasyMeshParser::yyundef_token_ = 2;


} // lol

/* Line 1136 of lalr1.cc  */
#line 1375 "generated/easymesh-parser.cpp"


/* Line 1138 of lalr1.cc  */
#line 175 "easymesh/easymesh-parser.y"


void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}


