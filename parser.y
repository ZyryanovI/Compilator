%{
#include <cstdio>
#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <exception>
using namespace std;

#include "parser.tab.h"

extern int yylex();
extern int yyparse(Goal** goal);
extern FILE *yyin;

void yyerror(Goal** goal, const char* s);
%}

%code requires {
#include "AST/Interfaces.h"
#include "AST/Identifier.h"
#include "AST/Expressions.h"
#include "AST/Statements.h"
#include "AST/Types.h"
#include "AST/VarDeclaration.h"
#include "AST/MethodDeclaration.h"
#include "AST/ClassDeclaration.h"
#include "AST/MainClass.h"
#include "AST/Goal.h"
#include "SymbolTable/Symbol.h"
}

%parse-param {Goal** goal}
%define parse.error verbose
%locations

%left PLUS
%left MINUS
%left MULT
%left AND
%left LESS
%left NOT

%token R_LEFT
%token R_RIGHT
%token F_LEFT
%token F_RIGHT
%token Q_RIGHT
%token COMMA
%token SCOLON
%token EQ

%right Q_LEFT
%right DOT

%token INT
%token BOOL
%token STRING

%token TRUE
%token FALSE

%token THIS
%token NEW
%token CLASS
%token PUBLIC
%token STATIC
%token VOID
%token EXTENDS
%token MAIN
%token RETURN

%token IF
%token ELSE
%token WHILE

%token PRINT

%left LENGTH

%union {
	int intval;
	char* strval;

 	IIdentifier* idval;
	IExpression* expval;
	std::vector<std::unique_ptr<IExpression>>* expvals;
	IStatement* statval;
	std::vector<std::unique_ptr<IStatement>>* statvals;
	IType* typeval;
	IVarDeclaration* varDeclarval;
	IMethodDeclaration* methodDeclarval;
	std::vector<std::unique_ptr<IVarDeclaration>>* varDeclarvals;
	std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>* methodParamval;
	IClassDeclaration* classDeclarval;
	IIdentifier* extval;
	std::vector<std::unique_ptr<IMethodDeclaration>>* methodDeclarvals;
	std::vector<std::unique_ptr<IClassDeclaration>>* classDeclarvals;
	IMainClass* mainval;
	IGoal* goalval;
}

%token <intval> 	NUM
%token <strval> 	ID

%type <idval> identifier
%type <expval> expression
%type <expvals> expressions
%type <statval> statement
%type <statvals> statements
%type <typeval> type
%type <varDeclarval> varDeclaration
%type <methodDeclarval> methodDeclaration
%type <varDeclarvals> varsDeclaration
%type <methodParamval> methodParams
%type <classDeclarval> classDeclaration
%type <extval> extends
%type <methodDeclarvals> methodsDeclaration
%type <classDeclarvals> classesDeclaration
%type <mainval> mainClass
%type <goalval> parser

%start parser

%%
parser:
	mainClass classesDeclaration {
		*goal = new Goal(@1.first_line, @1.first_column, $1, $2);
	}
	| error mainClass classesDeclaration { yyerrok; }
	;

mainClass:
	CLASS identifier F_LEFT PUBLIC STATIC VOID MAIN R_LEFT STRING Q_LEFT Q_RIGHT identifier R_RIGHT F_LEFT statement F_RIGHT F_RIGHT {
		$$ = new MainClass(@1.first_line, @1.first_column, $2, $12, $15);
		}
	;

classesDeclaration:
	%empty {
		$$ = new std::vector<std::unique_ptr<IClassDeclaration>>();
	}
	| classesDeclaration classDeclaration {
		$1->push_back(std::unique_ptr<IClassDeclaration>($2));
		$$ = $1;
	}
	| classesDeclaration error classDeclaration { yyerrok; }
	;

classDeclaration:
	CLASS identifier extends F_LEFT varsDeclaration methodsDeclaration F_RIGHT {
		$$ = new ClassDeclaration(@1.first_line, @1.first_column, $2, $3, $5, $6);
	}
	| CLASS error F_LEFT varsDeclaration methodsDeclaration F_RIGHT { yyerrok; }
	;

extends:
	%empty {
		$$ = nullptr;
	}
	| EXTENDS identifier {
		$$ = $2;
	}
	| EXTENDS error { yyerrok; }
	;

varsDeclaration:
	%empty {
		$$ = new std::vector<std::unique_ptr<IVarDeclaration>>();
	}
	| varsDeclaration varDeclaration {
		$1->push_back(std::unique_ptr<IVarDeclaration>($2));
		$$ = $1;
	}
	| varsDeclaration error varDeclaration { yyerrok; }
	;

varDeclaration:
	type identifier SCOLON {
		$$ = new VarDeclaration(@1.first_line, @1.first_column, $1, $2);
	}
	| error SCOLON { yyerrok; }
	;

methodsDeclaration:
	%empty {
		$$ = new std::vector<std::unique_ptr<IMethodDeclaration>>();
	}
	| methodsDeclaration methodDeclaration {
		$1->push_back(std::unique_ptr<IMethodDeclaration>($2));
		$$ = $1;
	}
	;

methodDeclaration:
	PUBLIC type identifier R_LEFT methodParams R_RIGHT F_LEFT varsDeclaration statements RETURN expression SCOLON F_RIGHT {
		$$ = new MethodDeclaration(@1.first_line, @1.first_column, $2, $3, $5, $8, $9, $11);
	}
	| PUBLIC type identifier R_LEFT error R_RIGHT F_LEFT varsDeclaration statements RETURN expression SCOLON F_RIGHT { yyerrok; }
	;

