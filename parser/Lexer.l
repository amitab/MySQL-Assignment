%{

/*
* Lexer.l file
* To generate the lexical analyzer run: "flex Lexer.l"
*/

#include "Expression.h"
#include "Parser.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

%}

%option outfile="Lexer.cc" header-file="Lexer.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

ADMIN               ^ADMIN

INSERT              ^INSERT
DELETE              ^DELETE
FIND                ^FIND
SHOW                ^SHOW
ALL                 ALL$
STATUS              STATUS$
CLIENT_DETAILS      "CLIENT DETAILS"$

STRING              \"[A-Za-z0-9" "]+\"$
FLOAT               ([1-9][0-9]+|[0-9])\.[0-9]+$
INT                 [0-9]+$
WS                  [ \r\n\t]+

%%

{WS}                { return TOKEN_WS; }

{ADMIN}             { return TOKEN_ADMIN;  }
{INSERT}            { return TOKEN_INSERT; }
{DELETE}            { return TOKEN_DELETE; }
{FIND}              { return TOKEN_FIND; }
{SHOW}              { return TOKEN_SHOW; }
{ALL}               { return TOKEN_ALL; }
{STATUS}            { return TOKEN_STATUS; }
{CLIENT_DETAILS}    { return TOKEN_CLIENT_DETAILS; }

{FLOAT}             { yylval->exp = new expression(atof(yytext)); return TOKEN_FLOAT; }
{INT}               { yylval->exp = new expression(atoi(yytext)); return TOKEN_INT; }
{STRING}            { yylval->exp = new expression(yytext); return TOKEN_STRING; }
.                   {  }

%%

int yyerror(const char *msg) {
  fprintf(stderr,"Error:%s\n",msg); return 0;
}
