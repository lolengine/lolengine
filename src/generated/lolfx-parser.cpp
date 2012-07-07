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
#line 1 "gpu/lolfx-parser.y"

//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"

#include <string>



/* Line 293 of lalr1.cc  */
#line 63 "generated/lolfx-parser.cpp"


#include "lolfx-parser.h"

/* User implementation prologue.  */

/* Line 299 of lalr1.cc  */
#line 243 "gpu/lolfx-parser.y"

#include "gpu/lolfx-compiler.h"

#undef yylex
#define yylex mc.m_lexer->lex


/* Line 299 of lalr1.cc  */
#line 80 "generated/lolfx-parser.cpp"

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
#line 166 "generated/lolfx-parser.cpp"

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  LolFxParser::yytnamerr_ (const char *yystr)
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
  LolFxParser::LolFxParser (class LolFxCompiler& mc_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      mc (mc_yyarg)
  {
  }

  LolFxParser::~LolFxParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  LolFxParser::yy_symbol_value_print_ (int yytype,
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
  LolFxParser::yy_symbol_print_ (int yytype,
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
  LolFxParser::yydestruct_ (const char* yymsg,
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
  LolFxParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  LolFxParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  LolFxParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  LolFxParser::debug_level_type
  LolFxParser::debug_level () const
  {
    return yydebug_;
  }

  void
  LolFxParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  LolFxParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  LolFxParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  LolFxParser::parse ()
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
#line 729 "gpu/lolfx-parser.y"
    { std::cout << "New tech " << (yysemantic_stack_[(5) - (2)].sval) << std::endl; }
    break;

  case 10:

/* Line 690 of lalr1.cc  */
#line 742 "gpu/lolfx-parser.y"
    { std::cout << "New pass " << (yysemantic_stack_[(5) - (2)].sval) << std::endl; }
    break;

  case 29:

/* Line 690 of lalr1.cc  */
#line 778 "gpu/lolfx-parser.y"
    { std::cout << "new shader" << std::endl; }
    break;



/* Line 690 of lalr1.cc  */
#line 488 "generated/lolfx-parser.cpp"
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
  LolFxParser::yysyntax_error_ (int yystate, int yytoken)
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
  const short int LolFxParser::yypact_ninf_ = -432;
  const short int
  LolFxParser::yypact_[] =
  {
      -216,   132,   -76,   143,  -432,  -216,  -432,  -432,  4397,  -281,
    -432,  -432,  -276,  -432,  -432,  -432,   107,   115,  -432,  -432,
    -248,   191,  -432,  -432,  -432,   158,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,   120,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,    -3,  -432,  -432,  -432,  -432,
    -426,  -241,  -229,   184,  -431,  -432,   217,   336,   279,   279,
    3716,  -432,  -432,  -432,  -214,  1052,  -432,  4397,  -432,  -432,
      42,   231,  -432,  -432,  -432,  -432,   237,  -432,  1052,  -432,
    -432,  -157,  4555,   495,  -432,  -432,  -432,   131,  -432,  -432,
    -432,  -432,  -432,  -432,   931,   150,  -432,   248,  -192,   279,
    -432,  -432,  -432,  -152,  -432,  -432,  2012,  -432,  -432,   276,
    -147,    42,  -144,  -151,  -145,  -128,  -186,  -432,  -130,  4555,
    -432,   931,   343,  3968,  -432,  -432,  -432,  -432,  -432,  -432,
     -83,   -78,  3402,   -77,   -96,   -73,  1316,   -71,   -70,  2135,
     -69,   -67,  3402,  3402,   619,  -432,  -432,  3402,  -432,  -432,
    -432,  -432,  -432,  -432,  -252,  -432,  -432,  -432,   -74,  -167,
    3525,   -66,   -13,  3402,  -182,  -166,  -131,  -262,  -113,   -85,
     -90,   -75,   -40,   -38,  -297,  -432,  -432,  -243,  -432,   -57,
     -58,  -432,  -432,  -432,  -432,   743,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -432,   379,   931,  -135,  3402,  2584,
    -432,  -432,  4555,  -432,  -432,  -432,   -55,  -432,   -47,  -432,
    -432,   384,  -432,   231,   391,  -432,   237,  -432,  4136,   343,
     -45,  -238,  -432,  -432,  -432,  -432,  -311,  -432,  -432,  -432,
     272,    13,  3402,  -432,  -227,  3402,  1889,  -432,  -432,  -432,
    1192,  -137,  -432,  -432,  3402,  1630,  -432,  -432,  3402,   -46,
    -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,  3402,  -432,  3402,  3402,  3402,  3402,  3402,
    3402,  3402,  3402,  3402,  3402,  3402,  3402,  3402,  3402,  3402,
    3402,  3402,  3402,  3402,  3402,  -432,  3402,  -432,  -432,   -42,
    -432,  -432,  3402,  2707,  -432,  -432,   -39,   -37,  4253,  -432,
      -7,   398,  -432,  -432,  -432,  -432,  -219,  2830,  -432,   343,
    -432,   -36,  -432,  -432,  1889,  -134,  -432,  -107,   -41,   399,
     931,   -24,  -432,  -432,   -20,   -41,   -19,  -432,  -432,  -432,
    -432,  -432,  -432,  -432,  -182,  -182,  -166,  -166,  -131,  -131,
    -131,  -131,  -262,  -262,  -113,   -85,   -90,   -75,   -40,   -38,
    -287,  -432,  3402,  -432,   -17,   -14,  3402,   -11,    -6,  -101,
     -10,    -2,     0,     1,     4,     8,  -432,    -1,    -7,  -432,
    -432,  -432,     9,  -432,  3402,  -432,    15,    11,  1316,     6,
      17,  1440,  -432,  3402,    16,  3402,    24,  -432,  3402,  -256,
    -432,     3,   441,   446,   460,   462,   463,   468,   465,  -432,
    -432,  -432,  -102,  3402,  1440,   367,  -432,  1316,  3402,  -432,
    -432,  -432,  -432,  -432,  -432,  3402,  -432,  -432,  3279,    35,
      36,    37,    38,    39,    40,    41,    44,    45,    46,    47,
      48,    49,   -41,  -432,  1316,  1316,    43,  -432,  -432,    51,
      50,  -432,  -432,  -432,  -432,  -432,    53,  -432,  -432,  -432,
    -432,    54,  -432,  -432,  -432,  -432,  -432,    52,   229,    -4,
    -432,    65,   162,   167,   286,  -432,  -432,  -432,  -432
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned short int
  LolFxParser::yydefact_[] =
  {
         0,     0,     0,     0,     2,     3,     5,     6,     0,     0,
      31,    30,     0,     1,     4,   282,     0,     0,   164,   163,
       0,   162,   283,   284,   285,     0,   190,   202,   203,   204,
     208,   209,   210,   205,   206,   207,   211,   212,   213,   214,
     215,   216,   217,   220,   221,   222,   218,   223,   224,   225,
     219,   226,   227,   228,   229,   232,   233,   234,   230,   235,
     236,   237,   231,   238,   239,   240,   245,   248,   250,   246,
     249,   251,   268,   269,   275,   278,   247,   252,   253,   272,
     254,   258,   255,   259,   270,   276,   279,   256,   257,   260,
     273,   261,   265,   262,   266,   271,   277,   280,   263,   264,
     267,   274,   201,     0,   180,   198,   197,   181,   199,   165,
     182,   241,   242,   243,   244,     0,   200,   189,   196,   345,
       0,     0,   129,   139,     0,   145,   152,   179,   176,   173,
       0,   172,   160,   191,   193,     0,   281,    28,   342,   344,
       0,     0,   185,   186,   187,   188,     0,   159,     0,   183,
     184,     0,     0,     0,   119,   346,   127,   139,   143,   171,
     140,   142,   141,   130,     0,   139,   120,     0,   153,     0,
     177,   175,   174,     0,   126,   161,     0,   192,   343,     0,
       0,     8,     0,     0,    33,   169,     0,   167,     0,     0,
     162,     0,     0,     0,   288,    41,    38,    39,    40,    62,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   312,   316,     0,    67,    68,
      69,    70,    37,    43,    63,    45,    50,    51,     0,     0,
      56,     0,    71,     0,    75,    78,    81,    86,    89,    91,
      93,    95,    97,    99,   101,   103,   116,     0,   298,     0,
     160,   301,   314,   300,   299,     0,   302,   303,   304,   305,
     306,   307,   131,   136,   138,   144,     0,   146,     0,     0,
     132,   178,     0,   194,    71,   118,     0,    60,     0,     7,
       9,     0,    29,     0,     0,   166,     0,   121,     0,     0,
     294,     0,   292,   287,   289,   338,     0,   337,   327,   341,
       0,     0,     0,   339,     0,     0,     0,    64,    65,   308,
       0,     0,    47,    48,     0,     0,    54,    53,     0,   196,
      57,    59,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   105,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,   313,   315,   133,
     135,   137,     0,     0,   297,   158,   154,     0,     0,   195,
       0,    35,    32,   170,   168,   286,     0,     0,   290,     0,
     326,     0,   332,   331,   334,     0,   340,     0,   321,     0,
       0,     0,   309,    42,     0,    49,    46,    61,    52,    58,
     104,    72,    73,    74,    76,    77,    79,    80,    84,    85,
      82,    83,    87,    88,    90,    92,    94,    96,    98,   100,
       0,   117,     0,   151,   147,     0,     0,   155,     0,     0,
       0,     0,     0,     0,     0,     0,    13,     0,    11,    34,
     291,   295,     0,   293,     0,   333,     0,     0,     0,     0,
       0,     0,    44,     0,     0,     0,   148,   156,     0,     0,
     122,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      12,   296,     0,   335,     0,   320,   318,   324,     0,   311,
     328,   310,   102,   134,   149,     0,   157,   123,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   336,   330,     0,   325,     0,   322,   150,     0,
       0,    16,    15,    14,    17,    18,     0,    23,    24,    25,
      26,     0,    27,   329,   319,   323,   124,     0,     0,     0,
     125,     0,     0,     0,     0,    20,    19,    21,    22
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  LolFxParser::yypgoto_[] =
  {
      -432,  -432,   475,  -432,  -432,   306,  -432,   293,  -432,  -432,
    -432,  -432,  -432,   470,  -432,  -432,  -432,  -432,  -432,  -432,
     535,  -432,  -432,  -432,  -432,   -21,  -432,     7,    14,   -68,
      12,   507,   521,   522,   520,   523,   524,  -432,  -153,   -89,
    -432,   -51,  -154,     2,     5,  -432,  -432,  -432,   607,   717,
     711,   612,  -432,  -432,  -183,  -432,   752,  -432,  -432,   594,
    -432,    64,    19,    -8,    32,  -432,   856,  -432,   -72,   -81,
     593,   504,  -244,   583,   -95,  -302,  -432,   411,   766,  -211,
     586,  -432,  -432,   505,  -432,  -432,  -432,  -432,  -432,  -432,
    -432,  -432,  -432,   751,  -432
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  LolFxParser::yydefgoto_[] =
  {
        -1,     3,     4,     5,     6,   180,   181,   437,   438,     7,
       8,    12,   183,   184,   222,   223,   224,   394,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     333,   247,   276,   248,   249,   121,   122,   123,   263,   163,
     164,   264,   124,   125,   126,   127,   128,   129,   186,   187,
     165,   130,   131,   277,   133,   134,   135,   136,   193,   194,
     291,   292,   365,   251,   252,   253,   254,   480,   481,   255,
     256,   257,   476,   391,   258,   506,   259,   260,   384,   446,
     447,   261,   137,   138,   139
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const short int LolFxParser::yytable_ninf_ = -129;
  const short int
  LolFxParser::yytable_[] =
  {
       132,   429,   459,   310,   533,   151,   489,   490,     1,   166,
     119,   491,   153,   120,   154,   167,   195,   196,   197,   198,
     199,   200,    15,   275,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   300,   294,   103,   104,   367,   275,   288,   423,   105,
      10,   106,   175,   389,   107,   353,   108,   109,   110,   132,
     111,   112,   113,   114,   115,   356,   116,   117,   118,   119,
       9,   320,   120,    13,   192,   250,   170,   171,   172,   479,
     380,   296,   158,   342,   343,   274,   265,   140,   304,   356,
     358,   312,   313,   354,     2,   430,   311,   177,   141,    11,
     431,   158,   479,   432,   453,    22,    23,    24,   492,   364,
     188,   192,   457,   289,   487,   192,   488,   534,   271,   433,
     314,   307,   308,   344,   345,   315,   146,   355,   250,   147,
     368,   389,   378,   356,   156,   158,   250,   294,   379,   425,
     275,   484,   334,   386,   486,   358,   191,   157,   142,   356,
     434,   440,   143,   442,   275,   168,   144,   379,   176,   399,
     145,   149,   159,   531,   507,   150,   179,   532,   435,   182,
     493,   508,   160,   161,   400,   185,   162,   250,   274,   268,
     269,   385,   270,   191,   387,   388,   267,   191,   265,   285,
     286,   160,   161,   395,   192,   162,   505,   421,   454,   275,
     335,   336,   337,   364,   408,   409,   410,   411,   317,   318,
     192,   189,   338,   339,   278,   159,   272,   294,   340,   341,
      16,    17,   279,   250,   282,   160,   161,   281,   250,   162,
      26,   283,   250,   420,   346,   347,   362,   363,   393,   356,
     287,   448,   356,   284,   401,   402,   403,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   388,   510,   275,   191,   364,   449,   356,
     461,   462,   274,   501,   356,   404,   405,    16,    17,    18,
      19,   290,   191,   475,   406,   407,   274,    26,   412,   413,
     192,   -61,   295,   297,   482,   298,   364,   299,   349,   364,
     390,   316,   348,   301,   302,   305,   250,   306,   321,   103,
     104,   351,   175,   154,   350,   352,   -60,   359,   369,   364,
     107,   370,   371,   472,   110,   373,   364,   377,   381,   -55,
     422,   274,   426,   117,   439,   356,   427,   450,   444,   524,
     358,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   451,   502,   452,   455,   -62,   212,   213,   332,   456,
     458,   463,   191,   436,   460,   152,   103,   104,   469,   464,
     250,   465,   466,   250,   477,   494,   467,   107,   390,   468,
     109,   110,   471,   216,   495,   473,   474,   217,   478,   483,
     117,   218,   219,   220,   221,   485,   250,   274,   496,   250,
     497,   498,   499,   500,   504,   511,   512,   513,   514,   515,
      14,   517,   525,   516,   518,   519,   520,   280,   522,   523,
     521,   526,   530,   527,   528,   529,   250,   250,   195,   196,
     197,   198,   199,   200,    15,   535,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   201,   202,   103,   104,   203,   204,   205,
     206,   105,   536,   106,   207,   208,   107,   537,   108,   109,
     110,   209,   111,   112,   113,   114,   115,   210,   116,   117,
     118,   211,   195,   196,   197,   198,   199,   200,    15,  -128,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   201,   202,   103,
     104,   203,   204,   205,   206,   105,   538,   106,   207,   208,
     107,   470,   108,   109,   110,   209,   111,   112,   113,   114,
     115,   210,   116,   117,   118,   211,   195,   196,   197,   198,
     199,   200,    15,   372,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   201,   202,   103,   104,   203,   204,   205,   206,   105,
     398,   106,   207,   208,   107,   414,   108,   109,   110,   209,
     111,   112,   113,   114,   115,   210,   116,   117,   118,   211,
     415,   417,   416,   360,   262,   418,   266,   419,   361,   169,
     374,   148,   376,   443,   382,   503,   155,   383,   178,   445,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   214,   215,   216,     0,     0,     0,   217,
      15,     0,     0,   218,   219,   220,   221,     0,    22,    23,
      24,     0,     0,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,     0,
       0,     0,   212,   213,     0,     0,     0,   105,     0,   106,
       0,     0,     0,     0,   108,     0,     0,     0,   111,   112,
     113,   114,   115,     0,   116,     0,   118,   214,   309,   216,
       0,    15,     0,   217,     0,     0,     0,   218,   219,   220,
     221,     0,     0,     0,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
       0,     0,     0,     0,     0,     0,   212,   213,   105,     0,
     106,     0,     0,     0,     0,   108,     0,     0,     0,   111,
     112,   113,   114,   115,     0,   116,     0,   118,     0,     0,
       0,   214,   357,   216,     0,     0,     0,   217,     0,     0,
       0,   218,   219,   220,   221,   195,   196,   197,   198,   199,
     200,    15,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     201,   202,   103,   104,   203,   204,   205,   206,   105,     0,
     106,   207,   208,   107,     0,   108,   109,   110,   209,   111,
     112,   113,   114,   115,   210,   116,   117,   118,   211,   195,
     196,   197,   198,   199,   200,    15,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   201,   202,   103,   104,   203,   204,
     205,   206,   105,     0,   106,   207,   208,   107,     0,   108,
     109,   110,   209,   111,   112,   113,   114,   115,   210,   116,
     117,   118,   211,   195,   196,   197,   198,   199,   200,    15,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   201,   202,
     103,   104,   203,   204,   205,   206,   105,     0,   106,   207,
     208,   107,     0,   108,   109,   110,   209,   111,   112,   113,
     114,   115,   210,   116,   117,   118,   211,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     214,   392,   216,     0,     0,     0,   217,   396,   397,    15,
     218,   219,   220,   221,     0,     0,     0,    22,    23,    24,
       0,     0,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,     0,   212,
     213,     0,     0,     0,     0,     0,   105,     0,   106,     0,
       0,     0,     0,   108,     0,     0,     0,   111,   112,   113,
     114,   115,     0,   116,   214,   118,   216,     0,     0,     0,
     217,     0,     0,     0,   218,   219,   220,   221,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
     216,     0,     0,     0,   217,     0,     0,     0,   218,   219,
     220,   221,   195,   196,   197,   198,   199,   200,    15,     0,
      16,    17,    18,    19,    20,   190,    22,    23,    24,     0,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,     0,     0,   103,
     104,     0,     0,     0,     0,   105,     0,   106,     0,     0,
     107,     0,   108,   109,   110,     0,   111,   112,   113,   114,
     115,     0,   116,   117,   118,   195,   196,   197,   198,   199,
     200,    15,     0,     0,     0,     0,     0,     0,     0,    22,
      23,    24,     0,     0,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
       0,     0,     0,     0,     0,     0,     0,     0,   105,     0,
     106,     0,     0,     0,     0,   108,     0,     0,     0,   111,
     112,   113,   114,   115,     0,   116,     0,   118,   195,   196,
     197,   198,   199,   200,    15,     0,     0,     0,     0,     0,
       0,     0,    22,    23,    24,     0,     0,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,     0,     0,     0,     0,     0,     0,
       0,   105,     0,   106,     0,     0,     0,     0,   108,     0,
       0,     0,   111,   112,   113,   114,   115,     0,   116,     0,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,     0,     0,   218,   219,   220,
     221,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,   217,     0,     0,     0,
     218,   219,   220,   221,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,     0,   217,
       0,     0,     0,   218,   219,   220,   221,   195,   196,   197,
     198,   199,   200,    15,     0,     0,     0,     0,     0,     0,
       0,    22,    23,    24,     0,     0,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,     0,     0,     0,     0,     0,     0,     0,     0,
     105,     0,   106,     0,     0,     0,     0,   108,     0,     0,
       0,   111,   112,   113,   114,   115,     0,   116,     0,   118,
     195,   196,   197,   198,   199,   200,    15,     0,     0,     0,
       0,     0,     0,     0,    22,    23,    24,     0,     0,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,   106,     0,     0,     0,     0,
     108,     0,     0,     0,   111,   112,   113,   114,   115,     0,
     116,     0,   118,   195,   196,   197,   198,   199,   200,    15,
       0,     0,     0,     0,     0,     0,     0,    22,    23,    24,
       0,     0,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,     0,     0,
       0,     0,     0,     0,     0,     0,   105,     0,   106,     0,
       0,     0,     0,   108,     0,     0,     0,   111,   112,   113,
     114,   115,     0,   116,     0,   118,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   217,     0,
       0,     0,   218,   219,   220,   221,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     424,   217,     0,     0,     0,   218,   219,   220,   221,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   217,     0,     0,     0,   218,   219,
     220,   221,   195,   196,   197,   198,   199,   200,    15,     0,
       0,     0,     0,     0,     0,     0,    22,    23,    24,     0,
       0,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,     0,     0,     0,
       0,     0,     0,     0,     0,   105,     0,   106,     0,     0,
       0,     0,   108,     0,     0,     0,   111,   112,   113,   114,
     115,     0,   116,     0,   118,   195,   196,   197,   198,   199,
     200,    15,     0,     0,     0,     0,     0,     0,     0,    22,
      23,    24,     0,     0,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
       0,     0,     0,     0,     0,     0,     0,     0,   105,     0,
     106,     0,     0,     0,     0,   108,     0,     0,     0,   111,
     112,   113,   114,   115,     0,   116,     0,   118,   195,   196,
     197,   198,   199,   200,    15,     0,     0,     0,     0,     0,
       0,     0,    22,    23,    24,     0,     0,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,     0,     0,     0,     0,     0,     0,
       0,   105,     0,   106,     0,     0,     0,     0,   108,     0,
       0,     0,   111,   112,   113,   114,   115,     0,   116,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,   217,   173,    15,     0,   218,   219,   220,
     221,     0,     0,    22,    23,    24,     0,     0,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,     0,   212,   213,     0,     0,     0,
       0,     0,   105,     0,   106,     0,     0,     0,     0,   108,
       0,     0,     0,   111,   112,   113,   114,   115,     0,   116,
       0,   118,     0,     0,     0,     0,   217,     0,     0,     0,
     218,   219,   220,   221,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   217,
       0,     0,     0,   218,   219,   220,   221,    15,     0,    16,
      17,    18,    19,    20,   190,    22,    23,    24,     0,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,     0,     0,   103,   104,
       0,     0,     0,     0,   105,     0,   106,     0,     0,   107,
       0,   108,   109,   110,     0,   111,   112,   113,   114,   115,
       0,   116,   117,   118,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    15,     0,    16,    17,    18,
      19,    20,   190,    22,    23,    24,   174,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,     0,     0,   103,   104,     0,     0,
       0,     0,   105,     0,   106,     0,     0,   107,     0,   108,
     109,   110,     0,   111,   112,   113,   114,   115,     0,   116,
     117,   118,    15,     0,    16,    17,    18,    19,    20,   190,
      22,    23,    24,     0,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,     0,     0,   103,   104,     0,     0,     0,     0,   105,
       0,   106,     0,     0,   107,     0,   108,   109,   110,     0,
     111,   112,   113,   114,   115,     0,   116,   117,   118,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    15,   293,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,     0,     0,   103,   104,     0,
       0,     0,     0,   105,     0,   106,     0,     0,   107,     0,
     108,   109,   110,     0,   111,   112,   113,   114,   115,     0,
     116,   117,   118,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    15,     0,    16,    17,    18,    19,
      20,   190,    22,    23,    24,   375,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,     0,   103,   104,     0,     0,     0,
       0,   105,     0,   106,     0,     0,   107,     0,   108,   109,
     110,     0,   111,   112,   113,   114,   115,     0,   116,   117,
     118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428
  };

  /* YYCHECK.  */
  const short int
  LolFxParser::yycheck_[] =
  {
         8,     8,     8,   214,     8,     8,     3,     4,   224,   440,
       8,     8,   438,     8,   440,   446,     3,     4,     5,     6,
       7,     8,     9,   176,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,   206,   193,   100,   101,   269,   269,   189,   362,   106,
     196,   108,   130,   306,   111,   422,   113,   114,   115,   137,
     117,   118,   119,   120,   121,   446,   123,   124,   125,   137,
       8,   230,   137,     0,   152,   153,   127,   128,   129,   451,
     461,   202,    21,   415,   416,   176,   164,   438,   209,   446,
     255,   413,   414,   460,   380,   172,   217,   135,   444,   245,
     177,    21,   474,   180,   461,    17,    18,    19,   175,   268,
     148,   189,   426,   191,   440,   193,   442,   191,   169,   196,
     442,   212,   213,   455,   456,   447,   444,   440,   206,     8,
     272,   384,   440,   446,   445,    21,   214,   288,   446,   363,
     363,   455,   233,   440,   458,   310,   152,   446,   111,   446,
     227,   440,   115,   377,   377,     8,   111,   446,   442,   318,
     115,   111,   101,     4,   478,   115,   194,     8,   245,     8,
     237,   485,   111,   112,   333,     8,   115,   255,   269,   441,
     442,   302,   444,   189,   305,   306,     8,   193,   266,   445,
     446,   111,   112,   314,   272,   115,   477,   356,   422,   422,
     452,   453,   454,   362,   342,   343,   344,   345,   445,   446,
     288,   438,   448,   449,     8,   101,   438,   368,   419,   420,
      11,    12,   439,   301,   445,   111,   112,   441,   306,   115,
      21,   446,   310,   354,   417,   418,   441,   442,   445,   446,
     440,   445,   446,   441,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   384,   488,   488,   272,   426,   445,   446,
     441,   442,   363,   445,   446,   338,   339,    11,    12,    13,
      14,     8,   288,   448,   340,   341,   377,    21,   346,   347,
     368,   444,   440,   440,   453,   461,   455,   440,   458,   458,
     306,   445,   457,   444,   444,   444,   384,   444,   444,   100,
     101,   421,   390,   440,   459,   423,   444,     8,   443,   478,
     111,   438,     8,   444,   115,     4,   485,   442,   126,   445,
     442,   422,   441,   124,     6,   446,   443,     8,   444,   504,
     505,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   445,   473,   443,   441,   444,   413,   414,   441,   443,
     441,   441,   368,   440,   440,   438,   100,   101,   439,   441,
     448,   441,   441,   451,   438,     4,   442,   111,   384,   441,
     114,   115,   443,   440,     8,   440,   445,   444,   441,   443,
     124,   448,   449,   450,   451,   441,   474,   488,     8,   477,
       8,     8,     4,     8,   107,   440,   440,   440,   440,   440,
       5,   440,   439,   443,   440,   440,   440,   181,   440,   440,
     443,   440,   440,   443,   441,   441,   504,   505,     3,     4,
       5,     6,     7,     8,     9,   440,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   440,   108,   109,   110,   111,   440,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,     3,     4,     5,     6,     7,     8,     9,   445,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   440,   108,   109,   110,
     111,   438,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,     3,     4,     5,     6,
       7,     8,     9,   283,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     315,   108,   109,   110,   111,   348,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     349,   351,   350,   266,   157,   352,   165,   353,   266,   127,
     286,    25,   289,   379,   301,   474,   120,   301,   137,   384,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   413,   414,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   438,   439,   440,    -1,    -1,    -1,   444,
       9,    -1,    -1,   448,   449,   450,   451,    -1,    17,    18,
      19,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      -1,    -1,   413,   414,    -1,    -1,    -1,   106,    -1,   108,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,
     119,   120,   121,    -1,   123,    -1,   125,   438,   439,   440,
      -1,     9,    -1,   444,    -1,    -1,    -1,   448,   449,   450,
     451,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,   413,   414,   106,    -1,
     108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,    -1,   123,    -1,   125,    -1,    -1,
      -1,   438,   439,   440,    -1,    -1,    -1,   444,    -1,    -1,
      -1,   448,   449,   450,   451,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,    -1,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,    -1,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    -1,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   413,   414,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     438,   439,   440,    -1,    -1,    -1,   444,     7,     8,     9,
     448,   449,   450,   451,    -1,    -1,    -1,    17,    18,    19,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,   413,
     414,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,   119,
     120,   121,    -1,   123,   438,   125,   440,    -1,    -1,    -1,
     444,    -1,    -1,    -1,   448,   449,   450,   451,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   413,   414,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   438,    -1,
     440,    -1,    -1,    -1,   444,    -1,    -1,    -1,   448,   449,
     450,   451,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,    -1,   123,   124,   125,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    19,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,    -1,   123,    -1,   125,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    19,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,   117,   118,   119,   120,   121,    -1,   123,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   413,   414,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   444,    -1,    -1,    -1,   448,   449,   450,
     451,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   413,   414,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   443,   444,    -1,    -1,    -1,
     448,   449,   450,   451,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   413,   414,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   440,    -1,    -1,    -1,   444,
      -1,    -1,    -1,   448,   449,   450,   451,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    19,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,   117,   118,   119,   120,   121,    -1,   123,    -1,   125,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    19,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,   108,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,   117,   118,   119,   120,   121,    -1,
     123,    -1,   125,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,   119,
     120,   121,    -1,   123,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   413,   414,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   443,   444,    -1,
      -1,    -1,   448,   449,   450,   451,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     413,   414,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     443,   444,    -1,    -1,    -1,   448,   449,   450,   451,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   413,   414,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   443,   444,    -1,    -1,    -1,   448,   449,
     450,   451,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,   117,   118,   119,   120,
     121,    -1,   123,    -1,   125,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    19,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     108,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,    -1,   123,    -1,   125,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    19,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,   117,   118,   119,   120,   121,    -1,   123,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   413,   414,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   443,   444,     8,     9,    -1,   448,   449,   450,
     451,    -1,    -1,    17,    18,    19,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,   413,   414,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,   117,   118,   119,   120,   121,    -1,   123,
      -1,   125,    -1,    -1,    -1,    -1,   444,    -1,    -1,    -1,
     448,   449,   450,   451,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   413,   414,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   444,
      -1,    -1,    -1,   448,   449,   450,   451,     9,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
      -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,   440,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,   106,    -1,   108,    -1,    -1,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,    -1,   123,
     124,   125,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,   106,
      -1,   108,    -1,    -1,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,    -1,   123,   124,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,   439,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,   106,    -1,   108,    -1,    -1,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,    -1,
     123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   439,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,   106,    -1,   108,    -1,    -1,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,    -1,   123,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   439
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned short int
  LolFxParser::yystos_[] =
  {
         0,   224,   380,   463,   464,   465,   466,   471,   472,     8,
     196,   245,   473,     0,   464,     9,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,   100,   101,   106,   108,   111,   113,   114,
     115,   117,   118,   119,   120,   121,   123,   124,   125,   505,
     506,   507,   508,   509,   514,   515,   516,   517,   518,   519,
     523,   524,   525,   526,   527,   528,   529,   554,   555,   556,
     438,   444,   111,   115,   111,   115,   444,     8,   528,   111,
     115,     8,   438,   438,   440,   540,   445,   446,    21,   101,
     111,   112,   115,   511,   512,   522,   440,   446,     8,   518,
     524,   524,   524,     8,   440,   525,   442,   526,   555,   194,
     467,   468,     8,   474,   475,     8,   520,   521,   526,   438,
      16,   523,   525,   530,   531,     3,     4,     5,     6,     7,
       8,    98,    99,   102,   103,   104,   105,   109,   110,   116,
     122,   126,   413,   414,   438,   439,   440,   444,   448,   449,
     450,   451,   476,   477,   478,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   503,   505,   506,
     525,   535,   536,   537,   538,   541,   542,   543,   546,   548,
     549,   553,   511,   510,   513,   525,   512,     8,   441,   442,
     444,   524,   438,   443,   487,   500,   504,   525,     8,   439,
     467,   441,   445,   446,   441,   445,   446,   440,   530,   525,
       8,   532,   533,   439,   531,   440,   503,   440,   461,   440,
     536,   444,   444,   440,   503,   444,   444,   487,   487,   439,
     541,   503,   413,   414,   442,   447,   445,   445,   446,   125,
     501,   444,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   441,   502,   487,   452,   453,   454,   448,   449,
     419,   420,   415,   416,   455,   456,   417,   418,   457,   458,
     459,   421,   423,   422,   460,   440,   446,   439,   536,     8,
     510,   513,   441,   442,   501,   534,   443,   504,   530,   443,
     438,     8,   475,     4,   521,   439,   532,   442,   440,   446,
     461,   126,   535,   542,   550,   503,   440,   503,   503,   516,
     523,   545,   439,   445,   479,   503,     7,     8,   482,   501,
     501,   487,   487,   487,   489,   489,   490,   490,   491,   491,
     491,   491,   492,   492,   493,   494,   495,   496,   497,   498,
     503,   501,   442,   534,   443,   504,   441,   443,   439,     8,
     172,   177,   180,   196,   227,   245,   440,   469,   470,     6,
     440,   443,   504,   533,   444,   545,   551,   552,   445,   445,
       8,   445,   443,   461,   504,   441,   443,   534,   441,     8,
     440,   441,   442,   441,   441,   441,   441,   442,   441,   439,
     469,   443,   503,   440,   445,   536,   544,   438,   441,   537,
     539,   540,   501,   443,   534,   441,   534,   440,   442,     3,
       4,     8,   175,   237,     4,     8,     8,     8,     8,     4,
       8,   445,   503,   539,   107,   541,   547,   534,   534,   443,
     504,   440,   440,   440,   440,   440,   443,   440,   440,   440,
     440,   443,   440,   440,   536,   439,   440,   443,   441,   441,
     440,     4,     8,     8,   191,   440,   440,   440,   440
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  LolFxParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,   691,   692,   123,   125,
      59,    61,    91,    93,    40,    41,    44,    46,    43,    45,
      33,   126,    42,    47,    37,    60,    62,    38,    94,   124,
      63,    58
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned short int
  LolFxParser::yyr1_[] =
  {
         0,   462,   463,   464,   464,   465,   465,   466,   467,   467,
     468,   469,   469,   470,   470,   470,   470,   470,   470,   470,
     470,   470,   470,   470,   470,   470,   470,   470,   471,   472,
     473,   473,   474,   474,   475,   475,   476,   477,   477,   477,
     477,   477,   477,   478,   478,   478,   478,   478,   478,   479,
     480,   481,   481,   482,   482,   483,   483,   484,   484,   485,
     486,   486,   486,   487,   487,   487,   487,   488,   488,   488,
     488,   489,   489,   489,   489,   490,   490,   490,   491,   491,
     491,   492,   492,   492,   492,   492,   493,   493,   493,   494,
     494,   495,   495,   496,   496,   497,   497,   498,   498,   499,
     499,   500,   500,   501,   501,   502,   502,   502,   502,   502,
     502,   502,   502,   502,   502,   502,   503,   503,   504,   505,
     505,   505,   505,   505,   505,   505,   505,   506,   507,   507,
     508,   508,   509,   510,   510,   511,   511,   511,   511,   512,
     512,   512,   512,   512,   513,   514,   514,   514,   514,   514,
     514,   514,   515,   515,   515,   515,   515,   515,   515,   515,
     516,   516,   517,   518,   518,   518,   519,   520,   520,   521,
     521,   522,   523,   523,   523,   523,   523,   523,   523,   523,
     524,   524,   524,   524,   524,   524,   524,   524,   524,   524,
     524,   525,   525,   526,   526,   526,   527,   527,   527,   527,
     527,   527,   527,   527,   527,   527,   527,   527,   527,   527,
     527,   527,   527,   527,   527,   527,   527,   527,   527,   527,
     527,   527,   527,   527,   527,   527,   527,   527,   527,   527,
     527,   527,   527,   527,   527,   527,   527,   527,   527,   527,
     527,   527,   527,   527,   527,   527,   527,   527,   527,   527,
     527,   527,   527,   527,   527,   527,   527,   527,   527,   527,
     527,   527,   527,   527,   527,   527,   527,   527,   527,   527,
     527,   527,   527,   527,   527,   527,   527,   527,   527,   527,
     527,   527,   527,   528,   528,   528,   529,   529,   530,   530,
     531,   531,   532,   532,   533,   533,   533,   534,   535,   536,
     536,   537,   537,   537,   537,   537,   537,   537,   538,   538,
     539,   539,   540,   540,   541,   541,   542,   542,   543,   544,
     544,   545,   545,   546,   547,   547,   548,   548,   549,   549,
     549,   550,   550,   551,   551,   552,   552,   553,   553,   553,
     553,   553,   554,   554,   555,   555,   556
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  LolFxParser::yyr2_[] =
  {
         0,     2,     1,     1,     2,     1,     1,     5,     1,     2,
       5,     1,     2,     1,     4,     4,     4,     4,     4,     7,
       7,     7,     7,     4,     4,     4,     4,     4,     2,     5,
       1,     1,     3,     1,     4,     3,     1,     1,     1,     1,
       1,     1,     3,     1,     4,     1,     3,     2,     2,     1,
       1,     1,     3,     2,     2,     2,     1,     2,     3,     2,
       1,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     5,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     2,
       2,     4,     6,     7,     9,    10,     2,     2,     1,     1,
       2,     3,     3,     2,     5,     3,     2,     3,     2,     0,
       1,     1,     1,     1,     1,     1,     3,     5,     6,     7,
       8,     5,     1,     2,     4,     5,     6,     7,     4,     2,
       1,     2,     1,     1,     1,     1,     4,     1,     3,     1,
       3,     1,     1,     1,     2,     2,     1,     2,     3,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     2,     1,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     4,     1,     2,
       3,     4,     1,     3,     1,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     3,
       1,     1,     2,     3,     1,     2,     1,     2,     5,     3,
       1,     1,     4,     7,     0,     1,     3,     2,     5,     7,
       6,     1,     1,     1,     0,     2,     3,     2,     2,     2,
       3,     2,     1,     2,     1,     1,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const LolFxParser::yytname_[] =
  {
    "T_END", "error", "$undefined", "BOOLCONSTANT", "INTCONSTANT",
  "UINTCONSTANT", "FLOATCONSTANT", "FIELDSELECTION", "IDENTIFIER",
  "TYPENAME", "GT_SUBROUTINE", "GT_PATCH", "GT_SAMPLE", "GT_FLAT",
  "GT_SMOOTH", "GT_LAYOUT", "GT_INVARIANT", "GT_HIGHP", "GT_MEDIUMP",
  "GT_LOWP", "GT_PRECISION", "GT_ATTRIBUTE", "GT_VEC2", "GT_VEC3",
  "GT_VEC4", "GT_BVEC2", "GT_BVEC3", "GT_BVEC4", "GT_DVEC2", "GT_DVEC3",
  "GT_DVEC4", "GT_IVEC2", "GT_IVEC3", "GT_IVEC4", "GT_UVEC2", "GT_UVEC3",
  "GT_UVEC4", "GT_MAT2", "GT_MAT2X2", "GT_MAT2X3", "GT_MAT2X4", "GT_MAT3",
  "GT_MAT3X2", "GT_MAT3X3", "GT_MAT3X4", "GT_MAT4", "GT_MAT4X2",
  "GT_MAT4X3", "GT_MAT4X4", "GT_DMAT2", "GT_DMAT2X2", "GT_DMAT2X3",
  "GT_DMAT2X4", "GT_DMAT3", "GT_DMAT3X2", "GT_DMAT3X3", "GT_DMAT3X4",
  "GT_DMAT4", "GT_DMAT4X2", "GT_DMAT4X3", "GT_DMAT4X4",
  "GT_SAMPLER1DSHADOW", "GT_SAMPLER1DARRAY", "GT_SAMPLER1DARRAYSHADOW",
  "GT_SAMPLER2DSHADOW", "GT_SAMPLER2DARRAY", "GT_SAMPLER2DARRAYSHADOW",
  "GT_SAMPLER2DRECT", "GT_SAMPLER2DRECTSHADOW", "GT_SAMPLER2DMS",
  "GT_SAMPLER2DMSARRAY", "GT_SAMPLERCUBESHADOW", "GT_SAMPLERCUBEARRAY",
  "GT_SAMPLERCUBEARRAYSHADOW", "GT_SAMPLERBUFFER", "GT_ISAMPLER1D",
  "GT_ISAMPLER1DARRAY", "GT_ISAMPLER2D", "GT_ISAMPLER2DARRAY",
  "GT_ISAMPLER2DRECT", "GT_ISAMPLER2DMS", "GT_ISAMPLER2DMSARRAY",
  "GT_ISAMPLER3D", "GT_ISAMPLERCUBE", "GT_ISAMPLERCUBEARRAY",
  "GT_ISAMPLERBUFFER", "GT_USAMPLER1D", "GT_USAMPLER1DARRAY",
  "GT_USAMPLER2D", "GT_USAMPLER2DARRAY", "GT_USAMPLER2DRECT",
  "GT_USAMPLER2DMS", "GT_USAMPLER2DMSARRAY", "GT_USAMPLER3D",
  "GT_USAMPLERCUBE", "GT_USAMPLERCUBEARRAY", "GT_USAMPLERBUFFER",
  "GHT_BOOL", "GHT_BREAK", "GHT_CASE", "GHT_CENTROID", "GHT_CONST",
  "GHT_CONTINUE", "GHT_DEFAULT", "GHT_DISCARD", "GHT_DO", "GHT_DOUBLE",
  "GHT_ELSE", "GHT_FLOAT", "GHT_FOR", "GHT_IF", "GHT_IN", "GHT_INOUT",
  "GHT_INT", "GHT_NOPERSPECTIVE", "GHT_OUT", "GHT_RETURN", "GHT_SAMPLER1D",
  "GHT_SAMPLER2D", "GHT_SAMPLER3D", "GHT_SAMPLERCUBE", "GHT_STRUCT",
  "GHT_SWITCH", "GHT_UINT", "GHT_UNIFORM", "GHT_VOID", "GHT_WHILE",
  "HGT_BOOL", "HGT_BREAK", "HGT_CASE", "HGT_CENTROID", "HGT_CONST",
  "HGT_CONTINUE", "HGT_DEFAULT", "HGT_DISCARD", "HGT_DO", "HGT_DOUBLE",
  "HGT_ELSE", "HGT_FLOAT", "HGT_FOR", "HGT_IF", "HGT_IN", "HGT_INOUT",
  "HGT_INT", "HGT_NOPERSPECTIVE", "HGT_OUT", "HGT_RETURN", "HGT_SAMPLER1D",
  "HGT_SAMPLER2D", "HGT_SAMPLER3D", "HGT_SAMPLERCUBE", "HGT_STRUCT",
  "HGT_SWITCH", "HGT_UINT", "HGT_UNIFORM", "HGT_VOID", "HGT_WHILE",
  "HT_APPENDSTRUCTUREDBUFFER", "HT_ASM", "HT_ASM_FRAGMENT",
  "HT_BLENDSTATE", "HT_BUFFER", "HT_BYTEADDRESSBUFFER", "HT_CBUFFER",
  "HT_COLUMN_MAJOR", "HT_COMPILE", "HT_COMPILE_FRAGMENT",
  "HT_COMPILESHADER", "HT_COMPUTESHADER", "HT_CONSUMESTRUCTUREDBUFFER",
  "HT_DEPTHSTENCILSTATE", "HT_DEPTHSTENCILVIEW", "HT_DOMAINSHADER",
  "HT_DWORD", "HT_EXTERN", "HT_FALSE", "HT_FXGROUP", "HT_GEOMETRYSHADER",
  "HT_GROUPSHARED", "HT_HALF", "HT_HULLSHADER", "HT_INLINE",
  "HT_INPUTPATCH", "HT_INTERFACE", "HT_LINE", "HT_LINEADJ", "HT_LINEAR",
  "HT_LINESTREAM", "HT_MATRIX", "HT_NAMESPACE", "HT_NOINTERPOLATION",
  "HT_NULL", "HT_OUTPUTPATCH", "HT_PACKOFFSET", "HT_PASS",
  "HT_PIXELFRAGMENT", "HT_PIXELSHADER", "HT_POINT", "HT_POINTSTREAM",
  "HT_PRECISE", "HT_RASTERIZERSTATE", "HT_RENDERTARGETVIEW", "HT_REGISTER",
  "HT_ROW_MAJOR", "HT_RWBUFFER", "HT_RWBYTEADDRESSBUFFER",
  "HT_RWSTRUCTUREDBUFFER", "HT_RWTEXTURE1D", "HT_RWTEXTURE1DARRAY",
  "HT_RWTEXTURE2D", "HT_RWTEXTURE2DARRAY", "HT_RWTEXTURE3D", "HT_SAMPLER",
  "HT_SAMPLER_STATE", "HT_SAMPLERSTATE", "HT_SAMPLERCOMPARISONSTATE",
  "HT_SHARED", "HT_SNORM", "HT_STATEBLOCK", "HT_STATEBLOCK_STATE",
  "HT_STATIC", "HT_STRING", "HT_STRUCTUREDBUFFER", "HT_TBUFFER",
  "HT_TECHNIQUE", "HT_TECHNIQUE10", "HT_TECHNIQUE11XZ", "HT_TEXTURE",
  "HT_TEXTURE1D", "HT_TEXTURE1DARRAY", "HT_TEXTURE2D", "HT_TEXTURE2DARRAY",
  "HT_TEXTURE2DMS", "HT_TEXTURE2DMSARRAY", "HT_TEXTURE3D",
  "HT_TEXTURECUBE", "HT_TEXTURECUBEARRAY", "HT_TRUE", "HT_TYPEDEF",
  "HT_TRIANGLE", "HT_TRIANGLEADJ", "HT_TRIANGLESTREAM", "HT_UNORM",
  "HT_VECTOR", "HT_VERTEXFRAGMENT", "HT_VERTEXSHADER", "HT_VOLATILE",
  "HT_BOOL1", "HT_BOOL1x1", "HT_BOOL2x1", "HT_BOOL3x1", "HT_BOOL4x1",
  "HT_BOOL2", "HT_BOOL1x2", "HT_BOOL2x2", "HT_BOOL3x2", "HT_BOOL4x2",
  "HT_BOOL3", "HT_BOOL1x3", "HT_BOOL2x3", "HT_BOOL3x3", "HT_BOOL4x3",
  "HT_BOOL4", "HT_BOOL1x4", "HT_BOOL2x4", "HT_BOOL3x4", "HT_BOOL4x4",
  "HT_FLOAT1", "HT_FLOAT1x1", "HT_FLOAT2x1", "HT_FLOAT3x1", "HT_FLOAT4x1",
  "HT_FLOAT2", "HT_FLOAT1x2", "HT_FLOAT2x2", "HT_FLOAT3x2", "HT_FLOAT4x2",
  "HT_FLOAT3", "HT_FLOAT1x3", "HT_FLOAT2x3", "HT_FLOAT3x3", "HT_FLOAT4x3",
  "HT_FLOAT4", "HT_FLOAT1x4", "HT_FLOAT2x4", "HT_FLOAT3x4", "HT_FLOAT4x4",
  "HT_DOUBLE1", "HT_DOUBLE1x1", "HT_DOUBLE2x1", "HT_DOUBLE3x1",
  "HT_DOUBLE4x1", "HT_DOUBLE2", "HT_DOUBLE1x2", "HT_DOUBLE2x2",
  "HT_DOUBLE3x2", "HT_DOUBLE4x2", "HT_DOUBLE3", "HT_DOUBLE1x3",
  "HT_DOUBLE2x3", "HT_DOUBLE3x3", "HT_DOUBLE4x3", "HT_DOUBLE4",
  "HT_DOUBLE1x4", "HT_DOUBLE2x4", "HT_DOUBLE3x4", "HT_DOUBLE4x4",
  "HT_DWORD1", "HT_DWORD1x1", "HT_DWORD2x1", "HT_DWORD3x1", "HT_DWORD4x1",
  "HT_DWORD2", "HT_DWORD1x2", "HT_DWORD2x2", "HT_DWORD3x2", "HT_DWORD4x2",
  "HT_DWORD3", "HT_DWORD1x3", "HT_DWORD2x3", "HT_DWORD3x3", "HT_DWORD4x3",
  "HT_DWORD4", "HT_DWORD1x4", "HT_DWORD2x4", "HT_DWORD3x4", "HT_DWORD4x4",
  "HT_INT1", "HT_INT1x1", "HT_INT2x1", "HT_INT3x1", "HT_INT4x1", "HT_INT2",
  "HT_INT1x2", "HT_INT2x2", "HT_INT3x2", "HT_INT4x2", "HT_INT3",
  "HT_INT1x3", "HT_INT2x3", "HT_INT3x3", "HT_INT4x3", "HT_INT4",
  "HT_INT1x4", "HT_INT2x4", "HT_INT3x4", "HT_INT4x4", "HT_UINT1",
  "HT_UINT1x1", "HT_UINT2x1", "HT_UINT3x1", "HT_UINT4x1", "HT_UINT2",
  "HT_UINT1x2", "HT_UINT2x2", "HT_UINT3x2", "HT_UINT4x2", "HT_UINT3",
  "HT_UINT1x3", "HT_UINT2x3", "HT_UINT3x3", "HT_UINT4x3", "HT_UINT4",
  "HT_UINT1x4", "HT_UINT2x4", "HT_UINT3x4", "HT_UINT4x4",
  "PREPROCESSOR_DEFINE", "PREPROCESSOR_ELIF", "PREPROCESSOR_ELSE",
  "PREPROCESSOR_ENDIF", "PREPROCESSOR_ERROR", "PREPROCESSOR_IF",
  "PREPROCESSOR_IFDEF", "PREPROCESSOR_IFNDEF", "PREPROCESSOR_INCLUDE",
  "PREPROCESSOR_LINE", "PREPROCESSOR_PRAGMA", "PREPROCESSOR_UNDEF",
  "PREPROCESSOR_REGION", "PRAGMA_LOLFX", "HT_AUTO", "HT_CATCH", "HT_CHAR",
  "HT_CLASS", "HT_CONST_CAST", "HT_DELETE", "HT_DYNAMIC_CAST", "HT_ENUM",
  "HT_EXPLICIT", "HT_FRIEND", "HT_GOTO", "HT_LONG", "HT_MUTABLE", "HT_NEW",
  "HT_OPERATOR", "HT_PRIVATE", "HT_PROTECTED", "HT_PUBLIC",
  "HT_REINTERPRET_CAST", "HT_SHORT", "HT_SIGNED", "HT_SIZEOF",
  "HT_STATIC_CAST", "HT_TEMPLATE", "HT_THIS", "HT_THROW", "HT_TRY",
  "HT_TYPENAME", "HT_UNION", "HT_UNSIGNED", "HT_USING", "HT_VIRTUAL",
  "T_INC", "T_DEC", "T_LE", "T_GE", "T_EQ", "T_NE", "T_LEFT", "T_RIGHT",
  "T_AND", "T_OR", "T_XOR", "T_MULEQ", "T_DIVEQ", "T_MODEQ", "T_ADDEQ",
  "T_SUBEQ", "T_LEFTEQ", "T_RIGHTEQ", "T_ANDEQ", "T_XOREQ", "T_OREQ",
  "FLOAT", "STRING", "NAME", "T_ERROR", "'{'", "'}'", "';'", "'='", "'['",
  "']'", "'('", "')'", "','", "'.'", "'+'", "'-'", "'!'", "'~'", "'*'",
  "'/'", "'%'", "'<'", "'>'", "'&'", "'^'", "'|'", "'?'", "':'", "$accept",
  "lolfx_file", "lolfx_section_list", "lolfx_section", "lolfx_technique",
  "pass_list", "pass", "pass_stmt_list", "pass_stmt", "lolfx_shader",
  "lolfx_shader_declaration", "lolfx_shader_type",
  "lolfx_shader_description_list", "lolfx_shader_description",
  "glsl_variable_identifier", "glsl_primary_expression",
  "glsl_postfix_expression", "glsl_integer_expression",
  "glsl_function_call", "glsl_function_call_or_method",
  "glsl_function_call_generic", "glsl_function_call_header_no_parameters",
  "glsl_function_call_header_with_parameters", "glsl_function_call_header",
  "glsl_function_identifier", "glsl_unary_expression",
  "glsl_unary_operator", "glsl_multiplicative_expression",
  "glsl_additive_expression", "glsl_shift_expression",
  "glsl_relational_expression", "glsl_equality_expression",
  "glsl_and_expression", "glsl_exclusive_or_expression",
  "glsl_inclusive_or_expression", "glsl_logical_and_expression",
  "glsl_logical_xor_expression", "glsl_logical_or_expression",
  "glsl_conditional_expression", "glsl_assignment_expression",
  "glsl_assignment_operator", "glsl_expression",
  "glsl_constant_expression", "glsl_declaration",
  "glsl_function_prototype", "glsl_function_declarator",
  "glsl_function_header_with_parameters", "glsl_function_header",
  "glsl_parameter_declarator", "glsl_parameter_declaration",
  "glsl_parameter_qualifier", "glsl_parameter_type_specifier",
  "glsl_init_declarator_list", "glsl_single_declaration",
  "glsl_fully_specified_type", "glsl_invariant_qualifier",
  "glsl_interpolation_qualifier", "glsl_layout_qualifier",
  "glsl_layout_qualifier_id_list", "glsl_layout_qualifier_id",
  "glsl_parameter_type_qualifier", "glsl_type_qualifier",
  "glsl_storage_qualifier", "glsl_type_specifier",
  "glsl_type_specifier_no_prec", "glsl_type_specifier_nonarray",
  "glsl_precision_qualifier", "glsl_struct_specifier",
  "glsl_struct_declaration_list", "glsl_struct_declaration",
  "glsl_struct_declarator_list", "glsl_struct_declarator",
  "glsl_initializer", "glsl_declaration_statement", "glsl_statement",
  "glsl_simple_statement", "glsl_compound_statement",
  "glsl_statement_no_new_scope", "glsl_compound_statement_no_new_scope",
  "glsl_statement_list", "glsl_expression_statement",
  "glsl_selection_statement", "glsl_selection_rest_statement",
  "glsl_condition", "glsl_switch_statement", "glsl_switch_statement_list",
  "glsl_case_label", "glsl_iteration_statement", "glsl_for_init_statement",
  "glsl_conditionopt", "glsl_for_rest_statement", "glsl_jump_statement",
  "glsl_translation_unit", "glsl_external_declaration",
  "glsl_function_definition", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const LolFxParser::rhs_number_type
  LolFxParser::yyrhs_[] =
  {
       463,     0,    -1,   464,    -1,   465,    -1,   465,   464,    -1,
     466,    -1,   471,    -1,   224,     8,   438,   467,   439,    -1,
     468,    -1,   468,   467,    -1,   194,     8,   438,   469,   439,
      -1,   470,    -1,   470,   469,    -1,   440,    -1,     8,   441,
       8,   440,    -1,     8,   441,     4,   440,    -1,     8,   441,
       3,   440,    -1,     8,   441,   175,   440,    -1,     8,   441,
     237,   440,    -1,     8,   442,     4,   443,   441,     8,   440,
      -1,     8,   442,     4,   443,   441,     4,   440,    -1,   227,
     442,     4,   443,   441,     8,   440,    -1,   227,   442,     4,
     443,   441,   191,   440,    -1,   172,   441,     8,   440,    -1,
     177,   441,     8,   440,    -1,   180,   441,     8,   440,    -1,
     196,   441,     8,   440,    -1,   245,   441,     8,   440,    -1,
     472,   554,    -1,   380,   473,   444,   474,   445,    -1,   245,
      -1,   196,    -1,   475,   446,   475,    -1,   475,    -1,     8,
     441,     8,     6,    -1,     8,   441,     8,    -1,     8,    -1,
     476,    -1,     4,    -1,     5,    -1,     6,    -1,     3,    -1,
     444,   503,   445,    -1,   477,    -1,   478,   442,   479,   443,
      -1,   480,    -1,   478,   447,     7,    -1,   478,   413,    -1,
     478,   414,    -1,   503,    -1,   481,    -1,   482,    -1,   478,
     447,   482,    -1,   484,   445,    -1,   483,   445,    -1,   485,
     125,    -1,   485,    -1,   485,   501,    -1,   484,   446,   501,
      -1,   486,   444,    -1,   525,    -1,     8,    -1,     7,    -1,
     478,    -1,   413,   487,    -1,   414,   487,    -1,   488,   487,
      -1,   448,    -1,   449,    -1,   450,    -1,   451,    -1,   487,
      -1,   489,   452,   487,    -1,   489,   453,   487,    -1,   489,
     454,   487,    -1,   489,    -1,   490,   448,   489,    -1,   490,
     449,   489,    -1,   490,    -1,   491,   419,   490,    -1,   491,
     420,   490,    -1,   491,    -1,   492,   455,   491,    -1,   492,
     456,   491,    -1,   492,   415,   491,    -1,   492,   416,   491,
      -1,   492,    -1,   493,   417,   492,    -1,   493,   418,   492,
      -1,   493,    -1,   494,   457,   493,    -1,   494,    -1,   495,
     458,   494,    -1,   495,    -1,   496,   459,   495,    -1,   496,
      -1,   497,   421,   496,    -1,   497,    -1,   498,   423,   497,
      -1,   498,    -1,   499,   422,   498,    -1,   499,    -1,   499,
     460,   503,   461,   501,    -1,   500,    -1,   487,   502,   501,
      -1,   441,    -1,   424,    -1,   425,    -1,   426,    -1,   427,
      -1,   428,    -1,   429,    -1,   430,    -1,   431,    -1,   432,
      -1,   433,    -1,   501,    -1,   503,   446,   501,    -1,   500,
      -1,   506,   440,    -1,   514,   440,    -1,    20,   528,   526,
     440,    -1,   523,     8,   438,   530,   439,   440,    -1,   523,
       8,   438,   530,   439,     8,   440,    -1,   523,     8,   438,
     530,   439,     8,   442,   443,   440,    -1,   523,     8,   438,
     530,   439,     8,   442,   504,   443,   440,    -1,   523,   440,
      -1,   507,   445,    -1,   509,    -1,   508,    -1,   509,   511,
      -1,   508,   446,   511,    -1,   516,     8,   444,    -1,   525,
       8,    -1,   525,     8,   442,   504,   443,    -1,   522,   512,
     510,    -1,   512,   510,    -1,   522,   512,   513,    -1,   512,
     513,    -1,    -1,   111,    -1,   115,    -1,   112,    -1,    21,
      -1,   525,    -1,   515,    -1,   514,   446,     8,    -1,   514,
     446,     8,   442,   443,    -1,   514,   446,     8,   442,   504,
     443,    -1,   514,   446,     8,   442,   443,   441,   534,    -1,
     514,   446,     8,   442,   504,   443,   441,   534,    -1,   514,
     446,     8,   441,   534,    -1,   516,    -1,   516,     8,    -1,
     516,     8,   442,   443,    -1,   516,     8,   442,   504,   443,
      -1,   516,     8,   442,   443,   441,   534,    -1,   516,     8,
     442,   504,   443,   441,   534,    -1,   516,     8,   441,   534,
      -1,    16,     8,    -1,   525,    -1,   523,   525,    -1,    16,
      -1,    14,    -1,    13,    -1,   114,    -1,    15,   444,   520,
     445,    -1,   521,    -1,   520,   446,   521,    -1,     8,    -1,
       8,   441,     4,    -1,   101,    -1,   524,    -1,   519,    -1,
     519,   524,    -1,   518,   524,    -1,   518,    -1,   517,   524,
      -1,   517,   518,   524,    -1,   517,    -1,   101,    -1,   111,
      -1,   115,    -1,   100,   111,    -1,   100,   115,    -1,    11,
     111,    -1,    11,   115,    -1,    12,   111,    -1,    12,   115,
      -1,   124,    -1,    21,    -1,   526,    -1,   528,   526,    -1,
     527,    -1,   527,   442,   443,    -1,   527,   442,   504,   443,
      -1,   125,    -1,   108,    -1,   106,    -1,   113,    -1,   123,
      -1,    97,    -1,    22,    -1,    23,    -1,    24,    -1,    28,
      -1,    29,    -1,    30,    -1,    25,    -1,    26,    -1,    27,
      -1,    31,    -1,    32,    -1,    33,    -1,    34,    -1,    35,
      -1,    36,    -1,    37,    -1,    41,    -1,    45,    -1,    38,
      -1,    39,    -1,    40,    -1,    42,    -1,    43,    -1,    44,
      -1,    46,    -1,    47,    -1,    48,    -1,    49,    -1,    53,
      -1,    57,    -1,    50,    -1,    51,    -1,    52,    -1,    54,
      -1,    55,    -1,    56,    -1,    58,    -1,    59,    -1,    60,
      -1,   117,    -1,   118,    -1,   119,    -1,   120,    -1,    61,
      -1,    64,    -1,    71,    -1,    62,    -1,    65,    -1,    63,
      -1,    66,    -1,    72,    -1,    73,    -1,    75,    -1,    77,
      -1,    82,    -1,    83,    -1,    76,    -1,    78,    -1,    84,
      -1,    86,    -1,    88,    -1,    93,    -1,    94,    -1,    87,
      -1,    89,    -1,    95,    -1,    67,    -1,    68,    -1,    79,
      -1,    90,    -1,    74,    -1,    85,    -1,    96,    -1,    69,
      -1,    80,    -1,    91,    -1,    70,    -1,    81,    -1,    92,
      -1,   529,    -1,     9,    -1,    17,    -1,    18,    -1,    19,
      -1,   121,     8,   438,   530,   439,    -1,   121,   438,   530,
     439,    -1,   531,    -1,   530,   531,    -1,   525,   532,   440,
      -1,   523,   525,   532,   440,    -1,   533,    -1,   532,   446,
     533,    -1,     8,    -1,     8,   442,   443,    -1,     8,   442,
     504,   443,    -1,   501,    -1,   505,    -1,   538,    -1,   537,
      -1,   535,    -1,   542,    -1,   543,    -1,   546,    -1,   548,
      -1,   549,    -1,   553,    -1,   438,   439,    -1,   438,   541,
     439,    -1,   540,    -1,   537,    -1,   438,   439,    -1,   438,
     541,   439,    -1,   536,    -1,   541,   536,    -1,   440,    -1,
     503,   440,    -1,   110,   444,   503,   445,   544,    -1,   536,
     107,   536,    -1,   536,    -1,   503,    -1,   516,     8,   441,
     534,    -1,   122,   444,   503,   445,   438,   547,   439,    -1,
      -1,   541,    -1,    99,   503,   461,    -1,   103,   461,    -1,
     126,   444,   545,   445,   539,    -1,   105,   536,   126,   444,
     503,   445,   440,    -1,   109,   444,   550,   552,   445,   539,
      -1,   542,    -1,   535,    -1,   545,    -1,    -1,   551,   440,
      -1,   551,   440,   503,    -1,   102,   440,    -1,    98,   440,
      -1,   116,   440,    -1,   116,   503,   440,    -1,   104,   440,
      -1,   555,    -1,   554,   555,    -1,   556,    -1,   505,    -1,
     506,   540,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  LolFxParser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,    10,    12,    14,    20,    22,
      25,    31,    33,    36,    38,    43,    48,    53,    58,    63,
      71,    79,    87,    95,   100,   105,   110,   115,   120,   123,
     129,   131,   133,   137,   139,   144,   148,   150,   152,   154,
     156,   158,   160,   164,   166,   171,   173,   177,   180,   183,
     185,   187,   189,   193,   196,   199,   202,   204,   207,   211,
     214,   216,   218,   220,   222,   225,   228,   231,   233,   235,
     237,   239,   241,   245,   249,   253,   255,   259,   263,   265,
     269,   273,   275,   279,   283,   287,   291,   293,   297,   301,
     303,   307,   309,   313,   315,   319,   321,   325,   327,   331,
     333,   337,   339,   345,   347,   351,   353,   355,   357,   359,
     361,   363,   365,   367,   369,   371,   373,   375,   379,   381,
     384,   387,   392,   399,   407,   417,   428,   431,   434,   436,
     438,   441,   445,   449,   452,   458,   462,   465,   469,   472,
     473,   475,   477,   479,   481,   483,   485,   489,   495,   502,
     510,   519,   525,   527,   530,   535,   541,   548,   556,   561,
     564,   566,   569,   571,   573,   575,   577,   582,   584,   588,
     590,   594,   596,   598,   600,   603,   606,   608,   611,   615,
     617,   619,   621,   623,   626,   629,   632,   635,   638,   641,
     643,   645,   647,   650,   652,   656,   661,   663,   665,   667,
     669,   671,   673,   675,   677,   679,   681,   683,   685,   687,
     689,   691,   693,   695,   697,   699,   701,   703,   705,   707,
     709,   711,   713,   715,   717,   719,   721,   723,   725,   727,
     729,   731,   733,   735,   737,   739,   741,   743,   745,   747,
     749,   751,   753,   755,   757,   759,   761,   763,   765,   767,
     769,   771,   773,   775,   777,   779,   781,   783,   785,   787,
     789,   791,   793,   795,   797,   799,   801,   803,   805,   807,
     809,   811,   813,   815,   817,   819,   821,   823,   825,   827,
     829,   831,   833,   835,   837,   839,   841,   847,   852,   854,
     857,   861,   866,   868,   872,   874,   878,   883,   885,   887,
     889,   891,   893,   895,   897,   899,   901,   903,   905,   908,
     912,   914,   916,   919,   923,   925,   928,   930,   933,   939,
     943,   945,   947,   952,   960,   961,   963,   967,   970,   976,
     984,   991,   993,   995,   997,   998,  1001,  1005,  1008,  1011,
    1014,  1018,  1021,  1023,  1026,  1028,  1030
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  LolFxParser::yyrline_[] =
  {
         0,   257,   711,   715,   716,   720,   721,   729,   737,   738,
     742,   746,   747,   752,   753,   754,   755,   756,   757,   758,
     759,   760,   761,   762,   763,   764,   765,   766,   774,   778,
     782,   783,   787,   788,   792,   793,   801,   805,   806,   807,
     808,   809,   810,   814,   815,   816,   817,   818,   819,   823,
     827,   831,   832,   836,   837,   841,   842,   846,   847,   851,
     855,   856,   857,   861,   862,   863,   864,   868,   869,   870,
     871,   875,   876,   877,   878,   882,   883,   884,   888,   889,
     890,   894,   895,   896,   897,   898,   902,   903,   904,   908,
     909,   913,   914,   918,   919,   923,   924,   928,   929,   933,
     934,   938,   939,   943,   944,   948,   949,   950,   951,   952,
     953,   954,   955,   956,   957,   958,   962,   963,   967,   971,
     972,   973,   974,   975,   976,   977,   978,   982,   986,   987,
     991,   992,   996,  1000,  1001,  1005,  1006,  1007,  1008,  1011,
    1013,  1014,  1015,  1016,  1020,  1024,  1025,  1026,  1027,  1028,
    1029,  1030,  1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,
    1045,  1046,  1050,  1054,  1055,  1056,  1060,  1064,  1065,  1069,
    1070,  1074,  1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,
    1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,
    1099,  1103,  1104,  1108,  1109,  1110,  1113,  1114,  1115,  1116,
    1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
    1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,  1156,
    1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,
    1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,
    1177,  1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,
    1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,
    1197,  1198,  1199,  1203,  1204,  1205,  1209,  1210,  1214,  1215,
    1219,  1220,  1224,  1225,  1229,  1230,  1231,  1235,  1239,  1243,
    1244,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1258,  1259,
    1263,  1264,  1268,  1269,  1273,  1274,  1278,  1279,  1283,  1287,
    1288,  1292,  1293,  1297,  1300,  1302,  1306,  1307,  1311,  1312,
    1313,  1317,  1318,  1322,  1323,  1327,  1328,  1332,  1333,  1334,
    1335,  1336,  1340,  1341,  1345,  1346,  1350
  };

  // Print the state stack on the debug stream.
  void
  LolFxParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  LolFxParser::yy_reduce_print_ (int yyrule)
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
  LolFxParser::token_number_type
  LolFxParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   450,     2,     2,     2,   454,   457,     2,
     444,   445,   452,   448,   446,   449,   447,   453,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   461,   440,
     455,   441,   456,   460,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   442,     2,   443,   458,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   438,   459,   439,   451,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int LolFxParser::yyeof_ = 0;
  const int LolFxParser::yylast_ = 4692;
  const int LolFxParser::yynnts_ = 95;
  const int LolFxParser::yyempty_ = -2;
  const int LolFxParser::yyfinal_ = 13;
  const int LolFxParser::yyterror_ = 1;
  const int LolFxParser::yyerrcode_ = 256;
  const int LolFxParser::yyntokens_ = 462;

  const unsigned int LolFxParser::yyuser_token_number_max_ = 692;
  const LolFxParser::token_number_type LolFxParser::yyundef_token_ = 2;


} // lol

/* Line 1136 of lalr1.cc  */
#line 2548 "generated/lolfx-parser.cpp"


/* Line 1138 of lalr1.cc  */
#line 1353 "gpu/lolfx-parser.y"


void lol::LolFxParser::error(const LolFxParser::location_type& l,
                             const std::string& m)
{
    mc.Error(l, m);
}


