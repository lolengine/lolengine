/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
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

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H



#include <string>
#include <iostream>
#include "stack.hh"
#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


namespace lol {

/* Line 35 of lalr1.cc  */
#line 68 "generated/lolfx-parser.h"

  /// A Bison parser.
  class LolFxParser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {

/* Line 35 of lalr1.cc  */
#line 34 "gpu/lolfx-parser.y"

    int ival;
    unsigned uval;
    float fval;
    char *sval;



/* Line 35 of lalr1.cc  */
#line 90 "generated/lolfx-parser.h"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     T_END = 0,
     BOOLCONSTANT = 258,
     INTCONSTANT = 259,
     UINTCONSTANT = 260,
     FLOATCONSTANT = 261,
     FIELDSELECTION = 262,
     IDENTIFIER = 263,
     TYPENAME = 264,
     GT_SUBROUTINE = 265,
     GT_PATCH = 266,
     GT_SAMPLE = 267,
     GT_FLAT = 268,
     GT_SMOOTH = 269,
     GT_LAYOUT = 270,
     GT_INVARIANT = 271,
     GT_HIGHP = 272,
     GT_MEDIUMP = 273,
     GT_LOWP = 274,
     GT_PRECISION = 275,
     GT_ATTRIBUTE = 276,
     GT_VEC2 = 277,
     GT_VEC3 = 278,
     GT_VEC4 = 279,
     GT_BVEC2 = 280,
     GT_BVEC3 = 281,
     GT_BVEC4 = 282,
     GT_DVEC2 = 283,
     GT_DVEC3 = 284,
     GT_DVEC4 = 285,
     GT_IVEC2 = 286,
     GT_IVEC3 = 287,
     GT_IVEC4 = 288,
     GT_UVEC2 = 289,
     GT_UVEC3 = 290,
     GT_UVEC4 = 291,
     GT_MAT2 = 292,
     GT_MAT2X2 = 293,
     GT_MAT2X3 = 294,
     GT_MAT2X4 = 295,
     GT_MAT3 = 296,
     GT_MAT3X2 = 297,
     GT_MAT3X3 = 298,
     GT_MAT3X4 = 299,
     GT_MAT4 = 300,
     GT_MAT4X2 = 301,
     GT_MAT4X3 = 302,
     GT_MAT4X4 = 303,
     GT_DMAT2 = 304,
     GT_DMAT2X2 = 305,
     GT_DMAT2X3 = 306,
     GT_DMAT2X4 = 307,
     GT_DMAT3 = 308,
     GT_DMAT3X2 = 309,
     GT_DMAT3X3 = 310,
     GT_DMAT3X4 = 311,
     GT_DMAT4 = 312,
     GT_DMAT4X2 = 313,
     GT_DMAT4X3 = 314,
     GT_DMAT4X4 = 315,
     GT_SAMPLER1DSHADOW = 316,
     GT_SAMPLER1DARRAY = 317,
     GT_SAMPLER1DARRAYSHADOW = 318,
     GT_SAMPLER2DSHADOW = 319,
     GT_SAMPLER2DARRAY = 320,
     GT_SAMPLER2DARRAYSHADOW = 321,
     GT_SAMPLER2DRECT = 322,
     GT_SAMPLER2DRECTSHADOW = 323,
     GT_SAMPLER2DMS = 324,
     GT_SAMPLER2DMSARRAY = 325,
     GT_SAMPLERCUBESHADOW = 326,
     GT_SAMPLERCUBEARRAY = 327,
     GT_SAMPLERCUBEARRAYSHADOW = 328,
     GT_SAMPLERBUFFER = 329,
     GT_ISAMPLER1D = 330,
     GT_ISAMPLER1DARRAY = 331,
     GT_ISAMPLER2D = 332,
     GT_ISAMPLER2DARRAY = 333,
     GT_ISAMPLER2DRECT = 334,
     GT_ISAMPLER2DMS = 335,
     GT_ISAMPLER2DMSARRAY = 336,
     GT_ISAMPLER3D = 337,
     GT_ISAMPLERCUBE = 338,
     GT_ISAMPLERCUBEARRAY = 339,
     GT_ISAMPLERBUFFER = 340,
     GT_USAMPLER1D = 341,
     GT_USAMPLER1DARRAY = 342,
     GT_USAMPLER2D = 343,
     GT_USAMPLER2DARRAY = 344,
     GT_USAMPLER2DRECT = 345,
     GT_USAMPLER2DMS = 346,
     GT_USAMPLER2DMSARRAY = 347,
     GT_USAMPLER3D = 348,
     GT_USAMPLERCUBE = 349,
     GT_USAMPLERCUBEARRAY = 350,
     GT_USAMPLERBUFFER = 351,
     GHT_BOOL = 352,
     GHT_BREAK = 353,
     GHT_CASE = 354,
     GHT_CENTROID = 355,
     GHT_CONST = 356,
     GHT_CONTINUE = 357,
     GHT_DEFAULT = 358,
     GHT_DISCARD = 359,
     GHT_DO = 360,
     GHT_DOUBLE = 361,
     GHT_ELSE = 362,
     GHT_FLOAT = 363,
     GHT_FOR = 364,
     GHT_IF = 365,
     GHT_IN = 366,
     GHT_INOUT = 367,
     GHT_INT = 368,
     GHT_NOPERSPECTIVE = 369,
     GHT_OUT = 370,
     GHT_RETURN = 371,
     GHT_SAMPLER1D = 372,
     GHT_SAMPLER2D = 373,
     GHT_SAMPLER3D = 374,
     GHT_SAMPLERCUBE = 375,
     GHT_STRUCT = 376,
     GHT_SWITCH = 377,
     GHT_UINT = 378,
     GHT_UNIFORM = 379,
     GHT_VOID = 380,
     GHT_WHILE = 381,
     HGT_BOOL = 382,
     HGT_BREAK = 383,
     HGT_CASE = 384,
     HGT_CENTROID = 385,
     HGT_CONST = 386,
     HGT_CONTINUE = 387,
     HGT_DEFAULT = 388,
     HGT_DISCARD = 389,
     HGT_DO = 390,
     HGT_DOUBLE = 391,
     HGT_ELSE = 392,
     HGT_FLOAT = 393,
     HGT_FOR = 394,
     HGT_IF = 395,
     HGT_IN = 396,
     HGT_INOUT = 397,
     HGT_INT = 398,
     HGT_NOPERSPECTIVE = 399,
     HGT_OUT = 400,
     HGT_RETURN = 401,
     HGT_SAMPLER1D = 402,
     HGT_SAMPLER2D = 403,
     HGT_SAMPLER3D = 404,
     HGT_SAMPLERCUBE = 405,
     HGT_STRUCT = 406,
     HGT_SWITCH = 407,
     HGT_UINT = 408,
     HGT_UNIFORM = 409,
     HGT_VOID = 410,
     HGT_WHILE = 411,
     HT_APPENDSTRUCTUREDBUFFER = 412,
     HT_ASM = 413,
     HT_ASM_FRAGMENT = 414,
     HT_BLENDSTATE = 415,
     HT_BUFFER = 416,
     HT_BYTEADDRESSBUFFER = 417,
     HT_CBUFFER = 418,
     HT_COLUMN_MAJOR = 419,
     HT_COMPILE = 420,
     HT_COMPILE_FRAGMENT = 421,
     HT_COMPILESHADER = 422,
     HT_COMPUTESHADER = 423,
     HT_CONSUMESTRUCTUREDBUFFER = 424,
     HT_DEPTHSTENCILSTATE = 425,
     HT_DEPTHSTENCILVIEW = 426,
     HT_DOMAINSHADER = 427,
     HT_DWORD = 428,
     HT_EXTERN = 429,
     HT_FALSE = 430,
     HT_FXGROUP = 431,
     HT_GEOMETRYSHADER = 432,
     HT_GROUPSHARED = 433,
     HT_HALF = 434,
     HT_HULLSHADER = 435,
     HT_INLINE = 436,
     HT_INPUTPATCH = 437,
     HT_INTERFACE = 438,
     HT_LINE = 439,
     HT_LINEADJ = 440,
     HT_LINEAR = 441,
     HT_LINESTREAM = 442,
     HT_MATRIX = 443,
     HT_NAMESPACE = 444,
     HT_NOINTERPOLATION = 445,
     HT_NULL = 446,
     HT_OUTPUTPATCH = 447,
     HT_PACKOFFSET = 448,
     HT_PASS = 449,
     HT_PIXELFRAGMENT = 450,
     HT_PIXELSHADER = 451,
     HT_POINT = 452,
     HT_POINTSTREAM = 453,
     HT_PRECISE = 454,
     HT_RASTERIZERSTATE = 455,
     HT_RENDERTARGETVIEW = 456,
     HT_REGISTER = 457,
     HT_ROW_MAJOR = 458,
     HT_RWBUFFER = 459,
     HT_RWBYTEADDRESSBUFFER = 460,
     HT_RWSTRUCTUREDBUFFER = 461,
     HT_RWTEXTURE1D = 462,
     HT_RWTEXTURE1DARRAY = 463,
     HT_RWTEXTURE2D = 464,
     HT_RWTEXTURE2DARRAY = 465,
     HT_RWTEXTURE3D = 466,
     HT_SAMPLER = 467,
     HT_SAMPLER_STATE = 468,
     HT_SAMPLERSTATE = 469,
     HT_SAMPLERCOMPARISONSTATE = 470,
     HT_SHARED = 471,
     HT_SNORM = 472,
     HT_STATEBLOCK = 473,
     HT_STATEBLOCK_STATE = 474,
     HT_STATIC = 475,
     HT_STRING = 476,
     HT_STRUCTUREDBUFFER = 477,
     HT_TBUFFER = 478,
     HT_TECHNIQUE = 479,
     HT_TECHNIQUE10 = 480,
     HT_TECHNIQUE11XZ = 481,
     HT_TEXTURE = 482,
     HT_TEXTURE1D = 483,
     HT_TEXTURE1DARRAY = 484,
     HT_TEXTURE2D = 485,
     HT_TEXTURE2DARRAY = 486,
     HT_TEXTURE2DMS = 487,
     HT_TEXTURE2DMSARRAY = 488,
     HT_TEXTURE3D = 489,
     HT_TEXTURECUBE = 490,
     HT_TEXTURECUBEARRAY = 491,
     HT_TRUE = 492,
     HT_TYPEDEF = 493,
     HT_TRIANGLE = 494,
     HT_TRIANGLEADJ = 495,
     HT_TRIANGLESTREAM = 496,
     HT_UNORM = 497,
     HT_VECTOR = 498,
     HT_VERTEXFRAGMENT = 499,
     HT_VERTEXSHADER = 500,
     HT_VOLATILE = 501,
     HT_BOOL1 = 502,
     HT_BOOL1x1 = 503,
     HT_BOOL2x1 = 504,
     HT_BOOL3x1 = 505,
     HT_BOOL4x1 = 506,
     HT_BOOL2 = 507,
     HT_BOOL1x2 = 508,
     HT_BOOL2x2 = 509,
     HT_BOOL3x2 = 510,
     HT_BOOL4x2 = 511,
     HT_BOOL3 = 512,
     HT_BOOL1x3 = 513,
     HT_BOOL2x3 = 514,
     HT_BOOL3x3 = 515,
     HT_BOOL4x3 = 516,
     HT_BOOL4 = 517,
     HT_BOOL1x4 = 518,
     HT_BOOL2x4 = 519,
     HT_BOOL3x4 = 520,
     HT_BOOL4x4 = 521,
     HT_FLOAT1 = 522,
     HT_FLOAT1x1 = 523,
     HT_FLOAT2x1 = 524,
     HT_FLOAT3x1 = 525,
     HT_FLOAT4x1 = 526,
     HT_FLOAT2 = 527,
     HT_FLOAT1x2 = 528,
     HT_FLOAT2x2 = 529,
     HT_FLOAT3x2 = 530,
     HT_FLOAT4x2 = 531,
     HT_FLOAT3 = 532,
     HT_FLOAT1x3 = 533,
     HT_FLOAT2x3 = 534,
     HT_FLOAT3x3 = 535,
     HT_FLOAT4x3 = 536,
     HT_FLOAT4 = 537,
     HT_FLOAT1x4 = 538,
     HT_FLOAT2x4 = 539,
     HT_FLOAT3x4 = 540,
     HT_FLOAT4x4 = 541,
     HT_DOUBLE1 = 542,
     HT_DOUBLE1x1 = 543,
     HT_DOUBLE2x1 = 544,
     HT_DOUBLE3x1 = 545,
     HT_DOUBLE4x1 = 546,
     HT_DOUBLE2 = 547,
     HT_DOUBLE1x2 = 548,
     HT_DOUBLE2x2 = 549,
     HT_DOUBLE3x2 = 550,
     HT_DOUBLE4x2 = 551,
     HT_DOUBLE3 = 552,
     HT_DOUBLE1x3 = 553,
     HT_DOUBLE2x3 = 554,
     HT_DOUBLE3x3 = 555,
     HT_DOUBLE4x3 = 556,
     HT_DOUBLE4 = 557,
     HT_DOUBLE1x4 = 558,
     HT_DOUBLE2x4 = 559,
     HT_DOUBLE3x4 = 560,
     HT_DOUBLE4x4 = 561,
     HT_DWORD1 = 562,
     HT_DWORD1x1 = 563,
     HT_DWORD2x1 = 564,
     HT_DWORD3x1 = 565,
     HT_DWORD4x1 = 566,
     HT_DWORD2 = 567,
     HT_DWORD1x2 = 568,
     HT_DWORD2x2 = 569,
     HT_DWORD3x2 = 570,
     HT_DWORD4x2 = 571,
     HT_DWORD3 = 572,
     HT_DWORD1x3 = 573,
     HT_DWORD2x3 = 574,
     HT_DWORD3x3 = 575,
     HT_DWORD4x3 = 576,
     HT_DWORD4 = 577,
     HT_DWORD1x4 = 578,
     HT_DWORD2x4 = 579,
     HT_DWORD3x4 = 580,
     HT_DWORD4x4 = 581,
     HT_INT1 = 582,
     HT_INT1x1 = 583,
     HT_INT2x1 = 584,
     HT_INT3x1 = 585,
     HT_INT4x1 = 586,
     HT_INT2 = 587,
     HT_INT1x2 = 588,
     HT_INT2x2 = 589,
     HT_INT3x2 = 590,
     HT_INT4x2 = 591,
     HT_INT3 = 592,
     HT_INT1x3 = 593,
     HT_INT2x3 = 594,
     HT_INT3x3 = 595,
     HT_INT4x3 = 596,
     HT_INT4 = 597,
     HT_INT1x4 = 598,
     HT_INT2x4 = 599,
     HT_INT3x4 = 600,
     HT_INT4x4 = 601,
     HT_UINT1 = 602,
     HT_UINT1x1 = 603,
     HT_UINT2x1 = 604,
     HT_UINT3x1 = 605,
     HT_UINT4x1 = 606,
     HT_UINT2 = 607,
     HT_UINT1x2 = 608,
     HT_UINT2x2 = 609,
     HT_UINT3x2 = 610,
     HT_UINT4x2 = 611,
     HT_UINT3 = 612,
     HT_UINT1x3 = 613,
     HT_UINT2x3 = 614,
     HT_UINT3x3 = 615,
     HT_UINT4x3 = 616,
     HT_UINT4 = 617,
     HT_UINT1x4 = 618,
     HT_UINT2x4 = 619,
     HT_UINT3x4 = 620,
     HT_UINT4x4 = 621,
     PREPROCESSOR_DEFINE = 622,
     PREPROCESSOR_ELIF = 623,
     PREPROCESSOR_ELSE = 624,
     PREPROCESSOR_ENDIF = 625,
     PREPROCESSOR_ERROR = 626,
     PREPROCESSOR_IF = 627,
     PREPROCESSOR_IFDEF = 628,
     PREPROCESSOR_IFNDEF = 629,
     PREPROCESSOR_INCLUDE = 630,
     PREPROCESSOR_LINE = 631,
     PREPROCESSOR_PRAGMA = 632,
     PREPROCESSOR_UNDEF = 633,
     PREPROCESSOR_REGION = 634,
     PRAGMA_LOLFX = 635,
     HT_AUTO = 636,
     HT_CATCH = 637,
     HT_CHAR = 638,
     HT_CLASS = 639,
     HT_CONST_CAST = 640,
     HT_DELETE = 641,
     HT_DYNAMIC_CAST = 642,
     HT_ENUM = 643,
     HT_EXPLICIT = 644,
     HT_FRIEND = 645,
     HT_GOTO = 646,
     HT_LONG = 647,
     HT_MUTABLE = 648,
     HT_NEW = 649,
     HT_OPERATOR = 650,
     HT_PRIVATE = 651,
     HT_PROTECTED = 652,
     HT_PUBLIC = 653,
     HT_REINTERPRET_CAST = 654,
     HT_SHORT = 655,
     HT_SIGNED = 656,
     HT_SIZEOF = 657,
     HT_STATIC_CAST = 658,
     HT_TEMPLATE = 659,
     HT_THIS = 660,
     HT_THROW = 661,
     HT_TRY = 662,
     HT_TYPENAME = 663,
     HT_UNION = 664,
     HT_UNSIGNED = 665,
     HT_USING = 666,
     HT_VIRTUAL = 667,
     T_INC = 668,
     T_DEC = 669,
     T_LE = 670,
     T_GE = 671,
     T_EQ = 672,
     T_NE = 673,
     T_LEFT = 674,
     T_RIGHT = 675,
     T_AND = 676,
     T_OR = 677,
     T_XOR = 678,
     T_MULEQ = 679,
     T_DIVEQ = 680,
     T_MODEQ = 681,
     T_ADDEQ = 682,
     T_SUBEQ = 683,
     T_LEFTEQ = 684,
     T_RIGHTEQ = 685,
     T_ANDEQ = 686,
     T_XOREQ = 687,
     T_OREQ = 688,
     FLOAT = 689,
     STRING = 690,
     NAME = 691,
     T_ERROR = 692
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    LolFxParser (class LolFxCompiler& mc_yyarg);
    virtual ~LolFxParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    /// Internal symbol numbers.
    typedef unsigned short int token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const short int yypact_ninf_;

    /// For a state, default reduction number.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned short int yydefact_[];

    static const short int yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const short int yytable_[];
    static const short int yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned short int yystos_[];

    /// For a rule, its LHS.
    static const unsigned short int yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);

#if YYDEBUG
    /// A type to store symbol numbers and -1.
    typedef short int rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    /* User arguments.  */
    class LolFxCompiler& mc;
  };

} // lol

/* Line 35 of lalr1.cc  */
#line 717 "generated/lolfx-parser.h"



#endif /* ! defined PARSER_HEADER_H */
