%{

/*
* Parser.y file
* To generate the parser run: "bison Parser.y"
*/

#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"
int yyerror(expression **exp, yyscan_t scanner, const char *msg) {
  throw 1;
}

%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

#endif

}

%output  "Parser.cc"
%defines "Parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { expression **exp }
%parse-param { yyscan_t scanner }

%union {
  expression *exp;
}

%token TOKEN_INSERT TOKEN_DELETE TOKEN_FIND TOKEN_SHOW TOKEN_STATUS TOKEN_ALL TOKEN_WS TOKEN_CLIENT_DETAILS
%token <exp> TOKEN_INT TOKEN_FLOAT TOKEN_STRING

%type <exp> expr operand
%%

input
    : expr TOKEN_WS { *exp = $1;  }
    ;

operand
    : TOKEN_INT { $$ = $1; }
    | TOKEN_FLOAT { $$ = $1; }
    | TOKEN_STRING { $$ = $1; }

expr
    : TOKEN_INSERT TOKEN_WS operand { $$ = new expression(eINSERT, $3); }
    | TOKEN_DELETE TOKEN_WS operand { $$ = new expression(eDELETE, $3); }
    | TOKEN_FIND TOKEN_WS operand { $$ = new expression(eFIND, $3); }
    | TOKEN_SHOW TOKEN_WS TOKEN_STATUS { $$ = new expression(eSHOW, new expression(eSTATUS)); }
    | TOKEN_SHOW TOKEN_WS TOKEN_CLIENT_DETAILS { $$ = new expression(eSHOW, new expression(eCLIENTDETAILS)); }
    | TOKEN_DELETE TOKEN_WS TOKEN_ALL { $$ = new expression(eDELETE, new expression(eALL)); }
    ;
%%
