#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// header files
#include"ast_const.h"
#include"ast_stmt.h"
#include"ast_ctrl_stmt.h"
#include"ast_var.h"
#include"ast_declare.h"
#include"ast_expr.h"
#include"ast_function.h"
#include"ast_simple.h"
#include"ast_loop.h"

// implementation files
#include"ast_const.c"
#include"ast_stmt.c"
#include"ast_ctrl_stmt.c"
#include"ast_var.c"
#include"ast_declare.c"
#include"ast_expr.c"
#include"ast_function.c"
#include"ast_simple.c"
#include"ast_loop.c"

ASTNode *newASTNode(Node_Type type, ASTNode *left, ASTNode *right){

    ASTNode *val = (ASTNode*)malloc(sizeof(ASTNode));

    val->type = type;
    val->left = left;
    val->right = right;

    return val;
}


void astPrintNode(ASTNode *node){

    /* temp nodes */
	ASTNodeDecl *temp_decl;
	ASTNodeConst *temp_const;
	ASTNodeStatements *temp_statements;
	ASTNodeIf *temp_if;
    ASTNodeWhen *temp_when;
	ASTNodeFor *temp_for;
	ASTNodeForCondition *temp_for_condition;
	ASTNodeAssign *temp_assign;
	ASTNodeSimple *temp_simple;
	ASTNodeIncr *temp_incr;
	ASTNodeFuncCall *temp_func_call;
	ASTNodeArithm *temp_arithm;
	ASTNodeBool *temp_bool;
	ASTNodeRel *temp_rel;
	ASTNodeEql *temp_equ;
	ASTNodeIn *temp_in;
	ASTNodeRange *temp_range;
	ASTNodeVariable *temp_variable;
	ASTNodeFuncDecl *temp_func_decl;
	ASTNodeReturn *temp_return;
	
	switch(node->type){
		case BASIC_NODE:
			printf("Basic Node\n");
			break;
		case DECL_NODE:
			temp_decl = (struct ASTNodeDecl *) node;
			printf("Declaration Node of data-type %d for %d names\n",
				temp_decl->data_type, temp_decl->names_count);
			break;
		case CONST_NODE:
			temp_const = (struct ASTNodeConst *) node;
			printf("Constant Node of const-type %d\n", temp_const->const_type);
			switch(temp_const->const_type){
				case INT_TYPE:
					printf("%d\n", temp_const->val.ival);
					break;
				case REAL_TYPE:
					printf("%.2f\n", temp_const->val.fval);
					break;
				case CHAR_TYPE:
					printf("%c\n",  temp_const->val.cval);
					break;
			}
			break;
		case STATEMENTS:
			temp_statements = (struct ASTNodeStatements *) node;
			printf("Statements Node with %d statements\n", temp_statements->statement_count);
			break;
		case IF_NODE:
			temp_if = (struct ASTNodeIf *) node;
			printf("If Node with %d elseifs\n", temp_if->elif_count);
			break;
        case WHEN_NODE:
            temp_when = (struct ASTNodeWhen*)node;
            printf("When Node\n");
            break;
		case ELSIF_NODE:
			printf("Elsif Node\n");
			break;
		case FOR_NODE:
			temp_for = (struct ASTNodeFor*)node;
			printf("For Node\n");
			break;
		case FOR_CONDITION_NODE:
			temp_for_condition = (struct ASTNodeForCondition*)node;
			printf("For loop condition with counter %s\n",temp_for_condition->counter->st_name);
			break;
		case WHILE_NODE:
			printf("While Node\n");
			break;
		case ASSIGN_NODE:
			temp_assign = (struct ASTNodeAssign *) node;
			printf("Assign Node of entry %s\n", temp_assign->entry->st_name);
			break;
		case SIMPLE_NODE:
			temp_simple = (struct ASTNodeSimple *) node;
			printf("Simple Node of statement %d\n", temp_simple->statement_type);
			break;
		case INCR_NODE:
			temp_incr = (struct ASTNodeIncr *) node;
			printf("Increment Node of entry %s being %d %d\n", 
				temp_incr->entry->st_name, temp_incr->incr_type, temp_incr->pf_type);
			break;
		case FUNC_CALL:
			temp_func_call = (struct ASTNodeFuncCall *) node;
			printf("Function Call Node with %d parameters\n", temp_func_call->num_of_pars);
			break;
		case ARITHM_NODE:
			temp_arithm = (struct ASTNodeArithm *) node;
			printf("Arithmetic Node of operator %d\n", temp_arithm->op);
			break;
		case BOOL_NODE:
			temp_bool = (struct ASTNodeBool *) node;
			printf("Boolean Node of operator %d\n", temp_bool->op);
			break;
		case REL_NODE:
			temp_rel = (struct ASTNodeRel *) node;
			printf("Relational Node of operator %d\n", temp_rel->op);
			break;
		case EQU_NODE:
			temp_equ = (struct ASTNodeEql *) node;
			printf("Equality Node of operator %d\n", temp_equ->op);
			break;
		case IN_NODE:
			temp_in = (struct ASTNodeIn*)node;
			printf("In node of operator %d\n", temp_in->op);
			break;
		case RANGE_NODE:
			temp_range = (struct ASTNodeRange*)node;
			printf("Range node of operator %d\n", temp_range->op);
			break;
		case VARIABLE_NODE:
			temp_variable = (struct ASTNodeVariable*)node;
			printf("Variable with type %d\n", temp_variable->symtab_item->st_type);
			break;
		case FUNC_DECL:
			temp_func_decl = (struct ASTNodeFuncDecl *) node;
			printf("Function Declaration Node of %s with ret_type %d\n", temp_func_decl->entry->st_name, temp_func_decl->ret_type);
			break;
		case RETURN_NODE:
			temp_return = (struct ASTNodeReturn *) node;
			printf("Return Node of ret_type %d\n", temp_return->ret_type);
			break;
		default: /* wrong choice case */
			fprintf(stderr, "Error in node selection!\n");
			exit(1);
	}
}


