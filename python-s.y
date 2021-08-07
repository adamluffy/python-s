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

	// for functions
	ASTNodeFuncDecl *temp_function;
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

	// for parameters
	Parameter par;
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
%right NOTOP INCR MINUS 
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
%type <node> when_statement when_entry when_else when_condition 
%type <node> loop_statement for_statement for_condition while_statement
%type <node> function optional_parameters parameters param_init
%type <par> parameter
%type <node> return_type function_call call_params call_param

%start program

%%



program: statements { astTraversal($1); };

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

statements_optional: statements 
		{
			temp_function->statements = $1;
		}
	| /* empty */ { temp_function->statements = NULL; }
;

statement: assignment
			{
				$$ = $1;
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
				$$ = $1;
			}
		| function
			{
				$$ = NULL;
			}
		| function_call 
			{
				$$ = $1;
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

		get_result_type(
			get_type(temp->symtab_item->st_name),
			expressionDataType($3),
			NONE
		);
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
	}
;

prefix_inc: INCR ID %prec INCR
	{
		if($1.ival == INC){
			$$ = newASTIncrNode($2, 0, 1);
		}else{
			$$ = newASTIncrNode($2, 1, 1);
		}
	}
;

simple_statement: CONTINUE 
		{
			$$ = newASTSimpleNode(0);
			
		}
	| BREAK
		{
			$$ = newASTSimpleNode(1);
			
		}
;

control_statement: if_statement { $$ = $1; }
	| when_statement { $$ = $1; } // will do this later
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

when_statement: WHEN when_subject LBRACE when_entry when_else RBRACE
	{
		ASTNodeWhenBody *temp_body = malloc(sizeof(ASTNodeWhenBody));
		temp_body->type = WHEN_BODY_NODE;
		temp_body->when_entries = elsifs;
		temp_body->entries_count = elseif_count;
		elsifs = NULL;
		elseif_count = 0;
		ASTNode* temp = (ASTNode*)temp_body;

		if($2 != NULL){
			$$ = newASTWhenNode(NULL, temp, $5);
		}else{
			$$ = newASTWhenNode($2, temp, $5);
		}

		
	}
;

when_subject: parenthesis_expression 
	{
		$$ = $1;
	}
| /* empty */
	{ $$ = NULL; }
;

when_entry: when_entry COMMA when_condition ARROW control_structure_body  
		{
			ASTNodeWhenConds *temp = (ASTNodeWhenConds*)$3;
			$$ = newASTWhenEntryNode(temp->expressions, temp->expr_count,$5);
			add_elseif($$);
		}
	| when_condition ARROW control_structure_body
		{

			ASTNodeWhenConds *temp = (ASTNodeWhenConds*)$1;
			$$ = newASTWhenEntryNode(temp->expressions, temp->expr_count,$3);
			add_elseif($$);
			
		}
;

when_else: COMMA ELSE ARROW control_structure_body 
		{
			$$ = $4;
		}
	| /* empty */
		{
			$$ = NULL;
		}	
;

when_condition: expression 
		{
			$$ = newASTWhenCondsNode(NULL,0,$1);
		}
	| when_condition COMMA expression 
		{
			ASTNodeWhenConds *temp = (ASTNodeWhenConds*)$1;
			$$ = newASTWhenCondsNode(temp->expressions, 
				temp->expr_count, $3);
		}
;

control_structure_body: LBRACE statements RBRACE
	{
		$$ = $2;
	}
;



/* loop */

loop_statement: for_statement { $$ = $1; }
	| while_statement	{ $$ = $1; }
;

for_statement: FOR LPAREN for_condition RPAREN control_structure_body
	{
		$$ = newASTForNode($3,$5);
		set_loop_counter($3);
	}
;

for_condition:  assignment SEMI expression SEMI ID INCR
		{
			/* create increment node*/
    		ASTNode *incr_node;
    		if($6.ival == INC){ /* increment */
    		    incr_node = newASTIncrNode($5, 0, 0);
    		}
    		else{
    		    incr_node = newASTIncrNode($5, 1, 0);
    		}
			$$ = newASTForConditionNode(0, $1, $3, incr_node);
    		
		}
	| in_expression
	{
		$$ = newASTForConditionNode(1, NULL, $1, NULL);
	};

