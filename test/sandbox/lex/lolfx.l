%{
#include <iostream>
#define YY_DECL extern "C" int yylex()
#include "lolfx.tab.h"
%}

%x C_COMMENT
%x CPP_COMMENT

%%

 /*
  * Our language's keywords
  */

"technique"    { return TECHNIQUE; }
"pass"         { return PASS; }

 /*
  * Various tokens
  */

[0-9]+\.[0-9]+ { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+         { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z][a-zA-Z0-9_]*  {
    /* Copy token for now */
    yylval.sval = strdup(yytext);
    return NAME;
}
[ \t\n]+       ;
.              { return *yytext; }

 /*
  * Ignore C comments
  */

"/*"              { BEGIN(C_COMMENT); }
<C_COMMENT>"*/"   { BEGIN(INITIAL); }
<C_COMMENT>[^*]*  { }
<C_COMMENT>.      { }

 /*
  * Ignore C++ comments
  */

"//"              { BEGIN(CPP_COMMENT); }
<CPP_COMMENT>.*\n { BEGIN(INITIAL); }

%%

