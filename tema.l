%{
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "y.tab.h"
%}
%option noyywrap
%%
"int"|"float"|"char"|"string"|"bool"|"const int"|"const float"|"const char"|"const string"|"const bool" {yylval.strVal=strdup(yytext); return TIP;}
"begin" {return BEGIN_PROGRAM;}
"end" {return END_PROGRAM;}
@[_a-zA-Z][_a-zA-Z0-9]* {yylval.strVal=strdup(yytext); return ID;}
"=>" {return ASSIGN;}
[0-9]+ {yylval.intVal=atoi(yytext); return INT;}
[0-9]+\.[0-9]+ {yylval.floatVal=atof(yytext); return FLOAT;}
\"[a-zA-Z0-9]+\" {yylval.strVal=strdup(yytext+1); yylval.strVal[strlen(yylval.strVal)-1]='\0'; return STRING;}
\'[a-zA-Z0-9]\' {yylval.charVal=yytext[1]; return CHAR;}
"true"|"True"|"false"|"False" {
    if(strcmp(yytext, "true") == 0 || strcmp(yytext, "True") == 0) {
        yylval.boolVal = true;
    } else {
        yylval.boolVal = false;
    }
    return BOOL;
}
[ \t] ;
\n {yylineno++;}
"return" {return RETURN;}
"struct" {return STRUCT;}
"class" {return CLASS;}
"public" {return PUBLIC;}
"privat" {return PRIVAT;}
"TypeOf" {return TYPEOF;}
"Eval" {return EVAL;}
"->" {return MEMBER;}
"==" {return EQ;}
"<" {return LT;}
">" {return GT;}
"<=" {return LTEQ;}
">=" {return GTEQ;}
"!=" {return EQN;}
"if" {return IF;}
"for" {return FOR;}
"then" {return THEN;}
"else" {return ELSE;}
"while" {return WHILE;}
"do" {return DO;}
"&&" {return AND;}
"||" {return OR;}
"+" {return PLUS;}
"-" {return MINUS;}
"*" {return MULT;}
"/" {return DIV;}
"%" {return MOD;}
. {return yytext[0];}
