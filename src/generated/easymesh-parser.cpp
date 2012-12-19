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



/* Line 310 of lalr1.cc  */
#line 66 "generated/easymesh-parser.cpp"


#include "easymesh-parser.h"

/* User implementation prologue.  */

/* Line 316 of lalr1.cc  */
#line 65 "easymesh/easymesh-parser.y"

#include "easymesh/easymesh-compiler.h"

#undef yylex
#define yylex mc.m_lexer->lex


/* Line 316 of lalr1.cc  */
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
#line 149 "generated/easymesh-parser.cpp"
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
#line 89 "easymesh/easymesh-parser.y"
    { mc.m_mesh.OpenBrace(); }
    break;

  case 8:

/* Line 677 of lalr1.cc  */
#line 93 "easymesh/easymesh-parser.y"
    { mc.m_mesh.CloseBrace(); }
    break;

  case 14:

/* Line 677 of lalr1.cc  */
#line 108 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor(vec4((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3)); }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 109 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                      vec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                      mc.m_mesh.SetCurColor(vec4(v) * (1. / 255)); }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 112 "easymesh/easymesh-parser.y"
    { mc.m_mesh.SetCurColor2(vec4((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3)); }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 113 "easymesh/easymesh-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                      vec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                      mc.m_mesh.SetCurColor2(vec4(v) * (1. / 255)); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 119 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Chamfer((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 19:

/* Line 677 of lalr1.cc  */
#line 120 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].args).f0, 0, 0)); }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 121 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0, (yysemantic_stack_[(2) - (2)].args).f0, 0)); }
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 122 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3(0, 0, (yysemantic_stack_[(2) - (2)].args).f0)); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 123 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Translate(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 124 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateX((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 125 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateY((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 126 "easymesh/easymesh-parser.y"
    { mc.m_mesh.RotateZ((yysemantic_stack_[(2) - (2)].args).f0); }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 127 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperX((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 128 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperY((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 129 "easymesh/easymesh-parser.y"
    { mc.m_mesh.TaperZ((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 130 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].args).f0, 1.0, 1.0)); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 131 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.0, (yysemantic_stack_[(2) - (2)].args).f0, 1.0)); }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 132 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3(1.0, 1.0, (yysemantic_stack_[(2) - (2)].args).f0)); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 133 "easymesh/easymesh-parser.y"
    { mc.m_mesh.Scale(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 134 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorX(); }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 135 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorY(); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 136 "easymesh/easymesh-parser.y"
    { mc.m_mesh.MirrorZ(); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 137 "easymesh/easymesh-parser.y"
    { mc.m_mesh.ToggleScaleWinding(); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 141 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCylinder((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                 (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3,
                                                 (int)(yysemantic_stack_[(2) - (2)].args).f4, (int)(yysemantic_stack_[(2) - (2)].args).f5); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 144 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2)); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 145 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSmoothChamfBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                            (yysemantic_stack_[(2) - (2)].args).f2), (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 147 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendFlatChamfBox(vec3((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                          (yysemantic_stack_[(2) - (2)].args).f2), (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 149 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSphere((yysemantic_stack_[(2) - (2)].args).f0,
                                               vec3((yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3)); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 151 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCapsule((yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 43:

/* Line 677 of lalr1.cc  */
#line 152 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendTorus((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 44:

/* Line 677 of lalr1.cc  */
#line 153 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendStar((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (yysemantic_stack_[(2) - (2)].args).f2,
                                             (int)(yysemantic_stack_[(2) - (2)].args).f3, (int)(yysemantic_stack_[(2) - (2)].args).f4); }
    break;

  case 45:

/* Line 677 of lalr1.cc  */
#line 155 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendExpandedStar((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                                     (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3); }
    break;

  case 46:

/* Line 677 of lalr1.cc  */
#line 157 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendDisc((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1, (int)(yysemantic_stack_[(2) - (2)].args).f2); }
    break;

  case 47:

/* Line 677 of lalr1.cc  */
#line 158 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleTriangle((yysemantic_stack_[(2) - (2)].args).f0, (int)(yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 48:

/* Line 677 of lalr1.cc  */
#line 159 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendSimpleQuad((yysemantic_stack_[(2) - (2)].args).f0, (int)(yysemantic_stack_[(2) - (2)].args).f1); }
    break;

  case 49:

/* Line 677 of lalr1.cc  */
#line 160 "easymesh/easymesh-parser.y"
    { mc.m_mesh.AppendCog((int)(yysemantic_stack_[(2) - (2)].args).f0, (yysemantic_stack_[(2) - (2)].args).f1,
                                 (yysemantic_stack_[(2) - (2)].args).f2 / 2, (yysemantic_stack_[(2) - (2)].args).f3 / 2, (yysemantic_stack_[(2) - (2)].args).f2, (yysemantic_stack_[(2) - (2)].args).f3, (yysemantic_stack_[(2) - (2)].args).f4,
                                 (yysemantic_stack_[(2) - (2)].args).f5, (yysemantic_stack_[(2) - (2)].args).f6, (int)(yysemantic_stack_[(2) - (2)].args).f7); }
    break;

  case 50:

/* Line 677 of lalr1.cc  */
#line 165 "easymesh/easymesh-parser.y"
    { (yyval.args).f0 = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 51:

/* Line 677 of lalr1.cc  */
#line 166 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f1 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 52:

/* Line 677 of lalr1.cc  */
#line 167 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f2 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 53:

/* Line 677 of lalr1.cc  */
#line 168 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f3 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 54:

/* Line 677 of lalr1.cc  */
#line 169 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f4 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 55:

/* Line 677 of lalr1.cc  */
#line 170 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f5 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 56:

/* Line 677 of lalr1.cc  */
#line 171 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f6 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 57:

/* Line 677 of lalr1.cc  */
#line 172 "easymesh/easymesh-parser.y"
    { (yyval.args) = (yysemantic_stack_[(2) - (1)].args); (yyval.args).f7 = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 58:

/* Line 677 of lalr1.cc  */
#line 175 "easymesh/easymesh-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 59:

/* Line 677 of lalr1.cc  */
#line 176 "easymesh/easymesh-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;



/* Line 677 of lalr1.cc  */
#line 789 "generated/easymesh-parser.cpp"
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
  const signed char EasyMeshParser::yypact_ninf_ = -10;
  const signed char
  EasyMeshParser::yypact_[] =
  {
        -3,    11,    87,     6,     6,     6,     6,   -10,     6,     6,
       6,     6,   -10,     6,     6,     6,     6,   -10,     6,     6,
     -10,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,   -10,    36,    43,    -3,    -3,
      73,   -10,   -10,   -10,   -10,   -10,   -10,     6,     6,     6,
       6,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
       6,     6,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,   -10,     6,     6,   -10,   -10,   -10,   -10,   -10,
      10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  EasyMeshParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,    34,     0,     0,     0,     0,    35,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     0,     0,     3,     0,
       5,     9,    11,    12,    13,    58,    15,     0,     0,     0,
       0,    14,    50,    17,    16,    19,    23,    26,    29,    20,
      24,    27,    30,    21,    25,    28,    31,    22,    32,    18,
       0,     0,    37,    38,    39,    40,    41,    42,    44,    45,
      46,    47,    48,     0,     0,    49,    43,     1,     2,     4,
       0,    10,    59,    51,    52,    53,    54,    55,    56,    57,
       8,     6
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  EasyMeshParser::yypgoto_[] =
  {
       -10,   -10,    -4,   -10,   -10,   -10,   -10,    14,   -10,   -10,
     -10,   107,    26,    37,    44,    31,    30,   -10,   -10,    -9
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  EasyMeshParser::yydefgoto_[] =
  {
        -1,    36,    37,    38,    39,   101,    40,    41,    42,    43,
      44,    48,    49,    50,    70,    71,    72,    84,    85,    52
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char EasyMeshParser::yytable_ninf_ = -1;
  const unsigned char
  EasyMeshParser::yytable_[] =
  {
         1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    89,    90,    87,    35,    92,    93,
      94,    95,    57,    88,    45,    51,    54,    61,    47,    45,
      46,   100,    65,    47,    91,    67,    68,    81,    82,    78,
      73,    96,    97,    83,    77,     0,     0,    80,    74,    75,
      76,    86,     0,    79,    98,    99,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      55,    56,     0,    58,     0,    59,    60,     0,    62,     0,
      63,    64,     0,    66,     0,    45,    53,     0,    69,    47
  };

  /* YYCHECK.  */
  const signed char
  EasyMeshParser::yycheck_[] =
  {
         3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    38,    39,     0,    40,    47,    48,
      49,    50,     5,     0,    38,     1,     2,    10,    42,    38,
      39,    41,    15,    42,    40,    18,    19,    31,    32,    28,
      23,    70,    71,    33,    27,    -1,    -1,    30,    24,    25,
      26,    34,    -1,    29,    83,    84,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
       3,     4,    -1,     6,    -1,     8,     9,    -1,    11,    -1,
      13,    14,    -1,    16,    -1,    38,    39,    -1,    21,    42
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  EasyMeshParser::yystos_[] =
  {
         0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    40,    44,    45,    46,    47,
      49,    50,    51,    52,    53,    38,    39,    42,    54,    55,
      56,    57,    62,    39,    57,    54,    54,    56,    54,    54,
      54,    56,    54,    54,    54,    56,    54,    56,    56,    54,
      57,    58,    59,    56,    57,    57,    57,    56,    58,    57,
      56,    55,    55,    59,    60,    61,    56,     0,     0,    45,
      45,    50,    62,    62,    62,    62,    62,    62,    62,    62,
      41,    48
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
      91,    93,    45
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  EasyMeshParser::yyr1_[] =
  {
         0,    43,    44,    45,    45,    46,    46,    47,    48,    49,
      49,    50,    50,    50,    51,    51,    51,    51,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    62
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  EasyMeshParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     3,     1,     1,     1,
       2,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     1,     2
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
  "T_SCALE", "T_TOGGLESCALEWINDING", "T_CHAMFER", "T_CYLINDER", "T_BOX",
  "T_SMOOTHCHAMFBOX", "T_FLATCHAMFBOX", "T_SPHERE", "T_CAPSULE", "T_STAR",
  "T_EXPANDEDSTAR", "T_DISC", "T_TRIANGLE", "T_QUAD", "T_COG", "T_TORUS",
  "T_ERROR", "NUMBER", "COLOR", "'['", "']'", "'-'", "$accept",
  "mesh_description", "mesh_expression_list", "mesh_expression",
  "mesh_open", "mesh_close", "mesh_command_list", "mesh_command",
  "color_command", "transform_command", "primitive_command", "args1",
  "args2", "args3", "args4", "args5", "args6", "args7", "args8", "number", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const EasyMeshParser::rhs_number_type
  EasyMeshParser::yyrhs_[] =
  {
        44,     0,    -1,    45,     0,    -1,    46,    -1,    46,    45,
      -1,    49,    -1,    47,    45,    48,    -1,    40,    -1,    41,
      -1,    50,    -1,    49,    50,    -1,    51,    -1,    52,    -1,
      53,    -1,     3,    57,    -1,     3,    39,    -1,     4,    57,
      -1,     4,    39,    -1,    23,    54,    -1,     5,    54,    -1,
      10,    54,    -1,    15,    54,    -1,    20,    56,    -1,     6,
      54,    -1,    11,    54,    -1,    16,    54,    -1,     7,    56,
      -1,    12,    56,    -1,    17,    56,    -1,     8,    54,    -1,
      13,    54,    -1,    18,    54,    -1,    21,    56,    -1,     9,
      -1,    14,    -1,    19,    -1,    22,    -1,    24,    59,    -1,
      25,    56,    -1,    26,    57,    -1,    27,    57,    -1,    28,
      57,    -1,    29,    56,    -1,    36,    56,    -1,    30,    58,
      -1,    31,    57,    -1,    32,    56,    -1,    33,    55,    -1,
      34,    55,    -1,    35,    61,    -1,    62,    -1,    54,    62,
      -1,    55,    62,    -1,    56,    62,    -1,    57,    62,    -1,
      58,    62,    -1,    59,    62,    -1,    60,    62,    -1,    38,
      -1,    42,    62,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  EasyMeshParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    17,    19,    21,
      23,    26,    28,    30,    32,    35,    38,    41,    44,    47,
      50,    53,    56,    59,    62,    65,    68,    71,    74,    77,
      80,    83,    86,    89,    91,    93,    95,    97,   100,   103,
     106,   109,   112,   115,   118,   121,   124,   127,   130,   133,
     136,   138,   141,   144,   147,   150,   153,   156,   159,   161
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  EasyMeshParser::yyrline_[] =
  {
         0,    75,    75,    79,    80,    84,    85,    89,    93,    97,
      98,   102,   103,   104,   108,   109,   112,   113,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   141,   144,   145,
     147,   149,   151,   152,   153,   155,   157,   158,   159,   160,
     165,   166,   167,   168,   169,   170,   171,   172,   175,   176
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
       2,     2,     2,     2,     2,    42,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    41,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int EasyMeshParser::yyeof_ = 0;
  const int EasyMeshParser::yylast_ = 129;
  const int EasyMeshParser::yynnts_ = 20;
  const int EasyMeshParser::yyempty_ = -2;
  const int EasyMeshParser::yyfinal_ = 87;
  const int EasyMeshParser::yyterror_ = 1;
  const int EasyMeshParser::yyerrcode_ = 256;
  const int EasyMeshParser::yyntokens_ = 43;

  const unsigned int EasyMeshParser::yyuser_token_number_max_ = 294;
  const EasyMeshParser::token_number_type EasyMeshParser::yyundef_token_ = 2;


} // lol

/* Line 1053 of lalr1.cc  */
#line 1303 "generated/easymesh-parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 179 "easymesh/easymesh-parser.y"


void lol::EasyMeshParser::error(const EasyMeshParser::location_type& l,
                                const std::string& m)
{
    mc.Error(l, m);
}


