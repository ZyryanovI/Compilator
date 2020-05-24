%option noyywrap
%option outfile="parser.lexer.c" header-file="parser.lexer.h"

%{
#include <string>
#include <iostream>
using namespace std;

#include "parser.tab.h"

void update_pos() {
	yylloc.first_column = yylloc.last_column;
	yylloc.last_column += yyleng;
}

void ignore_line() {
	yylloc.first_line++;
	yylloc.last_line++;
	yylloc.first_column = 1;
	yylloc.last_column = 1;
}

%}

DIGIT		[0-9]
NUMBER 		({DIGIT})+
CHAR		[a-zA-Z_]
STRING 		({CHAR})+
IDENT		{STRING}({STRING}|{NUMBER})*
COMMENT 	"//".*\n

%%
" " 		{ update_pos(); }
\t 			{ update_pos(); }
\n 			{ ignore_line(); }
{COMMENT} 	{ ignore_line(); }

"+" 		{ update_pos(); return PLUS; }
"-" 		{ update_pos(); return MINUS;}
"*" 		{ update_pos(); return MULT;}
"&&" 		{ update_pos(); return AND;}
"<" 		{ update_pos(); return LESS;}
"!" 		{ update_pos(); return NOT;}

"(" 		{ update_pos(); return R_LEFT;}
")" 		{ update_pos(); return R_RIGHT;}
"{" 		{ update_pos(); return F_LEFT;}
"}" 		{ update_pos(); return F_RIGHT;}
"[" 		{ update_pos(); return Q_LEFT;}
"]" 		{ update_pos(); return Q_RIGHT;}
"." 		{ update_pos(); return DOT;}
"," 		{ update_pos(); return COMMA;}
";" 		{ update_pos(); return SCOLON;}
"=" 		{ update_pos(); return EQ;}

"int" 		{ update_pos(); return INT;}
"boolean" 	{ update_pos(); return BOOL;}
"String" 	{ update_pos(); return STRING;}

"true" 		{ update_pos(); return TRUE;}
"false" 	{ update_pos(); return FALSE;}

"this" 		{ update_pos(); return THIS;}
"new" 		{ update_pos(); return NEW;}
"class" 	{ update_pos(); return CLASS;}
"public" 	{ update_pos(); return PUBLIC;}
"static" 	{ update_pos(); return STATIC;}
"void" 		{ update_pos(); return VOID;}
"extends" 	{ update_pos(); return EXTENDS;}
"main" 		{ update_pos(); return MAIN;}
"return" 	{ update_pos(); return RETURN;}

"if" 		{ update_pos(); return IF;}
"else" 		{ update_pos(); return ELSE;}
"while" 	{ update_pos(); return WHILE;}

"System.out.println" 	{ update_pos(); return PRINT;}
".length" 	{ update_pos(); return LENGTH;}

{IDENT} 	{ yylval.strval = strdup(yytext); update_pos(); return ID; }
{NUMBER} 	{ yylval.intval = atoi(yytext); update_pos(); return NUM; }

. {cout << "Invalid syntax: " << yytext << " at line " << yylloc.first_line << ", column " << yylloc.last_column << "\n"; throw std::exception();}

%%