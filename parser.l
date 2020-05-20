%option noyywrap
%option outfile="parser.lexer.c" header-file="parser.lexer.h"

%{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <string>
#include <iostream>
#include "parser.tab.h"
using namespace std;

void update_pos() {
	yylloc.first_column = yylloc.last_column;
	yylloc.last_column += yyleng;
}

//size_t column = 1;
//#define YY_USER_ACTION yylloc->first_line = yylloc->last_line = yylineno; \
//    yylloc->first_column = column; yylloc->last_column = column + yyleng; \
//    column += yyleng;

%}

DIGIT [0-9]
NUM ({DIGIT})+
CHAR [a-zA-Z_]
STRING ({CHAR})+
IDENT {STRING}({STRING}|{NUM})*
COMMENT "//".*\n


%%
" " {update_pos();}
\t {update_pos();}
\n {yylloc.first_line++; yylloc.last_line++; yylloc.first_column=1; yylloc.last_column=1;}
{COMMENT} {yylloc.first_line++; yylloc.last_line++; yylloc.first_column=1; yylloc.last_column=1;} // Ignore comments


"+" {update_pos(); return T_PLUS;}
"-" {update_pos(); return T_MINUS;}
"*" {update_pos(); return T_MULT;}
"%" {update_pos(); return T_REMAIN;}
"&&" {update_pos(); return T_AND;}
"||" {update_pos(); return T_OR;}
"<" {update_pos(); return T_LESS;}
"!" {update_pos(); return T_NOT;}

"(" {update_pos(); return T_R_LEFT;}
")" {update_pos(); return T_R_RIGHT;}
"{" {update_pos(); return T_F_LEFT;}
"}" {update_pos(); return T_F_RIGHT;}
"[" {update_pos(); return T_Q_LEFT;}
"]" {update_pos(); return T_Q_RIGHT;}
"." {update_pos(); return T_DOT;}
"," {update_pos(); return T_COMMA;}
";" {update_pos(); return T_SCOLON;}
"=" {update_pos(); return T_EQ;}

"int" {update_pos(); return T_INT;}
"boolean" {update_pos(); return T_BOOL;}
"String" {update_pos(); return T_STRING;}

"true" {update_pos(); return T_TRUE;}
"false" {update_pos(); return T_FALSE;}

"this" {update_pos(); return T_THIS;}
"new" {update_pos(); return T_NEW;}
"class" {update_pos(); return T_CLASS;}
"public" {update_pos(); return T_PUBLIC;}
"private" {update_pos(); return T_PRIVATE;}
"static" {update_pos(); return T_STATIC;}
"void" {update_pos(); return T_VOID;}
"extends" {update_pos(); return T_EXTENDS;}
"main" {update_pos(); return T_MAIN;}
"return" {update_pos(); return T_RETURN;}

"if" {update_pos(); return T_IF;}
"else" {update_pos(); return T_ELSE;}
"while" {update_pos(); return T_WHILE;}

"System.out.println" {update_pos(); return T_PRINT;}
".length" {update_pos(); return T_LENGTH;}

{IDENT} {yylval.sval = strdup(yytext); update_pos(); return T_IDENT;}
{NUM} {yylval.ival = atoi(yytext); update_pos(); return T_NUM;}

. {cout << "Invalid syntax: " << yytext << " at line " << yylloc.first_line << ", column " << yylloc.last_column << "\n"; throw std::exception();}

%%

#pragma clang diagnostic pop
