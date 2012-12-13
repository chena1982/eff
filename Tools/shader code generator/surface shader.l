%option noyywrap

%{

/* need this for the call to atof() below */

#include "stdio.h"


%}

digit        [0-9]
float       ("-"|"+")?{digit}+"."{digit}*"f"?
letter       [a-zA-Z]
identifier   ({letter}|_)({digit}|{letter}|_)*
string       \"[^"]*\"
newline      [\n]
whitespace   [ \t]+
comment      "//"

%%

 
{float}     { printf("%f\n", yytext); }
{identifier} { printf("%s\n", yytext);  }
{string}     { printf("%s\n", yytext);  }
{whitespace} {}
{newline}   {}
{comment} {}
. { printf("unknown.\n"); }

%%
