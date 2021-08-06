%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include"semantic.c"
	#include"symtab.c"
	#include"ast/ast.c"

	extern FILE *yyin;
	extern FILE *yyout;
	extern int lineno;
	extern int yylex();
	void yyerror();

	// for declarations
	void add_to_names(listNode *entry);
	listNode **names;
	int nc = 0;

	void add_to_exprs(ASTNode *entry);
	ASTNode **expressions;
	int ec = 0;

	// for else ifs
	void add_elseif(ASTNode *elsif);
	ASTNode **elsifs;
	int elseif_count = 0;
	
	// for the initializations of arrays
	void add_to_vals(Value val);
	Value *vals;
	int vc = 0;
%}


%union{
	
	// different types of values
	Value val;

	// structures
	listNode *symtab_item;
	ASTNode *node;

	// for declarations
	int data_type;
	int const_type;

	// for arrays
	int array_size;
}

/* token definition */
%token <val> CHAR INT FLOAT DOUBLE DEFINE IF BOOL ELSE ELIF WHEN WHILE FOR CONTINUE BREAK VOID RETURN
%token <val> ADDOP MULOP DIVOP INCR OROP ANDOP NOTOP EQUOP RELOP IN ARROW
%token <val> LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMI COLON DOT COMMA ASSIGN NEWLINE RANGE SHORTHAND
%token <symtab_item> ID 
%token <val> ICONST
%token <val> FCONST
%token <val> CCONST
%token <val> STRING



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


%type <node> declaration
%type <node> statements statement assignment
%type <node> simple_statement
%type <data_type> type
%type <node> variable 
%type <array_size> array
%type <symtab_item> init variable_init array_init
%type <node> constant numeric_constant string_constant
%type <node> expression simple_expression boolean_expression 
%type <node> logical_expression relation_expression 
%type <node> term factor parenthesis_expression range_expression
%type <node> control_expression if_expression else_expression when_expression
%type <node> in_expression when_entry_expr when_else_expr when_subject
%type <node> increment postfix_inc prefix_inc
%type <node> control_structure_body
%type <node> control_statement if_statement elif_part optional_elif else_part optional_else
%type <val> sign

%start program

%%



program: statements;

declaration: { declare = 1; } type names { declare = 0; } SEMI
	{
		int i;
		$$ = newASTDeclNode($2, names, nc);
		nc = 0;
		
		ASTNodeDecl *temp = (ASTNodeDecl*) $$;
		
		// declare types of the names
		for(i=0; i < temp->names_count; i++){
			// variable
			if(temp->names[i]->st_type == UNDEF){
				set_type(temp->names[i]->st_name, temp->data_type, UNDEF);
			}
			// array
			else if(temp->names[i]->st_type == ARRAY_TYPE){
				set_type(temp->names[i]->st_name, ARRAY_TYPE, temp->data_type);
			}
		}
		astTraversal($$); /* just for testing */
	}
;

names: names COMMA variable 
		{
			ASTNodeVariable *temp = (ASTNodeVariable*)$3;
			add_to_names(temp->symtab_item);
		}
	| names COMMA init
		{
			add_to_names($3);
		}
	| variable 
		{
			ASTNodeVariable *temp = (ASTNodeVariable*)$1;
			add_to_names(temp->symtab_item);
		}
	| init
		{
			add_to_names($1);
		}
;

init: variable_init 
	{
		$$ = $1;
	}
| array_init
	{
		$$ = $1;
	}
;

variable_init: ID ASSIGN constant
	{
		ASTNodeConst *temp = (ASTNodeConst*)$3;
		$1->val = temp->val;
		$1->st_type = temp->const_type;
		$$ = $1;
	}
;

array_init: ID array ASSIGN LBRACK values RBRACK
{
	if($1->array_size != vc){
		fprintf(stderr, "Array init at %d doesn't contain the right amount of values!\n", lineno);
	}
	$1->vals = vals;
	$1->array_size = $2;
	$$ = $1;
	vc = 0; // variable count

}
;