while_statement: WHILE expression control_structure_body
	{
		$$ = newASTWhileNode($2,$3);
	}
;

variable: ID	{ $$ = newASTVariableNode($1); } 
	| ID array
		{
			$1->st_type = ARRAY_TYPE;
			$1->array_size = $2;
			$$ = newASTVariableNode($1);
		}
;

array: LBRACK expression RBRACK
	{
		// temporarly set to int const for testing

		if(declare == 1 && $2->type != CONST_NODE){
			fprintf(stderr, "Array declaration at %d contains expression!\n", lineno);
			exit(1);
		}
		else if($2->type == CONST_NODE && declare == 0){
			ASTNodeConst *temp = (ASTNodeConst*)$2;
			if(temp->const_type == INT_TYPE) {
				$$ = temp->val.ival;
			}
		}
	}
;


/* expression */
expression: simple_expression { $$ = $1; }
	| control_expression	{ $$ = $1; }
	| function_call { $$ = $1; }
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
			
		}
	| simple_expression OROP term 
		{
			$$ = newASTBoolNode(OR, $1, $3);
			
		}
;

relation_expression: simple_expression RELOP term 
		{
			$$ = newASTRelNode($2.ival, $1, $3);
			
		}
	| simple_expression EQUOP term
		{
			$$ = newASTEqlNode($2.ival, $1, $3);
			
		}
;

term: factor 
		{
			$$ = $1;
		}
	| term MULOP factor
		{
			$$ = newASTArithmNode(MUL, $1, $3);
			
		} 
	| term DIVOP factor
		{
			$$ = newASTArithmNode(DIV, $1, $3);
			
		}
	;

