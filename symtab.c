#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* current scope */
int cur_scope = 0;
int declare = 0; // 1: declare , 0: not declare

// intialize hast table data structure
void init_hash_table(){
	int i; 
	hash_table = malloc(SIZE * sizeof(listNode*));
	for(i = 0; i < SIZE; i++) hash_table[i] = NULL;
}

unsigned int hash(char *key){
	unsigned int hashval = 0;
	for(;*key!='\0';key++) hashval += *key;
	hashval += key[0] % 11 + (key[0] << 3) - key[0];
	return hashval % SIZE;
}

void insert(char *name, int len, int type, int lineno){
	unsigned int hashval = hash(name);
	listNode *l = hash_table[hashval];
	
	while ((l != NULL) && (strcmp(name,l->st_name) != 0)) l = l->next;
	
	/* variable not yet in table */
	if (l == NULL){
		l = (listNode*) malloc(sizeof(listNode));
		strncpy(l->st_name, name, len);  
		/* add to hashtable */
		l->st_type = type;
		l->scope = cur_scope;
		l->lines = (RefList*) malloc(sizeof(RefList));
		l->lines->lineno = lineno;
		l->lines->next = NULL;
		l->next = hash_table[hashval];
		hash_table[hashval] = l; 
		printf("Inserted %s for the first time with linenumber %d!\n", name, lineno); // error checking
	}
	/* found in table, so just add line number */
	else{
		
		// just add line number to the dump
		if(declare == 0){
			l->scope = cur_scope;
			RefList *t = l->lines;
			while (t->next != NULL) t = t->next;
			/* add linenumber to reference list */
			t->next = (RefList*) malloc(sizeof(RefList));
			t->next->lineno = lineno;
			t->next->next = NULL;
			printf("Found %s again at line %d!\n", name, lineno);
		}
		// new entry
		else{

			// same scope and declare, error
			if(l->scope == cur_scope){
				fprintf(stderr, "A multiple declaration of variable %s at line %d\n", name, lineno);
 				exit(1);
			}
			// if in another scope - create a new entry
			else{
				/* set up entry */
				l = (listNode*) malloc(sizeof(listNode));
				strncpy(l->st_name, name, len);  
				l->st_type = type;
				l->scope = cur_scope;
				l->lines = (RefList*) malloc(sizeof(RefList));
				l->lines->lineno = lineno;
				l->lines->next = NULL;
				
				/* add to hashtable */
				l->next = hash_table[hashval];
				hash_table[hashval] = l; 
				printf("Inserted %s for a new scope with linenumber %d!\n", name, lineno);
			}
		}
	}
}

listNode *lookup(char *name){ /* return symbol if found or NULL if not found */
	unsigned int hashval = hash(name);
	listNode *l = hash_table[hashval];
	while ((l != NULL) && (strcmp(name,l->st_name) != 0)) l = l->next;
	return l; // NULL is not found
}

listNode *lookup_scope(char *name, int scope){ /* return symbol if found or NULL if not found */
	unsigned int hashval = hash(name);
	listNode *l = hash_table[hashval];
	while ((l != NULL) && (strcmp(name,l->st_name) != 0) && (scope != l->scope)) l = l->next;
	return l; // NULL is not found
}

void hide_scope(){ /* hide the current scope */
	listNode *l;
	printf("Hiding scope %d: \n", cur_scope);

	/* for all the lists */
	for (int i = 0; i < SIZE; i++){
		if(hash_table[i] != NULL){
			l = hash_table[i];
			/* Find the first item that is from another scope */
			while(l != NULL && l->scope == cur_scope){
				printf("Hiding %s..\n", l->st_name);
				l = l->next;
			}
			/* Set the list equal to that item */
			hash_table[i] = l;
		}
	}
	cur_scope--;
}

void incr_scope(){ /* go to next scope */
	cur_scope++;
}