values: constant 
		{
			ASTNodeConst *temp = (ASTNodeConst*)$1;
			add_to_vals(temp->val);
		}
	| values COMMA constant
		{
			ASTNodeConst *temp = (ASTNodeConst*)$3;
			add_to_vals(temp->val);
		}	
;

type: INT		{ $$ = INT_TYPE; } 
	| FLOAT 	{ $$ = REAL_TYPE; }
	| DOUBLE 	{ $$ = REAL_TYPE; }
	| CHAR		{ $$ = CHAR_TYPE; }
	| VOID 		{ $$ = VOID_TYPE; }
;	



/* statements */
statements: statement 
	{
		$$ = newASTStatementsNode(NULL,0,$1);
	}
| statements statement
	{
		ASTNodeStatements *temp = (ASTNodeStatements*)$1;
		$$ = newASTStatementsNode(temp->statements, temp->statement_count, $2);
	}
;

statements_optional: statements | /* empty */;

statement: assignment
			{
				$$ = $1;
				astTraversal($$);
			}
		| declaration 
			{
				$$ = $1;
			}
		| control_statement  
			{
				$$ = $1;
			}
		| simple_statement
			{
				$$ = $1;
			}
		| loop_statement 
			{
				$$ = NULL;
			}
		| function
			{
				$$ = NULL;
			}
		| function_call 
			{
				$$ = NULL;
			}
		| increment
			{
				$$ = $1;
			}
;

assignment: variable ASSIGN expression
	{
		ASTNodeVariable *temp = (ASTNodeVariable*)$1;
		$$ = newASTAssignNode(temp->symtab_item, $3);
	}
;

increment: prefix_inc
		{
			$$ = $1;
		}
	| postfix_inc
		{
			$$ = $1;
		}
;

postfix_inc: ID INCR
	{
		if($2.ival == INC){
			$$ = newASTIncrNode($1, 0, 0);
		}else{
			$$ = newASTIncrNode($1, 1, 0);
		}
		astTraversal($$);
	}
;

prefix_inc: INCR ID %prec INCR
	{
		if($1.ival == INC){
			$$ = newASTIncrNode($2, 0, 1);
		}else{
			$$ = newASTIncrNode($2, 1, 1);
		}
		astTraversal($$);
	}
;

simple_statement: CONTINUE 
		{
			$$ = newASTSimpleNode(0);
			astTraversal($$);
		}
	| BREAK
		{
			$$ = newASTSimpleNode(1);
			astTraversal($$);
		}
;

control_statement: if_statement { $$ = $1; }
	| when_statement { $$ = NULL; } // will do this later
;

/* if statement grammar*/
if_statement: IF expression control_structure_body optional_elif optional_else
	{
		$$ = newASTIfNode($2, $3, elsifs, elseif_count, $5);
		elseif_count = 0;
		elsifs = NULL;
	}
;

elif_part:  elif_part ELIF expression control_structure_body 
		{
			ASTNode *temp = newASTElsifNode($3, $4);
			add_elseif(temp);
		}
	| ELIF expression control_structure_body
		{
			ASTNode *temp = newASTElsifNode($2, $3);
			add_elseif(temp);
		}
; 

else_part: ELSE control_structure_body
	{
		$$ = $2;
	}
;

optional_elif: elif_part 
	{ $$ = $1; }
	| /* empty */	{ $$ = NULL; }
;

optional_else: else_part { $$ = $1; }
	 | /* empty */  { $$ = NULL; }
;

/* when statement grammar */

when_statement: WHEN when_subject LBRACE when_entry RBRACE;

when_subject: parenthesis_expression 
	{
		$$ = $1;
	}
| /* empty */
	{ $$ = NULL; }
;

when_entry: when_condition ARROW control_structure_body COMMA when_entry 
	| when_condition ARROW control_structure_body
	| when_else;

when_else: ELSE ARROW control_structure_body | /* empty */;

when_condition: expression 
		{
			add_to_exprs($1);
		}
	| when_condition COMMA expression 
		{
			add_to_exprs($3);
		}
;

control_structure_body: LBRACE statements RBRACE
	{
		$$ = $2;
		astTraversal($2);
	}
;