void astTraversal(ASTNode *node){

    int i;
	
	/* check if empty */
	if(node == NULL){
		return;
	}
	
	/* left and right child cases */
	if(node->type == BASIC_NODE || node->type == ARITHM_NODE || node->type == BOOL_NODE
	|| node->type == REL_NODE || node->type == EQU_NODE){
		astTraversal(node->left);
		astTraversal(node->right);
		astPrintNode(node); // postfix
	}
	/* statements case */
	else if(node->type == STATEMENTS){
		ASTNodeStatements *temp_statements = (struct ASTNodeStatements *) node;
		astPrintNode(node);
		for(i = 0; i < temp_statements->statement_count; i++){
			astTraversal(temp_statements->statements[i]);
		}
	}
	/* the if case */
	else if(node->type == IF_NODE){
		ASTNodeIf *temp_if = (struct ASTNodeIf *) node;
		astPrintNode(node);
		printf("Condition:\n");
		astTraversal(temp_if->condition);
		printf("If branch:\n");
		astTraversal(temp_if->if_branch);
		if(temp_if->elif_count>0){
			printf("Elif branches:\n");
			for(i = 0; i < temp_if->elif_count; i++){
				printf("Elif branch %d :\n", i);
				astTraversal(temp_if->elif_branch[i]);
			}
		}
		
		if(temp_if->else_branch != NULL){
			printf("Else branch:\n");
			astTraversal(temp_if->else_branch);
		}
		
	}
	/* the else if case */ 
	else if(node->type == ELSIF_NODE){
		ASTNodeElif *temp_elif = (struct ASTNodeElif *) node;
		astPrintNode(node);
		astTraversal(temp_elif->condition);
		astTraversal(temp_elif->elif_branch);
	}
    /* when case */
    else if(node->type == WHEN_NODE){
        ASTNodeWhen *temp_when = (struct ASTNodeWhen*)node;
        astPrintNode(node);
        astTraversal(temp_when->condition_expr);
        astTraversal(temp_when->when_branch);
    }
    /* when body */
    else if(node->type == WHEN_BODY_NODE){
        ASTNodeWhenBody *temp_when_body = (struct ASTNodeWhenBody*)node;
        astPrintNode(node);
        for(i=0;i<temp_when_body->entries_count;i++){
            astTraversal(temp_when_body->when_entries[i]);
        }
        astTraversal(temp_when_body->else_branch);
    }
    /* when entry */
    else if(node->type == WHEN_ENTRY_NODE){
        ASTNodeWhenEntry *temp_when_entry = (struct ASTNodeWhenEntry*)node;
        astPrintNode(node);
        for(i=0;i<temp_when_entry->expr_count;i++){
            astTraversal(temp_when_entry->expressions[i]);
        }
        astTraversal(temp_when_entry->entry_branch);
    }
	/* for loop condition */
	else if(node->type == FOR_CONDITION_NODE){
		ASTNodeForCondition *temp_for_condition = (struct ASTNodeForCondition*)node;
		astPrintNode(node);
		if(temp_for_condition->initialize!=NULL){
			astTraversal(temp_for_condition->initialize);
		}
		astTraversal(temp_for_condition->condition);
		if(temp_for_condition->increment!=NULL){
			astTraversal(temp_for_condition->increment);
		}
	}
	/* for case */
	else if(node->type == FOR_NODE){
		ASTNodeFor *temp_for = (struct ASTNodeFor *) node;
		astPrintNode(node);
		printf("Condition:\n");
		astTraversal(temp_for->condition);
		printf("For branch:\n");
		astTraversal(temp_for->for_branch);
	
	}
	/* while case */
	else if(node->type == WHILE_NODE){
		ASTNodeWhile *temp_while = (struct ASTNodeWhile *) node;
		astPrintNode(node);
		astTraversal(temp_while->condition);
		astTraversal(temp_while->while_branch);
	}
	/* assign case */
	else if(node->type == ASSIGN_NODE){
		ASTNodeAssign *temp_assign = (struct ASTNodeAssign *) node;
		astPrintNode(node);
		printf("Assigning:\n");
		astTraversal(temp_assign->assign_val);
	}
	/* function call case */
	else if(node->type == FUNC_CALL){
		ASTNodeFuncCall *temp_func_call = (struct ASTNodeFuncCall *) node;
		astPrintNode(node);
		printf("Call parameters:\n");
		for(i = 0; i < temp_func_call->num_of_pars; i++){
			astTraversal(temp_func_call->params[i]);
		}
	}
	/* return case */
	else if(node->type == RETURN_NODE){
		ASTNodeReturn *temp_return = (struct ASTNodeReturn *) node;
		astPrintNode(node);
		printf("Returning:\n");
		astTraversal(temp_return->ret_val);
	}
	/* others */
	else{
		astPrintNode(node);
	}
}