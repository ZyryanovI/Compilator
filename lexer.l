%{
    #include <stdio.h>
%}

identifier      [_a-zA-Z][_a-zA-Z0-9]*
operation       ( "&&" | "<" | "+" | "-" | "*" )
integer         ([1-9][0-9]* | "0")

%%
\n\r|\r\n|\n|\r { /* skip empty line */ }

"class"         printf("CLASS ")

"public"        printf("PUBLIC ")
"static"        printf("STATIC ")
"void"          printf("VOID ")
"main"          printf("MAIN ")
"extends"       printf("EXTENDS ")
"return"        printf("RETURN ")

"String"        printf("STRING ")
"int"           printf("INT ")
"boolean"       printf("BOOL ")

";"             printf("SEMI ")
"="             printf("ATTR ")
"."             printf("DOT ")
"!"             printf("NOT ")

"("             printf("L_BRACE ")
")"             printf("R_BRACE ")
"["             printf("LSQ_BRACE ")
"]"             printf("RSQ_BRACE ")
"{"             printf("LF_BRACE ")
"}"             printf("RF_BRACE ")

"if"            printf("IF ")
"else"          printf("ELSE ")
"while"         printf("WHILE ")
"System.out.println"    printf("OUT ")

"length"        printf("LEN ")
"this"          printf("THIS ")
"new"           printf("NEW ")

"true"          printf("TRUE ")
"false"         printf("FALSE ")

.               printf("ERROR ")

{identifier}    printf("ID ")
{integer}       printf("INT ")
%%