/* loop */

loop_statement: for_statement | while_statement;

for_statement: FOR LPAREN for_condition RPAREN control_structure_body;

for_condition:  assignment SEMI expression SEMI ID INCR
	| in_expression;

while_statement: WHILE expression control_structure_body;

variable: ID	{ $$ = newASTVariableNode($1); } 
	| ID array
		{
			$1->st_type = ARRAY_TYPE;
			$1->array_size = $2;
			$$ = newASTVariableNode($1);
		}
;

array: LBRACK ICONST RBRACK
	{
		// temporarly set to int const for testing
		$$ = $2.ival;
	}
;


/* expression */
expression: simple_expression { $$ = $1; }
	| control_expression	{ $$ = $1; }
	| function_call { $$ = NULL; }
	| increment { $$ = $1; }
;

simple_expression: term { $$ = $1; }
	| simple_expression ADDOP term 
		{
			$$ = newASTArithmNode($2.ival, $1, $3);
		}
	| boolean_expression
		{
			$$ = $1;
		}
	;

boolean_expression: logical_expression 
		{ $$ = $1; }
	| relation_expression
		{ $$ = $1; }	
;

logical_expression: simple_expression ANDOP term 
		{
			$$ = newASTBoolNode(AND, $1, $3);
			astTraversal($$);
		}
	| simple_expression OROP term 
		{
			$$ = newASTBoolNode(OR, $1, $3);
			astTraversal($$);
		}
;

relation_expression: simple_expression RELOP term 
		{
			$$ = newASTRelNode($2.ival, $1, $3);
			astTraversal($$);
		}
	| simple_expression EQUOP term
		{
			$$ = newASTEqlNode($2.ival, $1, $3);
			astTraversal($$);
		}
;

term: factor 
		{
			$$ = $1;
		}
	| term MULOP factor
		{
			$$ = newASTArithmNode(MUL, $1, $3);
			astTraversal($$);
		} 
	| term DIVOP factor
		{
			$$ = newASTArithmNode(DIV, $1, $3);
			astTraversal($$);
		}
	;

factor: variable { $$ = $1; }
	| constant { $$ = $1; }
	| parenthesis_expression { $$ = $1; } 
	| NOTOP factor 
		{
			$$ = newASTBoolNode(NOT, $2, NULL);
			astTraversal($$);
		}
	| sign numeric_constant 
		{
			if($1.ival == 1){
				ASTNodeConst *temp = (ASTNodeConst*)$2;

				switch(temp->const_type){
					case INT_TYPE:
						temp->val.ival *= -1;
						break;
					case REAL_TYPE:
						temp->val.fval *= -1;
						break;
				}

				$$ = (ASTNode*)temp;
			}else{
				$$ = $2;
			}
			astTraversal($$);

		}
		
;

sign: ADDOP
	{
		if($1.ival == ADD){
			fprintf(stderr, "Error having plus as a sign!\n");
			exit(1);
		}else{
			$$.ival = 1; // sign
		}
	}
;

constant: 
	numeric_constant { $$ = $1; }
	| string_constant { $$ = $1; }
;	

numeric_constant: ICONST 	{ $$ = newASTConstNode(INT_TYPE,$1); }
	| FCONST { $$ = newASTConstNode(REAL_TYPE,$1); }
;

string_constant: CCONST 	{ $$ = newASTConstNode(CHAR_TYPE,$1); } 
	| STRING	{ $$ = newASTConstNode(STR_TYPE,$1); }
;

parenthesis_expression: LPAREN expression RPAREN { $$ = $2; };

range_expression: numeric_constant RANGE numeric_constant
	{
		$$ = newASTRangeNode(TO,$1,$3);
		astTraversal($$);
	}
;

in_expression: ID IN range_expression
	{ 
		$$ = NULL; // figure out later
	}
;

control_expression: if_expression 
		{ $$ = $1; }
	| when_expression
		{ $$ = $1; }
;

if_expression: IF parenthesis_expression expression else_expression
	{
		$$ = newASTIfNode($2, $3, NULL, 0, $4);
		astTraversal($$);
	}
;

