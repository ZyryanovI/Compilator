%{
#include <iostream>
#include "mini_scanner.h"
%}

%option c++
%option noyywrap
%option yyclass="MiniScanner"

%{
    int lexem_iterator = 0;
%}

LEX_COMMENT 		\/\/.*|\/\*((.|\n)*?)\*\/
LEX_SPACE			[\n\r\t ]+

LEX_CLASS 			"class"

LEX_PIBLIC			"public"
LEX_STATIC			"static"
LEX_VOID			"void"
LEX_MAIN 			"main"
LEX_EXTENDS 		"extends"
LEX_RETURN 			"return"

LEX_STRING 			"String"
LEX_INT 			"int"
LEX_BOOL 			"boolean"

LEX_SEMI			";"
LEX_ASSIGN			"="
LEX_DOT				"."

LEX_NOT				"!"
LEX_AND 			"&&"

LEX_L_BRACE 		"("
LEX_R_BRACE 		")"
LEX_LSQ_BRACE 		"["
LEX_RSQ_BRACE  		"]"
LEX_LF_BRACE 		"{"
LEX_RF_BRACE 		"}"

LEX_IF 				"if"
LEX_ELSE 			"else"
LEX_WHILE 			"while"
LEX_OUT 			"System.out.println"

LEX_LEN 			"length"
LEX_THIS 			"this"
LEX_NEW 			"new"

LEX_LOGIC_LIT 		"True" | "False"
LEX_DIGIT       [0-9]
LEX_LETTER      [a-zA-Z]
LEX_ID          {LETTER}({DIGIT}|{LETTER})*
LEX_INTEGER     [1-9]{DIGIT}*|0
LEX_OPERATION   "&&" | "<" | "+" | "-" | "*"

%%

{LEX_INTEGER} {
    handleToken("INTEGER", lexem_iterator);
}

{LEX_LOGIC_LIT} {
    handleToken("LOGIC_LITERAL", lexem_iterator);
}

{LEX_CLASS} {
    handleToken("CLASS", lexem_iterator);
}

{LEX_PUBLIC} {
    handleToken("PUBLIC", lexem_iterator);
}

{LEX_STATIC} {
    handleToken("STATIC", lexem_iterator);
}

{LEX_VOID} {
    handleToken("VOID", lexem_iterator);
}

{LEX_MAIN} {
    handleToken("MAIN", lexem_iterator);
}

{LEX_EXTENDS} {
    handleToken("EXTENDS", lexem_iterator);
}

{LEX_RETURN} {
    handleToken("RETURN", lexem_iterator);
}

{LEX_IF} {
    handleToken("IF", lexem_iterator);
}

{LEX_ELSE} {
    handleToken("ELSE", lexem_iterator);
}

{LEX_WHILE} {
    handleToken("WHILE", lexem_iterator);
}

{LEX_OUT} {
    handleToken("OUT", lexem_iterator);
}

{LEX_LEN} {
    handleToken("LEN", lexem_iterator);
}

{LEX_NEW} {
    handleToken("NEW", lexem_iterator);
}

{LEX_THIS} {
    handleToken("THIS", lexem_iterator);
}

{LEX_STRING} {
    handleToken("STRING", lexem_iterator);
}

{LEX_BOOL} {
    handleToken("BOOL", lexem_iterator);
}

{LEX_INT} {
    handleToken("INT", lexem_iterator);
}

{LEX_AND} {
    handleToken("AND", lexem_iterator);
}

{LEX_ID} {
    handleToken("ID(" + std::string(yytext) +")", lexem_iterator);
}

{LEX_SPACE} {
    handleToken("", lexem_iterator);
}

{LEX_COMMENT} {
    handleToken("COMMENT", lexem_iterator);
}

{LEX_OPERATORS} { handleToken(std::string(1, yytext[0]), lexem_iterator); }

.  { std::cout << std::endl << "UNKNOWN  " << yytext << std::endl; }

%%
