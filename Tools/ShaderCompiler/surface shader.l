
%{

#include "y.tab.h"
int yyerror(char *errormsg);


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

int main(void)
{
    yyparse();
    return 0;
}

int yywrap(void)
{
    return 0;
}

int yyerror(char *errormsg)
{
    fprintf(stderr, "%s\n", errormsg);
    return 1;
}