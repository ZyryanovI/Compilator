%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {MiniJava}
%define api.parser.class {MiniJava_Parser}

%code requires{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   #include <string>


   namespace MiniJava {
      class MiniJava_Lexer;
   }

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { MiniJava_Lexer  &lexer  }

%code{
   #include <memory>
   #include "MiniJava.Lexer.hpp"

   MiniJava::YYLTYPE LLCAST(MiniJava::location x) { return MiniJava::YYLTYPE({(x.begin.line), (x.begin.column), (x.end.line), (x.end.column)} ); }

#undef yylex
}

%define api.value.type variant
%define parse.assert

///////////

%token<int> INTEGER
%token<std::string> LOGICAL_LIT
%token<std::string> OPERATION
%token<std::string> ID
%token PUBLIC
%token EXTENDS
%token RETURN
%token INT
%token BOOL
%token ASSIGN
%token SEMI
%token L_BRACE
%token R_BRACE
%token LSQ_BRACE
%token RSQ_BRACE
%token LF_BRACE 
%token RF_BRACE 
%token DOT
%token LEN
%token NOT
%token THIS
%token MAIN
%token CLASS
%token VOID
%token IF
%token ELSE
%token WHILE
%token STRING
%token STATIC
%token END 0 "end of file"
%token NEW
%token COMMA

%type<ast::PExpression> expr
%type<ast::PStatement> statement
%type<ast::PType> type
%type<ast::PMainClass> main_class
%type<ast::PVarDeclaration> var_declaration
%type<ast::PMethodBody> method_body
%type<ast::PMethodDeclaration> method_declaration
%type<ast::PGoal> goal

%type <std::vector<ast::PExpression>> expressions
%type <std::vector<ast::PStatement>> statements
%type <std::vector<ast::PVarDeclaration>> var_declarations
%type <std::vector<ast::PMethodDeclaration>> method_declarations
%type <std::vector<ast::PClass>> classes
%type <std::vector<std::pair<ast::PType, std::string>>> method_args
%locations

%left ASSIGN

%left OPERATION
%right UNARY_NEGATION
%right NEW

%right L_BRACE
%right LSQ_BRACE
%right LF_BRACE
%left IF
%left ELSE
%left COMMA
%left DOT
%left EXTENDS
%left R_BRACE
%left RSQ_BRACE
%left RF_BRACE

%start goal

%%

goal
      : main_class classes {$$ = std::make_shared<ast::Goal>($1, $2, LLCAST(@$)); *root = $$;}

main_class
      : CLASS ID LF_BRACE PUBLIC STATIC VOID MAIN L_BRACE STRING LSQ_BRACE RSQ_BRACE ID R_BRACE LF_BRACE statement RF_BRACE RF_BRACE
        {$$ = std::make_shared<ast::MainClass>($2, $4, $12, $15, LLCAST(@$));}

classes
      : classes class { $1.push_back($2); $$ = $1; }
      | {std::vector<ast::PClass> array; $$ = array;}


class
      : CLASS ID LF_BRACE var_declarations method_declarations RF_BRACE
          {$$ = std::make_shared<ast::Class>($2, $4, $5, LLCAST(@$));}
      | CLASS ID EXTENDS ID LF_BRACE var_declarations method_declarations RF_BRACE
          {$$ = std::make_shared<ast::Class>($2, $4, $6, $7, LLCAST(@$));}

method_args
      : type ID COMMA method_args
          {$4.push_back(std::make_pair($1, $2)); $$ = $4;}
      | type ID
          {std::vector<std::pair<ast::PType, std::string>> array; array.push_back(std::make_pair($1, $2)); $$ = array;}


method_declarations
      : method_declarations method_declaration
          { $1.push_back($2); $$ = $1; }
      | {std::vector<ast::PMethodDeclaration> array; $$ = array;}


method_declaration
      : PUBLIC type ID L_BRACE method_args R_BRACE method_body
          {$$ = std::make_shared<ast::MethodDeclaration>($1, $2, $3, $5, $7, LLCAST(@$));}
      | PUBLIC type ID L_BRACE R_BRACE method_body
          {$$ = std::make_shared<ast::MethodDeclaration>($1, $2, $3, std::vector<std::pair<ast::PType, std::string>>(), $6, LLCAST(@$));}


method_body
      : LF_BRACE var_declarations statements RETURN expr SEMI RF_BRACE
          {$$ = std::make_shared<ast::MethodBody>($2, $3, $5, LLCAST(@$));}

var_declaration
      : type ID SEMI {$$ = std::make_shared<ast::VarDeclaration>($1, $2, LLCAST(@$));}

var_declarations
      : var_declarations var_declaration { $1.push_back($2); $$ = $1; }
      | { std::vector<ast::PVarDeclaration> array; $$ = array; }

type
      : INT {$$ = std::make_shared<ast::TypeInt>(LLCAST(@$)); }
      | BOOL {$$ = std::make_shared<ast::TypeBoolean>(LLCAST(@$)); }
      | INT LSQ_BRACE RF_BRACE {$$ = std::make_shared<ast::TypeArray>(LLCAST(@$)); }
      | ID {$$ = std::make_shared<ast::TypeClass>($1, LLCAST(@$)); }

statements
      : statement statements{ $2.push_back($1); $$ = $2; }
      | {std::vector<ast::PStatement> array; $$ = array;}

statement
      : ID ASSIGN expr SEMI
          {$$ = std::make_shared<ast::StatementAssign>($1, $3, LLCAST(@$));}
      | LF_BRACE statements LF_BRACE
          {$$ = std::make_shared<ast::Statements>($2, LLCAST(@$));}

      | IF L_BRACE expr R_BRACE statement ELSE statement
          {$$ = std::make_shared<ast::StatementIf>($3, $5, $7, LLCAST(@$));}

      | WHILE L_BRACE expr R_BRACE statement
          {$$ = std::make_shared<ast::StatementWhile>($3, $5, LLCAST(@$));}

      | PRINT L_BRACE expr L_BRACE SEMI
          {$$ = std::make_shared<ast::StatementPrint>($3, LLCAST(@$));}

      | ID LSQ_BRACE expr RSQ_BRACE ASSIGN expr SEMI
          {$$ = std::make_shared<ast::StatementArrayAssign>($1, $3, $6, LLCAST(@$));}

expressions
    : expressions COMMA expr { $1.push_back($3); $$ = $1;}
    | expr {std::vector<ast::PExpression> array; array.push_back($1), $$ = array;}

expr
    : INTEGER
        { $$ = std::make_shared<ast::ExpressionInt>    ($1, LLCAST(@$)); }
    | LOGICAL_LIT
        { $$ = std::make_shared<ast::ExpressionLogical>($1, LLCAST(@$)); }
    | NEW INT LSQ_BRACE expr RF_BRACE
        { $$ = std::make_shared<ast::ExpressionNewIntArray>($4, LLCAST(@$)); }
    | ID
        { $$ = std::make_shared<ast::ExpressionId>     ($1, LLCAST(@$)); }
    | NEW ID LBRACKET RBRACKET
        { $$ = std::make_shared<ast::ExpressionNewId>  ($2, LLCAST(@$)); }

    | expr DOT IDENTIFIER L_BRACE  R_BRACE
        { $$ = std::make_shared<ast::ExpressionCallFunction> ($1, $3, std::vector<ast::PExpression>(), LLCAST(@$)); }

    | expr DOT ID L_BRACE expressions R_BRACE
        { $$ = std::make_shared<ast::ExpressionCallFunction> ($1, $3, $5, LLCAST(@$)); }

    | L_BRACE expr R_BRACE { $$ = $2; }

    | expr OPERATION expr
        { $$ = std::make_shared<ast::ExpressionBinaryOp>($1, $3, $2, LLCAST(@$));}
    | expr LSQ_BRACE expr RF_BRACE
        { $$ = std::make_shared<ast::ExpressionSquareBracket>($1, $3, LLCAST(@$)); }
    | expr DOT LEN
        { $$ = std::make_shared<ast::ExpressionLen>($1, LLCAST(@$)); }
    | NOT expr
        { $$ = std::make_shared<ast::ExpressionNot>($2, LLCAST(@$)); }
    | THIS
        { $$ = std::make_shared<ast::ExpressionThis>(LLCAST(@$)); }

%%

void
MiniJava::MiniJava_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}