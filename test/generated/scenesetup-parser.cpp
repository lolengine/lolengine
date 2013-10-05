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

#include "core.h"
#include "../scenesetup.h"



/* Line 310 of lalr1.cc  */
#line 63 "generated/scenesetup-parser.cpp"


#include "scenesetup-parser.h"

/* User implementation prologue.  */

/* Line 316 of lalr1.cc  */
#line 75 "scenesetup-parser.y"

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
	  case 11:

/* Line 677 of lalr1.cc  */
#line 114 "scenesetup-parser.y"
    { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT"; }
    break;

  case 12:

/* Line 677 of lalr1.cc  */
#line 115 "scenesetup-parser.y"
    { uc.m_sstp.m_lights << new Light(); uc.m_last_cmd = "ADDLIGHT";
                                      uc.m_sstp.m_lights.Last()->SetPosition(vec4(vec3::zero, (yysemantic_stack_[(2) - (2)].fval))); }
    break;

  case 13:

/* Line 677 of lalr1.cc  */
#line 120 "scenesetup-parser.y"
    { if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetPosition(vec4(vec3((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2]), uc.m_sstp.m_lights.Last()->GetPosition().w)); }
    break;

  case 14:

/* Line 677 of lalr1.cc  */
#line 122 "scenesetup-parser.y"
    { if (uc.m_last_cmd == "ADDLIGHT")
                                        { /* */ } }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 124 "scenesetup-parser.y"
    { if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetColor(vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3])); }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 126 "scenesetup-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val);
                                      ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                                      vec4 vv = vec4(v) * (1.f / 255.f);
                                      if (uc.m_last_cmd == "ADDLIGHT")
                                        uc.m_sstp.m_lights.Last()->SetColor(vv); }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 131 "scenesetup-parser.y"
    { uc.m_sstp.m_clear_color = vec4((yysemantic_stack_[(2) - (2)].vval)[0], (yysemantic_stack_[(2) - (2)].vval)[1], (yysemantic_stack_[(2) - (2)].vval)[2], (yysemantic_stack_[(2) - (2)].vval)[3]); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 132 "scenesetup-parser.y"
    { uint32_t x = (yysemantic_stack_[(2) - (2)].u32val); ivec4 v(x >> 24, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
                                      uc.m_sstp.m_clear_color = vec4(v) * (1.f / 255.f); }
    break;

  case 19:

/* Line 677 of lalr1.cc  */
#line 137 "scenesetup-parser.y"
    { uc.m_sstp.m_custom_cmd.Push((yysemantic_stack_[(3) - (2)].svval), (yysemantic_stack_[(3) - (3)].sval)); }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 143 "scenesetup-parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 144 "scenesetup-parser.y"
    { (yyval.fval) = -(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 145 "scenesetup-parser.y"
    { (yyval.fval) = (float)(yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 146 "scenesetup-parser.y"
    { (yyval.fval) = -(float)(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 150 "scenesetup-parser.y"
    { (yyval.ival) = (yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 151 "scenesetup-parser.y"
    { (yyval.ival) = -(yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 152 "scenesetup-parser.y"
    { (yyval.ival) = (int)(yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 153 "scenesetup-parser.y"
    { (yyval.ival) = -(int)(yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 158 "scenesetup-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 159 "scenesetup-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(5) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(5) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(5) - (4)].fval); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 168 "scenesetup-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[2] = (yysemantic_stack_[(3) - (2)].fval); (yyval.vval)[3] = (yysemantic_stack_[(3) - (2)].fval); }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 169 "scenesetup-parser.y"
    { (yyval.vval)[0] = (yysemantic_stack_[(6) - (2)].fval); (yyval.vval)[1] = (yysemantic_stack_[(6) - (3)].fval); (yyval.vval)[2] = (yysemantic_stack_[(6) - (4)].fval); (yyval.vval)[3] = (yysemantic_stack_[(6) - (5)].fval); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 180 "scenesetup-parser.y"
    { (yyval.svval) = (yysemantic_stack_[(1) - (1)].svval); }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 184 "scenesetup-parser.y"
    { String t = (yysemantic_stack_[(1) - (1)].sval);
                      t.Replace('"', ' ', true);
                      free((yysemantic_stack_[(1) - (1)].sval));
                      (yyval.sval) = strdup((const char *)t.C()); }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 188 "scenesetup-parser.y"
    { String t = (yysemantic_stack_[(2) - (1)].sval);
                      t += (yysemantic_stack_[(2) - (2)].sval);
                      t.Replace('"', ' ', true);
                      free((yysemantic_stack_[(2) - (1)].sval));
                      free((yysemantic_stack_[(2) - (2)].sval));
                      (yyval.sval) = strdup((const char *)t.C()); }
    break;



/* Line 677 of lalr1.cc  */
#line 627 "generated/scenesetup-parser.cpp"
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
  const signed char SceneSetupParser::yypact_ninf_ = -18;
  const signed char
  SceneSetupParser::yypact_[] =
  {
        30,     8,   -13,   -13,    -6,    -2,     3,     1,    26,    30,
      30,   -18,   -18,   -18,   -18,   -18,   -18,    12,   -18,     8,
     -18,   -18,   -18,     8,   -18,   -18,   -18,   -18,    14,   -18,
     -18,   -18,   -18,   -18,   -18,    12,   -18,   -18,    -8,    -4,
      14,   -18,   -18,   -18,   -18,     8,   -18,     8,   -18,    13,
       8,   -18,    21,   -18
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  SceneSetupParser::yydefact_[] =
  {
         0,    11,     0,     0,     0,     0,     0,     0,     0,     3,
       5,     6,     8,     9,    10,    20,    22,     0,    12,     0,
      13,    14,    16,     0,    15,    18,    17,    32,     0,     1,
       2,     4,     7,    20,    22,     0,    21,    23,     0,     0,
      33,    19,    21,    23,    28,     0,    30,     0,    34,     0,
       0,    29,     0,    31
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  SceneSetupParser::yypgoto_[] =
  {
       -18,   -18,    22,   -18,   -18,    31,   -18,   -18,   -18,   -17,
       7,    40,    39,   -18,     5
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  SceneSetupParser::yydefgoto_[] =
  {
        -1,     7,     8,     9,    10,    11,    12,    13,    14,    18,
      37,    20,    24,    28,    41
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char SceneSetupParser::yytable_ninf_ = -1;
  const unsigned char
  SceneSetupParser::yytable_[] =
  {
        36,    29,    38,    15,    16,    19,    39,    15,    16,    17,
      22,    44,    23,    17,    25,    46,    23,    27,    42,    15,
      16,    45,    47,    33,    34,    17,    30,    40,    49,    35,
      50,    31,    51,    52,     1,     2,     3,     4,     5,     6,
      53,    32,    43,    21,    26,    48
  };

  /* YYCHECK.  */
  const unsigned char
  SceneSetupParser::yycheck_[] =
  {
        17,     0,    19,    11,    12,    18,    23,    11,    12,    17,
      16,    19,    18,    17,    16,    19,    18,    14,    35,    11,
      12,    38,    39,    11,    12,    17,     0,    13,    45,    17,
      47,     9,    19,    50,     4,     5,     6,     7,     8,     9,
      19,    10,    35,     3,     5,    40
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  SceneSetupParser::yystos_[] =
  {
         0,     4,     5,     6,     7,     8,     9,    21,    22,    23,
      24,    25,    26,    27,    28,    11,    12,    17,    29,    18,
      31,    31,    16,    18,    32,    16,    32,    14,    33,     0,
       0,    22,    25,    11,    12,    17,    29,    30,    29,    29,
      13,    34,    29,    30,    19,    29,    19,    29,    34,    29,
      29,    19,    29,    19
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  SceneSetupParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,    45,    40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  SceneSetupParser::yyr1_[] =
  {
         0,    20,    21,    22,    22,    23,    24,    24,    25,    25,
      25,    26,    26,    27,    27,    27,    27,    27,    27,    28,
      29,    29,    29,    29,    30,    30,    30,    30,    31,    31,
      32,    32,    33,    34,    34
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  SceneSetupParser::yyr2_[] =
  {
         0,     2,     2,     1,     2,     1,     1,     2,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     3,
       1,     2,     1,     2,     1,     2,     1,     2,     3,     5,
       3,     6,     1,     1,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const SceneSetupParser::yytname_[] =
  {
    "T_END", "error", "$undefined", "T_COLOR", "T_ADDLIGHT",
  "T_OBJPOSITION", "T_OBJLOOKAT", "T_OBJCOLOR", "T_CLEARCOLOR",
  "T_CUSTOMCMD", "T_ERROR", "F_NUMBER", "I_NUMBER", "STRING", "STRING_VAR",
  "BOOLEAN", "COLOR", "'-'", "'('", "')'", "$accept", "sstp_description",
  "sstp_expression_list", "sstp_expression", "sstp_command_list",
  "sstp_command", "light_command", "setup_command", "custom_command", "fv",
  "iv", "v3", "v4", "svv", "sv", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const SceneSetupParser::rhs_number_type
  SceneSetupParser::yyrhs_[] =
  {
        21,     0,    -1,    22,     0,    -1,    23,    -1,    23,    22,
      -1,    24,    -1,    25,    -1,    24,    25,    -1,    26,    -1,
      27,    -1,    28,    -1,     4,    -1,     4,    29,    -1,     5,
      31,    -1,     6,    31,    -1,     7,    32,    -1,     7,    16,
      -1,     8,    32,    -1,     8,    16,    -1,     9,    33,    34,
      -1,    11,    -1,    17,    29,    -1,    12,    -1,    17,    30,
      -1,    12,    -1,    17,    30,    -1,    11,    -1,    17,    29,
      -1,    18,    29,    19,    -1,    18,    29,    29,    29,    19,
      -1,    18,    29,    19,    -1,    18,    29,    29,    29,    29,
      19,    -1,    14,    -1,    13,    -1,    13,    34,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  SceneSetupParser::yyprhs_[] =
  {
         0,     0,     3,     6,     8,    11,    13,    15,    18,    20,
      22,    24,    26,    29,    32,    35,    38,    41,    44,    47,
      51,    53,    56,    58,    61,    63,    66,    68,    71,    75,
      81,    85,    92,    94,    96
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  SceneSetupParser::yyrline_[] =
  {
         0,    90,    90,    94,    95,    99,   103,   104,   108,   109,
     110,   114,   115,   120,   122,   124,   126,   131,   132,   137,
     143,   144,   145,   146,   150,   151,   152,   153,   158,   159,
     168,   169,   180,   184,   188
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
      18,    19,     2,     2,     2,    17,     2,     2,     2,     2,
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
      15,    16
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int SceneSetupParser::yyeof_ = 0;
  const int SceneSetupParser::yylast_ = 45;
  const int SceneSetupParser::yynnts_ = 15;
  const int SceneSetupParser::yyempty_ = -2;
  const int SceneSetupParser::yyfinal_ = 29;
  const int SceneSetupParser::yyterror_ = 1;
  const int SceneSetupParser::yyerrcode_ = 256;
  const int SceneSetupParser::yyntokens_ = 20;

  const unsigned int SceneSetupParser::yyuser_token_number_max_ = 271;
  const SceneSetupParser::token_number_type SceneSetupParser::yyundef_token_ = 2;


} // lol

/* Line 1053 of lalr1.cc  */
#line 1085 "generated/scenesetup-parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 196 "scenesetup-parser.y"


void lol::SceneSetupParser::error(const SceneSetupParser::location_type& l,
                                const std::string& m)
{
    uc.Error(l, m);
}


