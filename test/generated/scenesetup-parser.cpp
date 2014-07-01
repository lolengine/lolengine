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
#line 1 "scenesetup-parser.y"

//
// Lol Engine
//
// Copyright: (c) 2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/main.h>
#include "../scenesetup.h"



/* Line 310 of lalr1.cc  */
#line 63 "generated/scenesetup-parser.cpp"


#include "scenesetup-parser.h"

/* User implementation prologue.  */

/* Line 316 of lalr1.cc  */
#line 76 "scenesetup-parser.y"

#include "../scenesetup-compiler.h"

#undef yylex
#define yylex uc.m_lexer->lex

/* HACK: workaround for Bison who insists on using exceptions */
#define try if (true)
#define catch(...) if (false)
#define throw (void)0


/* Line 316 of lalr1.cc  */
#line 85 "generated/scenesetup-parser.cpp"

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
#line 151 "generated/scenesetup-parser.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  SceneSetupParser::yytnamerr_ (const char *yystr)
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
  SceneSetupParser::SceneSetupParser (class SceneSetupCompiler& uc_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      uc (uc_yyarg)
  {
  }

  SceneSetupParser::~SceneSetupParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  SceneSetupParser::yy_symbol_value_print_ (int yytype,
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
  SceneSetupParser::yy_symbol_print_ (int yytype,
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
  SceneSetupParser::yydestruct_ (const char* yymsg,
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
  SceneSetupParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  SceneSetupParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  SceneSetupParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  SceneSetupParser::debug_level_type
  SceneSetupParser::debug_level () const
  {
    return yydebug_;
  }

  void
  SceneSetupParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  SceneSetupParser::parse ()
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
	  case 12:

/* Line 677 of lalr1.cc  */
#line 116 "scenesetup-parser.y"
    { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT"; }
    break;

  case 13:

/* Line 677 of lalr1.cc  */
#line 117 "scenesetup-parser.y"
    { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT";
                                      uc.m_sstp.m_lights.Last()->SetType(LightType((yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 14:

/* Line 677 of lalr1.cc  */
#line 119 "scenesetup-parser.y"
    { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT";
                                      uc.m_sstp.m_lights.Last()->SetType(FindValue<LightType>((yysemantic_stack_[(2) - (2)].svval))); }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 124 "scenesetup-parser.y"
    { if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetPosition(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2])); }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 126 "scenesetup-parser.y"
    { if (uc.m_last_cmd == "ADDLIGHT")
                                        { /* */ } }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 128 "scenesetup-parser.y"
    { if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 130 "scenesetup-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                                      ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                                      vec4 vv = vec4(v) * (1.f / 255.f);
                                      if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetColor(vv); }
    break;

  case 19:

/* Line 677 of lalr1.cc  */
#line 138 "scenesetup-parser.y"
    { uc.m_sstp.m_clear_color = vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3]); }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 139 "scenesetup-parser.y"
    { uc.m_sstp.m_clear_color = vec4((yysemantic_stack_[(5) - (2)].vval)[0], (yysemantic_stack_[(5) - (2)].vval)[1], (yysemantic_stack_[(5) - (2)].vval)[2], 1.f); }
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 140 "scenesetup-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val); ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                                      uc.m_sstp.m_clear_color = vec4(v) * (1.f / 255.f); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 142 "scenesetup-parser.y"
    { uc.m_sstp.m_show_gizmo = (yysemantic_stack_[(2) - (2)].bval); }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 143 "scenesetup-parser.y"
    { uc.m_sstp.m_show_lights = (yysemantic_stack_[(2) - (2)].bval); }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 147 "scenesetup-parser.y"
    { uc.m_sstp.m_custom_cmd.Push((yysemantic_stack_[(3) - (2)].svval), (yysemantic_stack_[(3) - (3)].sval)); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 153 "scenesetup-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 154 "scenesetup-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 155 "scenesetup-parser.y"
    { (yyval.fval) = (float)(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 156 "scenesetup-parser.y"
    { (yyval.fval) = -(float)(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 160 "scenesetup-parser.y"
    { (yyval.ival) = (yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 161 "scenesetup-parser.y"
    { (yyval.ival) = -(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 162 "scenesetup-parser.y"
    { (yyval.ival) = (int)(yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 163 "scenesetup-parser.y"
    { (yyval.ival) = -(int)(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 168 "scenesetup-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 169 "scenesetup-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(5) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(5) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(5) - (4)].fval); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 178 "scenesetup-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[3] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 179 "scenesetup-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(6) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(6) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(6) - (4)].fval); (yyval.vval)[3] = (yysemantic_stack_[(6) - (5)].fval); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 184 "scenesetup-parser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 185 "scenesetup-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 186 "scenesetup-parser.y"
    { (yyval.bval) = !!(yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 190 "scenesetup-parser.y"
    { (yyval.svval) = (yysemantic_stack_[(1) - (1)].svval); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 194 "scenesetup-parser.y"
    { String t = (yysemantic_stack_[(1) - (1)].sval);
                      t.Replace('"', ' ', true);
                      free((yysemantic_stack_[(1) - (1)].sval));
                      (yyval.sval) = strdup((const char *)t.C()); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 198 "scenesetup-parser.y"
    { String t = (yysemantic_stack_[(2) - (1)].sval);
                      t += (yysemantic_stack_[(2) - (2)].sval);
                      t.Replace('"', ' ', true);
                      free((yysemantic_stack_[(2) - (1)].sval));
                      free((yysemantic_stack_[(2) - (2)].sval));
                      (yyval.sval) = strdup((const char *)t.C()); }
    break;



/* Line 677 of lalr1.cc  */
#line 677 "generated/scenesetup-parser.cpp"
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
  SceneSetupParser::yysyntax_error_ (int yystate, int tok)
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
  const signed char SceneSetupParser::yypact_ninf_ = -21;
  const signed char
  SceneSetupParser::yypact_[] =
  {
        38,     6,   -18,   -18,    -6,    -2,    37,    37,   -13,     7,
      21,    38,    38,   -21,   -21,   -21,   -21,   -21,   -21,   -21,
     -21,    17,   -21,   -21,    19,   -21,   -21,   -21,    19,   -21,
     -21,   -21,   -21,   -21,   -21,   -21,   -21,     8,   -21,   -21,
     -21,   -21,   -21,   -21,    17,   -21,   -21,    -8,    -4,    18,
       8,   -21,   -21,   -21,   -21,    19,   -21,    19,   -18,   -21,
      13,    19,   -21,   -21,    31,   -21
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  SceneSetupParser::yydefact_[] =
  {
         0,    12,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     3,     5,     6,     8,     9,    10,    11,    25,    27,
      40,     0,    13,    14,     0,    15,    16,    18,     0,    17,
      21,    19,    39,    38,    37,    22,    23,     0,     1,     2,
       4,     7,    25,    27,     0,    26,    28,     0,     0,     0,
      41,    24,    26,    28,    33,     0,    35,     0,     0,    42,
       0,     0,    20,    34,     0,    36
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  SceneSetupParser::yypgoto_[] =
  {
       -21,   -21,    28,   -21,   -21,    41,   -21,   -21,   -21,   -21,
     -21,   -20,   -15,    -3,    35,    49,    50,     9
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  SceneSetupParser::yydefgoto_[] =
  {
        -1,     9,    10,    11,    12,    13,    14,    15,    16,    49,
      17,    22,    46,    25,    29,    35,    23,    51
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char SceneSetupParser::yytable_ninf_ = -1;
  const unsigned char
  SceneSetupParser::yytable_[] =
  {
        26,    45,    24,    20,    47,    18,    19,    38,    48,    18,
      19,    21,    27,    54,    28,    21,    30,    56,    28,    18,
      19,    39,    20,    50,    52,    21,    58,    55,    57,    53,
      42,    43,    18,    19,    63,    60,    44,    61,    21,    40,
      31,    64,     1,     2,     3,     4,     5,     6,     7,     8,
      32,    33,    65,    41,    34,    62,    36,     0,    37,    59
  };

  /* YYCHECK.  */
  const signed char
  SceneSetupParser::yycheck_[] =
  {
         3,    21,    20,    16,    24,    13,    14,     0,    28,    13,
      14,    19,    18,    21,    20,    19,    18,    21,    20,    13,
      14,     0,    16,    15,    44,    19,     8,    47,    48,    44,
      13,    14,    13,    14,    21,    55,    19,    57,    19,    11,
       5,    61,     4,     5,     6,     7,     8,     9,    10,    11,
      13,    14,    21,    12,    17,    58,     7,    -1,     8,    50
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  SceneSetupParser::yystos_[] =
  {
         0,     4,     5,     6,     7,     8,     9,    10,    11,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    13,    14,
      16,    19,    33,    38,    20,    35,    35,    18,    20,    36,
      18,    36,    13,    14,    17,    37,    37,    38,     0,     0,
      24,    27,    13,    14,    19,    33,    34,    33,    33,    31,
      15,    39,    33,    34,    21,    33,    21,    33,     8,    39,
      33,    33,    35,    21,    33,    21
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  SceneSetupParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,    45,
      40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  SceneSetupParser::yyr1_[] =
  {
         0,    22,    23,    24,    24,    25,    26,    26,    27,    27,
      27,    27,    28,    28,    28,    29,    29,    29,    29,    31,
      30,    30,    30,    30,    32,    33,    33,    33,    33,    34,
      34,    34,    34,    35,    35,    36,    36,    37,    37,    37,
      38,    39,    39
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  SceneSetupParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     1,     2,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     0,
       5,     2,     2,     2,     3,     1,     2,     1,     2,     1,
       2,     1,     2,     3,     5,     3,     6,     1,     1,     1,
       1,     1,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const SceneSetupParser::yytname_[] =
  {
    "T_END", "error", "$undefined", "T_COLOR", "T_ADDLIGHT",
  "T_OBJPOSITION", "T_OBJLOOKAT", "T_OBJCOLOR", "T_CLEARCOLOR",
  "T_SHOWGIZMO", "T_SHOWLIGHT", "T_CUSTOMCMD", "T_ERROR", "F_NUMBER",
  "I_NUMBER", "STRING", "STRING_VAR", "BOOLEAN", "COLOR", "'-'", "'('",
  "')'", "$accept", "sstp_description", "sstp_expression_list",
  "sstp_expression", "sstp_command_list", "sstp_command", "light_command",
  "setup_command", "scene_command", "$@1", "custom_command", "fv", "iv",
  "v3", "v4", "bv", "svv", "sv", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const SceneSetupParser::rhs_number_type
  SceneSetupParser::yyrhs_[] =
  {
        23,     0,    -1,    24,     0,    -1,    25,    -1,    25,    24,
      -1,    26,    -1,    27,    -1,    26,    27,    -1,    28,    -1,
      29,    -1,    30,    -1,    32,    -1,     4,    -1,     4,    33,
      -1,     4,    38,    -1,     5,    35,    -1,     6,    35,    -1,
       7,    36,    -1,     7,    18,    -1,    -1,     8,    36,    31,
       8,    35,    -1,     8,    18,    -1,     9,    37,    -1,    10,
      37,    -1,    11,    38,    39,    -1,    13,    -1,    19,    33,
      -1,    14,    -1,    19,    34,    -1,    14,    -1,    19,    34,
      -1,    13,    -1,    19,    33,    -1,    20,    33,    21,    -1,
      20,    33,    33,    33,    21,    -1,    20,    33,    21,    -1,
      20,    33,    33,    33,    33,    21,    -1,    17,    -1,    14,
      -1,    13,    -1,    16,    -1,    15,    -1,    15,    39,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  SceneSetupParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    15,    18,    20,
      22,    24,    26,    28,    31,    34,    37,    40,    43,    46,
      47,    53,    56,    59,    62,    66,    68,    71,    73,    76,
      78,    81,    83,    86,    90,    96,   100,   107,   109,   111,
     113,   115,   117
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  SceneSetupParser::yyrline_[] =
  {
         0,    91,    91,    95,    96,   100,   104,   105,   109,   110,
     111,   112,   116,   117,   119,   124,   126,   128,   130,   138,
     138,   140,   142,   143,   147,   153,   154,   155,   156,   160,
     161,   162,   163,   168,   169,   178,   179,   184,   185,   186,
     190,   194,   198
  };

  // Print the state stack on the debug stream.
  void
  SceneSetupParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  SceneSetupParser::yy_reduce_print_ (int yyrule)
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
  SceneSetupParser::token_number_type
  SceneSetupParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      20,    21,     2,     2,     2,    19,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int SceneSetupParser::yyeof_ = 0;
  const int SceneSetupParser::yylast_ = 59;
  const int SceneSetupParser::yynnts_ = 18;
  const int SceneSetupParser::yyempty_ = -2;
  const int SceneSetupParser::yyfinal_ = 38;
  const int SceneSetupParser::yyterror_ = 1;
  const int SceneSetupParser::yyerrcode_ = 256;
  const int SceneSetupParser::yyntokens_ = 22;

  const unsigned int SceneSetupParser::yyuser_token_number_max_ = 273;
  const SceneSetupParser::token_number_type SceneSetupParser::yyundef_token_ = 2;


} // lol

/* Line 1053 of lalr1.cc  */
#line 1148 "generated/scenesetup-parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 206 "scenesetup-parser.y"


void lol::SceneSetupParser::error(const SceneSetupParser::location_type& l,
                                const std::string& m)
{
    uc.Error(l, m);
}


