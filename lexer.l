%option reentrant noyywrap never-interactive nounistd
%option bison-bridge
%option bison-locations

%option prefix="mj"
%option outfile="MiniJava.Lexer.cpp" header-file="MiniJava.Lexer.h"

DIGIT [0-9]
LETTER [a-zA-Z]
Id {LETTER}({DIGIT}|{LETTER})*
IntegerLiteral [1-9]{DIGIT}*|0
%% 
“class” printf( “CLASS ” );
“extends” printf( “EXTENDS ” );
{Id} printf( “ID (%*s) ”, yylen, yytext );

