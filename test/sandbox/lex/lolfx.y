%{
#include <cstdio>
#include <iostream>

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" int yylineno;

void yyerror(const char *s);
%}

/* The classic Bison union trick */
%union
{
    int ival;
    float fval;
    char *sval;
}

 /*
  * All the LolFx tokens
  */

%token TECHNIQUE PASS
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING NAME

%%

fx:
    section_list
    ;

section_list:
    section
    | section_list section
    ;

section:
    technique
    | shader
    ;

 /*
  * Grammar for techniques
  */

technique:
    TECHNIQUE NAME '{' pass_list '}' { std::cout << "TECHNIQUE" << std::endl; }
    ;

 /*
  * Grammar for passes
  */

pass_list:
    pass
    | pass_list pass
    ;

pass:
    PASS NAME '{' pass_stmt_list '}' { std::cout << "PASS" << std::endl; }
    ;

pass_stmt_list:
    pass_stmt
    | pass_stmt_list pass_stmt
    ;

pass_stmt:
    ';'
    | NAME '=' NAME ';'
    | NAME '=' INT ';'
    | NAME '[' INT ']' '=' NAME ';'
    | NAME '[' INT ']' '=' INT ';'
    ;

 /*
  * Grammar for shaders
  */

shader:
    shader_region shader_code
    | shader_region
    ;

shader_region:
    '#' NAME shader_name { std::cout << "new shader " << $2 << std::endl; }
    ;

shader_name:
    NAME
    | shader_name '.' NAME
    ;

shader_code:
    INT shader_code      { std::cout << "int: " << $1 << std::endl; }
    | FLOAT shader_code  { std::cout << "float: " << $1 << std::endl; }
    | STRING shader_code { std::cout << "string: " << $1 << std::endl; }
    | INT            { std::cout << "int: " << $1 << std::endl; }
    | FLOAT          { std::cout << "float: " << $1 << std::endl; }
    | STRING         { std::cout << "string: " << $1 << std::endl; }
    ;

%%

main()
{
    yyin = fopen("test.lolfx", "r");
    do
    {
        yyparse();
    }
    while (!feof(yyin));

    fclose(yyin);
}

void yyerror(const char *s)
{
    std::cout << "Parse error line " << yylineno << ": " << s << std::endl;
    exit(-1);
}