else_expression: ELSE expression
	{
		$$ = $2;
	}
;

/* when expression */
when_expression: WHEN when_subject LBRACE when_entry_expr RBRACE
	{
		if($2 != NULL){
			$$ = newASTWhenNode($2, $4);
		}else{
			$$ = newASTWhenNode(NULL, $4);
		}

		astTraversal($$);
	}
;

when_entry_expr: when_condition ARROW expression COMMA when_entry_expr 
		{
			$$ = newASTWhenEntryNode(expressions, ec, $3);
			expressions = NULL;
			ec = 0;
			astTraversal($$);
		}
	| when_condition ARROW expression
		{
			$$ = newASTWhenEntryNode(expressions, ec, $3);
			expressions = NULL;
			ec = 0;
			astTraversal($$);
		}
	| when_else_expr
		{
			$$ = $1;
		}	
;

when_else_expr: ELSE ARROW expression 
		{
			$$ = $3;
		}
	| /* empty */
	{
		$$ = NULL;
	}
;




/* functions */
function_call: ID LPAREN call_params RPAREN;

call_params: call_param | /* empty */;

call_param: call_param COMMA expression | expression;

function: { incr_scope(); }function_head function_body {hide_scope();};

function_head: { declare = 1; }DEFINE ID LPAREN optional_parameters RPAREN COLON return_type { declare = 0; };

function_body: LBRACE statements_optional return_optional RBRACE | SHORTHAND expression;

parameters: parameters COMMA parameter |  parameter;

parameter: { declare = 1; } type param_init { declare = 0; };

param_init: variable_init | variable;

optional_parameters: parameters | /* empty */;

return_type: type; 

return_statement: RETURN expression;

return_optional: return_statement | /* empty */;

%%

void yyerror ()
{
  fprintf(stderr, "Syntax error at line %d\n", lineno);
  exit(1);
}

void add_to_names(listNode *entry){
	// first entry
	if(nc == 0){
		nc = 1;
		names = (listNode **) malloc( 1 * sizeof(listNode *));
		names[0] = entry;
	}
	// general case
	else{
		nc++;
		names = (listNode **) realloc(names, nc * sizeof(listNode *));
		names[nc - 1] = entry;		
	}
}

void add_to_exprs(ASTNode *entry){

	if(ec == 0){
		ec = 1;
		expressions = (ASTNode **)malloc(1*sizeof(ASTNode*));
		expressions[0] = entry;
	}else{
		ec++;
		expressions = (ASTNode **)realloc(expressions, ec*sizeof(ASTNode*));
		expressions[ec-1] = entry;
	}
}

void add_elseif(ASTNode *elsif){
  // first entry
  if(elseif_count == 0){
    elseif_count = 1;
    elsifs = (ASTNode **) malloc(1 * sizeof(ASTNode*));
    elsifs[0] = elsif;
  }
  // general case
  else{
    elseif_count++;
    elsifs = (ASTNode **) realloc(elsifs, elseif_count * sizeof(ASTNode*));
    elsifs[elseif_count - 1] = elsif;
  }
}

void add_to_vals(Value val){
	// first entry
	if(vc == 0){
		vc = 1;
		vals = (Value *) malloc(1 * sizeof(Value));
		vals[0] = val;
	}
	// general case
	else{
		vc++;
		vals = (Value *) realloc(vals, vc * sizeof(Value));
		vals[vc - 1] = val;
	}
}

int main (int argc, char *argv[]){

	// initialize symbol table
	init_hash_table();

	queue = NULL;

	// parsing
	int flag;
	yyin = fopen(argv[1], "r");
	flag = yyparse();
	fclose(yyin);

	printf("Parsing finished!\n");

	if(queue!=NULL){
		printf("Warning: Something has not been checked in the revisit queue!\n");
	}
	
	// symbol table dump
	yyout = fopen("symtab_dump.out", "w");
	symtab_dump(yyout);
	fclose(yyout);	

	// revisit queue dump
	yyout = fopen("revisit_dump.out", "w");
	revisit_dump(yyout);
	fclose(yyout);
	
	return flag;
}
