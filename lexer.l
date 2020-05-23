%option noyywrap
%option outfile="parser.lexer.c" header-file="parser.lexer.h"

%{
#include <iostream>
#include "parser.tab.h"

yy::location loc;

#define YY_DECL yy::parser::symbol_type yylex()
#define YY_USER_ACTION loc.columns(yyleng);
%}

DIGIT		[0-9]
NUM 		({DIGIT})+
CHAR		[a-zA-Z_]
STRING 		({CHAR})+
ID			{STRING}({STRING}|{NUM})*
COMMENT 	"//".*\n


%%
" " 		{ loc.step(); }
\t 			{ loc.step(); }
\n 			{ loc.lines(yyleng); loc.step(); }
{COMMENT} 	{ loc.step(); }


"+" 		{ return yy::parser::make_PLUS(loc); }
"-" 		{ return yy::parser::make_MINUS(loc);}
"*" 		{ return yy::parser::make_MULT(loc);}
"&&" 		{ return yy::parser::make_AND(loc);}
"<" 		{ return yy::parser::make_LESS(loc);}
"!" 		{ return yy::parser::make_NOT(loc);}

"(" 		{ return yy::parser::make_R_LEFT(loc);}
")" 		{ return yy::parser::make_R_RIGHT(loc);}
"{" 		{ return yy::parser::make_F_LEFT(loc);}
"}" 		{ return yy::parser::make_F_RIGHT(loc);}
"[" 		{ return yy::parser::make_Q_LEFT(loc);}
"]" 		{ return yy::parser::make_Q_RIGHT(loc);}
"." 		{ return yy::parser::make_DOT(loc);}
"," 		{ return yy::parser::make_COMMA(loc);}
";" 		{ return yy::parser::make_SCOLON(loc);}
"=" 		{ return yy::parser::make_EQ(loc);}

"int" 		{ return yy::parser::make_INT(loc);}
"boolean" 	{ return yy::parser::make_BOOL(loc);}
"String" 	{ return yy::parser::make_STRING(loc);}

"true" 		{ return yy::parser::make_TRUE(loc);}
"false" 	{ return yy::parser::make_FALSE(loc);}

"this" 		{ return yy::parser::make_THIS(loc);}
"new" 		{ return yy::parser::make_NEW(loc);}
"class" 	{ return yy::parser::make_CLASS(loc);}
"public" 	{ return yy::parser::make_PUBLIC(loc);}
"static" 	{ return yy::parser::make_STATIC(loc);}
"void" 		{ return yy::parser::make_VOID(loc);}
"extends" 	{ return yy::parser::make_EXTENDS(loc);}
"main" 		{ return yy::parser::make_MAIN(loc);}
"return" 	{ return yy::parser::make_RETURN(loc);}

"if" 		{ return yy::parser::make_IF(loc);}
"else" 		{ return yy::parser::make_ELSE(loc);}
"while" 	{ return yy::parser::make_WHILE(loc);}

"System.out.println" 	{ return yy::parser::make_PRINT(loc);}
".length" 	{ return yy::parser::make_LENGTH(loc);}

{ ID } 		{return yy::parser::make_ID(std::string(yytext), loc);}
{ NUM } 	{return yy::parser::make_NUM(atoi(yytext), loc);}

<<EOF>> 	{ return yy::parser::make_END(loc); }
. 			{ throw yy::parser::syntax_error(loc, "Invalid syntax: " + std::string(yytext)); }
%%