/* print to stdout by default */ 
void symtab_dump(FILE * of){  
  int i;
  fprintf(of,"------------ ------ ------------\n");
  fprintf(of,"Name         Type   Line Numbers\n");
  fprintf(of,"------------ ------ -------------\n");
  for (i=0; i < SIZE; ++i){ 
	if (hash_table[i] != NULL){ 
		listNode *l = hash_table[i];
		while (l != NULL){ 
			RefList *t = l->lines;
			fprintf(of,"%-12s ",l->st_name);
			if (l->st_type == INT_TYPE) fprintf(of,"%-7s","int");
			else if (l->st_type == REAL_TYPE) fprintf(of,"%-7s","real");
			else if (l->st_type == STR_TYPE) fprintf(of,"%-7s","string");
			else if (l->st_type == ARRAY_TYPE){
				fprintf(of,"array of ");
				if (l->inf_type == INT_TYPE) 		   fprintf(of,"%-7s","int");
				else if (l->inf_type  == REAL_TYPE)    fprintf(of,"%-7s","real");
				else if (l->inf_type  == STR_TYPE) 	   fprintf(of,"%-7s","string");
				else fprintf(of,"%-7s","undef");
			}
			else if (l->st_type == FUNCTION_TYPE){
				fprintf(of,"%-7s","function returns ");
				if (l->inf_type == INT_TYPE) 		   fprintf(of,"%-7s","int");
				else if (l->inf_type  == REAL_TYPE)    fprintf(of,"%-7s","real");
				else if (l->inf_type  == STR_TYPE) 	   fprintf(of,"%-7s","string");
				else fprintf(of,"%-7s","undef");
			}
			else fprintf(of,"%-7s","undef"); // if UNDEF or 0
			while (t != NULL){
				fprintf(of,"%4d ",t->lineno);
			t = t->next;
			}
			fprintf(of,"\n");
			l = l->next;
		}
    }
  }
}

// set the type of an entry (declaration)
void set_type(char *name, int st_type, int inf_type){

	/* lookup entry */
	listNode *l = lookup(name);
	
	/* set "main" type */
	l->st_type = st_type;
	
	/* if array, pointer or function */
	if(inf_type != UNDEF){
		l->inf_type = inf_type;
	}	
} 

// get the type of an entry
int get_type(char *name){

	/* lookup entry */
	listNode *l = lookup(name);
	
	/* if "simple" type */
	if(l->st_type == INT_TYPE || l->st_type == REAL_TYPE || l->st_type == CHAR_TYPE){
		return l->st_type;
	}
	/* if array, pointer or function */
	else{
		return l->inf_type;
	}
}

// Function Declaration and Parameters

Parameter def_param(int par_type, char *param_name, int passing){ // define parameter
	Parameter param; /* Parameter struct */
	
	/* set the information */
	param.par_type = par_type;
	strcpy(param.param_name, param_name);
	param.passing = passing;
	
	/* return the structure */
	return param;
}

int func_declare(char *name, int ret_type, int num_of_pars, Parameter *parameters){ // declare function
	/* lookup entry */
	listNode *l = lookup(name);
	
	/* if type is not defined yet */
	if(l->st_type != UNDEF){
		/* entry is of function type */
		l->st_type = FUNCTION_TYPE;
		
		/* return type is ret_type */
		l->inf_type = ret_type;
		
		/* parameter stuff */
		l->num_of_pars = num_of_pars;
		l->parameters = parameters;
		
		return 0; /* success */
	}
	/* already declared error */
	else{
		fprintf(stderr, "Function %s already declared!\n", name);
		exit(1);
	}
}

int func_param_check(char *name, int num_of_pars, Parameter*parameters){ // check parameters
	int i, type_1, type_2;
	
	/* lookup entry */
	listNode *l = lookup(name);
	
	/* check number of parameters */
	if(l->num_of_pars != num_of_pars){
		fprintf(stderr, "Function call of %s has wrong num of parameters!\n", name);
		exit(1);
	}
	
	/* check if parameters are compatible */
	for(i = 0; i < num_of_pars; i++){
		/* type of parameter in function declaration */
		type_1 = l->parameters[i].par_type; 
		
		/* type of parameter in function call*/
		type_2 = parameters[i].par_type; 
		
		/* check compatibility for function call */
		get_result_type(type_1, type_2, NONE);
		/* error occurs automatically in the function */
	}
	
	return 0; /* success */
}