factor: variable { $$ = $1; }
	| constant { $$ = $1; }
	| parenthesis_expression { $$ = $1; } 
	| NOTOP factor 
		{
			$$ = newASTBoolNode(NOT, $2, NULL);
			
		}
	| ADDOP numeric_constant %prec MINUS
		{
			if($1.ival == ADD){
				fprintf(stderr, "Error having plus as a sign!\n");
				exit(1);
			}else{

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
		
	}
	| string_constant RANGE string_constant
	{
		$$ = newASTRangeNode(TO,$1,$3);
	}
;

in_expression: ID IN range_expression
	{ 	
		ASTNode *temp = newASTVariableNode($1);
		$$ = newASTInNode(INOP, temp, $3); // figure out later
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
		
	}
;

else_expression: ELSE expression
	{
		$$ = $2;
	}
;

/* when expression */
when_expression: WHEN when_subject LBRACE when_entry_expr when_else_expr RBRACE
	{
		ASTNodeWhenBody *temp_body = malloc(sizeof(ASTNodeWhenBody));
		temp_body->type = WHEN_BODY_NODE;
		temp_body->when_entries = elsifs;
		temp_body->entries_count = elseif_count;
		elsifs = NULL;
		elseif_count = 0;
		ASTNode* temp = (ASTNode*)temp_body;

		if($2 != NULL){
			$$ = newASTWhenNode(NULL, temp, $5);
		}else{
			$$ = newASTWhenNode($2, temp, $5);
		}

		
	}
;

when_entry_expr: when_entry_expr COMMA when_condition ARROW expression  
		{

			ASTNodeWhenConds *temp = (ASTNodeWhenConds*)$3;
			$$ = newASTWhenEntryNode(temp->expressions, temp->expr_count,$5);
			add_elseif($$);
			
		}
	| when_condition ARROW expression
		{
			ASTNodeWhenConds *temp = (ASTNodeWhenConds*)$1;
			$$ = newASTWhenEntryNode(temp->expressions, temp->expr_count,$3);
			add_elseif($$);
		}
;

when_else_expr: COMMA ELSE ARROW expression 
		{
			$$ = $4;
		}
	| /* empty */
	{
		$$ = NULL;
	}
;




/* functions */
function_call: ID LPAREN call_params RPAREN
	{
		ASTNodeCallParams *temp = (ASTNodeCallParams*)$3;
		$$ = newASTFuncCallNode($1, temp->params, temp->num_of_pars);
	
		// add information to revisit the queue entry (if exists)
		revisitQueue *q = search_queue($1->st_name);
		if(q!=NULL){
			q->num_of_pars = temp->num_of_pars;
			q->par_types = (int*)malloc(temp->num_of_pars*sizeof(int));

			for(int i=0; i<temp->num_of_pars;i++){
				q->par_types[i] = expressionDataType(temp->params[i]);
			}
		}
	
	}
;

call_params: call_param 
		{
				
			if($1->type == CONST_NODE){
				ASTNodeConst *temp = (ASTNodeConst*)$1;
				if(temp->const_type == STR_TYPE) {
					$$ = newASTCallParamsNode(NULL,0,$1);
				}
			}else{
				$$ = $1;
			}
			
		}
	| /* empty */
		{
			ASTNodeCallParams *temp = malloc(sizeof(ASTNodeCallParams));
			temp->type = CALL_PARAMS;
			temp->params = NULL;
        	temp->num_of_pars = 0;
        	$$ = (ASTNode*)temp;
		}
;

call_param: call_param COMMA expression 
		{
			ASTNodeCallParams *temp = (ASTNodeCallParams*)$1;
			$$ = newASTCallParamsNode(temp->params,temp->num_of_pars,$3);
		}
	| expression
		{
			$$ = newASTCallParamsNode(NULL,0,$1);
		}
;

function: { incr_scope(); }function_head function_body 
	{
		hide_scope();
		$$ = (ASTNode*)temp_function;
		astTraversal($$); // testing
	}
;

function_head: { declare = 1; } DEFINE ID LPAREN optional_parameters RPAREN COLON return_type 
	{ 
		declare = 0; 

		ASTNodeRetType *temp = (ASTNodeRetType*)$8;
		temp_function = (ASTNodeFuncDecl*)
			newASTFuncDeclNode(temp->ret_type, $3);
		temp_function->entry->st_type = FUNCTION_TYPE;
		temp_function->entry->inf_type = temp->ret_type;

		if($5 != NULL){
			ASTNodeDeclParams *temp = (ASTNodeDeclParams*)$5;

			temp_function->entry->parameters = temp->parameters;
			temp_function->entry->num_of_pars = temp->num_of_pars;
		}else{
			temp_function->entry->parameters = NULL;
			temp_function->entry->num_of_pars = 0;
		}
	}
;

function_body: LBRACE statements_optional return_optional RBRACE | SHORTHAND expression;

parameters: parameters COMMA parameter 
		{
			ASTNodeDeclParams *temp = (ASTNodeDeclParams*)$1;
			$$ = newASTDeclParamsNode(temp->parameters, temp->num_of_pars,$3);
		}
	|  parameter
		{
			$$ = newASTDeclParamsNode(NULL, 0 ,$1);
		}
;

parameter: { declare = 1; } type param_init 
	{ 
		declare = 0; 
		ASTNodeVariable *temp = (ASTNodeVariable*)$3;

		if(temp->symtab_item->st_type == UNDEF){
		
			set_type(temp->symtab_item->st_name, $2, UNDEF);

		}else if(temp->symtab_item->st_type == ARRAY_TYPE){
			
			set_type(temp->symtab_item->st_name,ARRAY_TYPE,$2);
			
		}

		$$ = def_param($2, temp->symtab_item->st_name,0, temp->symtab_item->val);
	}
;

param_init: variable_init 
		{
			$$ = newASTVariableNode($1);
		}
	| variable { $$ = $1; };

optional_parameters: parameters
		{
			$$ = $1;
		}
 	| /* empty */ { $$ = NULL; }
;

return_type: type
	{
		$$ = newASTRetTypeNode($1);
	}
; 

return_optional: RETURN expression
	{
		temp_function->return_node = newASTReturnNode(temp_function->ret_type,$2);
	} 
	| /* empty */
	{
		temp_function->return_node = NULL;
	}
;

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