methodParams:
	%empty {
		$$ = new std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>();
	}
	| type identifier {
		$$ = new std::vector<std::pair<std::unique_ptr<IType>, std::unique_ptr<IIdentifier>>>();
		$$->push_back(std::make_pair(std::unique_ptr<IType>($1), std::unique_ptr<IIdentifier>($2)));
	}
	| methodParams COMMA type identifier {
		$$->push_back(std::make_pair(std::unique_ptr<IType>($3), std::unique_ptr<IIdentifier>($4)));
		$$ = $1;
	}
	;

statements:
	%empty {
		$$ = new std::vector<std::unique_ptr<IStatement>>();
	}
	| statement statements {
		$2->insert($2->begin(), std::unique_ptr<IStatement>($1));
		$$ = $2;
	}
	| statement error statements { yyerrok; }
	;

type:
	INT Q_LEFT Q_RIGHT {
		$$ = new Type(IntArrType{});
	}
	| BOOL {
		$$ = new Type(BoolType{});
	}
	| INT {
		$$ = new Type(IntType{});
	}
	| identifier {
		$$ = new Type(ClassType{dynamic_cast<Identifier*>($1)->name});
	}
	;

statement:
	F_LEFT statements F_RIGHT {
		$$ = new Statement(@1.first_line, @1.first_column, $2);
	}
	| IF R_LEFT expression R_RIGHT statement ELSE statement {
		$$ = new IfStatement(@1.first_line, @1.first_column, $3, $5, $7);
	}
	| WHILE R_LEFT expression R_RIGHT statement {
		$$ = new WhileStatement(@1.first_line, @1.first_column, $3, $5);
	}
	| PRINT R_LEFT expression R_RIGHT SCOLON {
		$$ = new PrintStatement(@1.first_line, @1.first_column, $3);
	}
	| identifier EQ expression SCOLON {
		$$ = new AssignmentStatement(@1.first_line, @1.first_column, $1, $3);
	}
	| identifier Q_LEFT expression Q_RIGHT EQ expression SCOLON {
		$$ = new ArrAssignmentStatement(@1.first_line, @1.first_column, $1, $3, $6);
	}
	| F_LEFT error F_RIGHT { yyerrok; }
	| IF R_LEFT error R_RIGHT statement ELSE statement { yyerrok; }
	| WHILE R_LEFT error R_RIGHT statement { yyerrok; }
	;

expressions:
	%empty {
		$$ = new std::vector<std::unique_ptr<IExpression>>();
	}
	| expression {
		$$ = new std::vector<std::unique_ptr<IExpression>>();
		$$->push_back(std::unique_ptr<IExpression>($1));
	}
	| expressions COMMA expression {
		$$->push_back(std::unique_ptr<IExpression>($3));
		$$ = $1;
	}
	;

expression:
	expression AND expression {
		$$ = new AndExpression(@1.first_line, @1.first_column, $1, $3);
	}
	| expression LESS expression {
		$$ = new LessExpression(@1.first_line, @1.first_column, $1, $3);
	}
	| expression PLUS expression {
		$$ = new PlusExpression(@1.first_line, @1.first_column, $1, $3);
	}
	| expression MINUS expression {
		$$ = new MinusExpression(@1.first_line, @1.first_column, $1, $3);
	}
	| expression MULT expression {
		$$ = new MultExpression(@1.first_line, @1.first_column, $1, $3);
	}
	| expression Q_LEFT expression Q_RIGHT {
		$$ = new ArrayExpression(@1.first_line, @1.first_column, $1, $3);
	}
	| expression LENGTH {
		$$ = new LengthExpression(@1.first_line, @1.first_column, $1);
	}
	| expression DOT identifier R_LEFT expressions R_RIGHT {
		$$ = new MethodExpression(@1.first_line, @1.first_column, $1, $3, $5);
	}
	| NUM {
		$$ = new Integer(@1.first_line, @1.first_column, $1);
	}
	| TRUE {
		$$ = new Bool(@1.first_line, @1.first_column, true);
	}
	| FALSE {
		$$ = new Bool(@1.first_line, @1.first_column, false);
	}
	| identifier {
		$$ = new IdentExpression(@1.first_line, @1.first_column, $1);
	}
	| THIS {
		$$ = new This(@1.first_line, @1.first_column);
	}
	| NEW INT Q_LEFT expression Q_RIGHT {
		$$ = new NewArrExpression(@1.first_line, @1.first_column, $4);
	}
	| NEW identifier R_LEFT R_RIGHT {
		$$ = new NewExpression(@1.first_line, @1.first_column, $2);
	}
	| NOT expression {
		$$ = new NotExpression(@1.first_line, @1.first_column, $2);
	}
	| R_LEFT expression R_RIGHT {
		$$ = new Expression(@1.first_line, @1.first_column, $2);
	}
	| R_LEFT error R_RIGHT { yyerrok; }
	;

identifier:
	ID {
		$$ = new Identifier(@1.first_line, @1.first_column, InternTable::getIntern(std::string($1)));
	}
	;
%%

void yyerror(Goal** goal, const char* s) {
	cout << "Error at line: " << yylloc.first_line << " column: " << yylloc.first_column << ". Message: " << s << endl;
	throw std::exception();
}