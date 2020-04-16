%option outfile="MiniJava.Lexer.cpp" header-file="MiniJava.Lexer.h"

%{
#include <string>
#include "src/mj_scanner.hpp"

#undef  YY_DECL
#define YY_DECL int MJ::MJ_Scanner::yylex( MJ::MJ_Parser::semantic_type * const lval, MJ::MJ_Parser::location_type *loc )

using token = MJ::MJ_Parser::token;

#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
#define yyterminate() return token::END
%}

%option debug
%option nodefault
%option yyclass="MJ::MJ_Scanner"
%option noyywrap
%option c++

LEX_CLASS      "class"
LEX_PRIVACY    "private"|"public"
LEX_STATIC     "static"
LEX_VOID       "void"
LEX_MAIN       "main"
LEX_EXTENDS    "extends"
LEX_RETURN     "return"
LEX_IF         "if"
LEX_ELSE       "else"
LEX_WHILE      "while"
LEX_PRINT       "System.out.println"
LEX_LENGTH     "length"
LEX_NEW        "new"
LEX_THIS       "this"
LEX_TRUE       "true"
LEX_FALSE      "false"
LEX_STRING     "String"
LEX_BOOLEAN    "boolean"
LEX_INT        "int"
LEX_DIGIT      [0-9]
LEX_LETTER     [a-zA-Z_]
LEX_INTEGER_LITERAL   [1-9]{LEX_DIGIT}*|0
LEX_LOGIC_LITERAL     {LEX_TRUE}|{LEX_FALSE}
LEX_ID         {LEX_LETTER}({LEX_DIGIT}|{LEX_LETTER})*
LEX_COMMENT    (\/\/.*)|(\/\*((.|\n)*?)\*\/)
LEX_SPACE      [\n\r\t ]+
LEX_OPERATORS  "+"|"-"|"*"|"/"|"%"|"=="|"&&"|"||"|"<"|">"
LEX_ASSIGN     "="

LEX_DOT_COMMA ";"
LEX_DOT "."
LEX_COMMA ","
LEX_LBRACKET "("
LEX_RBRACKET ")"
LEX_LSQUAREBRACKET "["
LEX_RSQUAREBRACKET "]"
LEX_LBRACE "{"
LEX_RBRACE "}"
LEX_UNARY_NEGATION "!"

%%

%{
  yylval = lval;
%}


{LEX_EXTENDS} {
    return token::EXTENDS;
}


{LEX_BOOLEAN} {
    return token::BOOLEAN;
}


{LEX_ELSE} {
    return token::ELSE;
}

{LEX_PRINT} {
    return token::PRINT;
}

{LEX_WHILE} {
    return token::WHILE;
}

{LEX_IF} {
    return token::IF;
}

{LEX_COMMA} {
    return token::COMMA;
}

{LEX_STATIC} {
    return token::STATIC;
}

{LEX_CLASS} {
    return token::CLASS;
}

{LEX_MAIN} {
    return token::MAIN;
}

{LEX_STRING} {
    return token::STRING;
}

{LEX_VOID} {
    return token::VOID;
}

{LEX_PRIVACY} {
    yylval->build<std::string>(yytext);
    return token::PRIVACY;
}

{LEX_RETURN} {
    return token::RETURN;
}

{LEX_INT} {
    return token::INT;
}


{LEX_DOT_COMMA} {
    return token::DOT_COMMA;
}


{LEX_ASSIGN} {
    return token::ASSIGN;
}


{LEX_INTEGER_LITERAL} {
    yylval->build<int>(atoi(yytext));
    return token::INTEGER_LITERAL;
}

{LEX_LOGIC_LITERAL} {
    yylval->build<std::string>(std::string(yytext));
    return token::LOGICAL_LITERAL;
}

{LEX_LENGTH} {
    return token::LENGTH;
}

{LEX_NEW} {
    return token::NEW;
}

{LEX_THIS} {
    return token::THIS;
}

{LEX_ID} {
    yylval->build<std::string>(yytext);
    return token::IDENTIFIER;
}


{LEX_COMMENT} {
// comments and spaces are ignored
}

{LEX_SPACE} {

}

{LEX_OPERATORS} {
	yylval->build<std::string>(yytext);
    return token::OPERATION_LITERAL;
}

{LEX_LBRACKET} {
    return token::LBRACKET;
}

{LEX_RBRACKET} {
    return token::RBRACKET;
}

{LEX_LSQUAREBRACKET} {
    return token::LSQUAREBRACKET;
}

{LEX_RSQUAREBRACKET} {
    return token::RSQUAREBRACKET;
}

{LEX_LBRACE} {
    return token::LBRACE;
}

{LEX_RBRACE} {
    return token::RBRACE;
}

{LEX_DOT} {
    return token::DOT;
}


{LEX_UNARY_NEGATION} {
    return token::UNARY_NEGATION;
}

<<EOF>>  {
    yyterminate();
    return 0;
}

.  { 
// for errors
std::cout << std::endl << "UNKNOWN  " << yytext << std::endl; 
}
%%
