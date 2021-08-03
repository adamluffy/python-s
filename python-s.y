%{
	#include "symtab.c"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern FILE *yyin;
	extern FILE *yyout;
	extern int lineno;
	extern int yylex();
	void yyerror();
%}

%glr-parser 

/* token definition */
%token CHAR INT FLOAT DOUBLE DEFINE IF BOOL ELSE ELIF WHEN WHILE FOR CONTINUE BREAK VOID RETURN
%token ADDOP MULOP DIVOP INCR OROP ANDOP NOTOP EQUOP  IN ARROW
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMI COLON DOT COMMA ASSIGN NEWLINE RANGE SHORTHAND
%token ID ICONST FCONST CCONST STRING TRUE FALSE

%start program

/* expression priorities and rules */
%left COMMA 
%right ASSIGN
%left OROP
%left ANDOP
%left EQUOP
%left IN 
%left RELOP
%left RANGE
%left ADDOP
%left MULOP DIVOP
%right NOTOP MINUS INCR
%left LPAREN RPAREN LBRACK RBRACK

%%

program: statements;

declaration: type names;

names: names COMMA variable | names COMMA init 
	| variable | init;

init: variable_init | array_init;

variable_init: ID ASSIGN constant ;

array_init: ID array ASSIGN LBRACK values RBRACK;

values: constant | constant COMMA values;

type: INT | FLOAT | DOUBLE | CHAR;



/* statements */
statements: statement | statements statement;

statement: assignment  | declaration | control_statement  | simple_statement
		| loop_statement | function_declaration
		| function_call | increment;

assignment: variable ASSIGN expression;

increment: prefix_inc | postfix_inc;

postfix_inc: ID INCR;

prefix_inc: INCR ID %prec INCR;

simple_statement: CONTINUE | BREAK;

control_statement: if_statement | when_statement;

/* if statement grammar*/
if_statement: IF expression control_structure_body optional_elif optional_else;

elif_part:  elif_part ELIF expression control_structure_body 
	| ELIF expression control_structure_body
; 

else_part: ELSE control_structure_body;

optional_elif: elif_part | /* empty */;

optional_else: else_part | /* empty */;

/* when statement grammar */

when_statement: WHEN when_subject LBRACE when_entry RBRACE;

when_subject: parenthesis_expression | /* empty */;

when_entry: when_condition ARROW control_structure_body COMMA when_entry 
	| when_condition ARROW control_structure_body
	| when_else;

when_else: ELSE ARROW control_structure_body | /* empty */;

when_condition: expression | expression COMMA when_condition;

control_structure_body: LBRACE statements RBRACE;



/* loop */

loop_statement: for_statement | while_statement;

for_statement: FOR LPAREN for_condition RPAREN control_structure_body;

for_condition:  assignment SEMI expression SEMI ID INCR
	| in_expression;

while_statement: WHILE expression control_structure_body;

variable: ID | ID array;

array: LBRACK expression RBRACK;


/* expression */
expression: simple_expression 
	| simple_expression RELOP simple_expression
	| control_expression
	| function_call
	| increment
;

simple_expression: term | simple_expression ADDOP term;

term: factor | term MULOP factor | term DIVOP factor;

factor: ID | constant | parenthesis_expression | ID LBRACK expression RBRACK | NOTOP factor;

constant: numeric_constant | signed_num | CCONST | STRING;

numeric_constant: ICONST | FCONST;

signed_num: ADDOP constant %prec MINUS;

parenthesis_expression: LPAREN expression RPAREN;

range_expression: ICONST RANGE ICONST | CCONST RANGE CCONST;

in_expression: ID IN range_expression;

control_expression: if_expression | when_expression;

if_expression: IF parenthesis_expression expression else_expression;

else_expression: ELSE expression;

/* when expression */
when_expression: WHEN when_subject LBRACE when_entry_expr RBRACE;

when_entry_expr: when_condition ARROW expression COMMA when_entry_expr 
	| when_condition ARROW expression
	| when_else_expr;

when_else_expr: ELSE ARROW expression | /* empty */;




/* functions */
function_call: ID LPAREN optional_call_params RPAREN;

call_params: call_params COMMA expression | expression;

optional_call_params: call_params | /* empty */;

function_declaration: full_func_decl | shorthand_function;

full_func_decl: function_head function_body;

shorthand_function: function_head SHORTHAND expression;

function_head: DEFINE ID LPAREN optional_parameters RPAREN COLON return_type;

function_body: LBRACE statements return_statment RBRACE;

parameters: parameter_declaration | parameter_declaration COMMA parameters;

parameter_declaration: type ID | default_parameter;

default_parameter: type ID ASSIGN constant;

optional_parameters: parameters | /* empty */;

return_type: type | VOID; 

return_statment: RETURN expression;

%%

void yyerror ()
{
  fprintf(stderr, "Syntax error at line %d\n", lineno);
  exit(1);
}

int main (int argc, char *argv[]){

	// initialize symbol table
	init_hash_table();

	// parsing
	int flag;
	yyin = fopen(argv[1], "r");
	flag = yyparse();
	fclose(yyin);
	
	// symbol table dump
	yyout = fopen("symtab_dump.out", "w");
	symtab_dump(yyout);
	fclose(yyout);	
	
	return flag;
}
