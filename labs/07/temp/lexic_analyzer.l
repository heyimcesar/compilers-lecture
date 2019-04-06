%{
#include "y.tab.h"
%}
%%
\/\/(.*) {}
(0{1}|[1-9]+[0-9]*) {yylval = atoi(yytext); return INUM;}
(0{1}|[1-9]+[0-9]*)\.([0-9]*[1-9]+) {yylval = atof(yytext); return FNUM;}
[\t\n] return(END);
"+" return(PLUS);
"-" return(MINUS);
"*" return(TIMES);
"/" return(DIVIDE);
"p" return(PRINT);
"f" return(FLOATDCL);
"i" return(INTDCL);
"=" return(ASSIGN);
[a-eg-hj-oq-z]  return(ID);
%%
int yywrap (void)
{return 1;}