#include"ast/ast.h"

/* maximum size of hash table */
#define SIZE 233

/* maximum size of tokens-identifiers */
#define MAXTOKENLEN 60

/* how parameter is passed */
#define BY_VALUE 1
#define BY_REFER 2

/* parameter struct */
typedef struct Parameter{
	int par_type;
	char param_name[MAXTOKENLEN];
	// to store value
	Value val;
	int passing; // value or reference
}Parameter;

/* a linked list of references (lineno's) for each variable */
typedef struct RefList{ 
    int lineno;
    struct RefList *next;
    int type;
}RefList;

// struct that represents a list node
typedef struct listNode{
	char st_name[MAXTOKENLEN];
    int st_size;
    int scope;
    RefList *lines;
	// to store value and sometimes more information
	Value val;
	// type
    int st_type;
	int inf_type; // for arrays (info type) and functions (return type)
	// array stuff
	Value *vals;
	int array_size;
	// function parameters
	Parameter *parameters;
	int num_of_pars;
	// pointer to next item in the list
	struct listNode *next;
}listNode;

typedef struct revisitQueue{

	// name of identifier
    char *st_name;
	
    // type of revisit
    int revisit_type;

    // maybe additional information to simplify the process ...

    struct revisitQueue *next;

}revisitQueue;

#define PARAM_CHECK 1 /* Check parameters of function call when functions gets declared */

/* the hash table */
static listNode **hash_table;
static revisitQueue *queue;

// Function Declarations
void init_hash_table(); // initialize hash table
unsigned int hash(char *key); // hash function 
void insert(char *name, int len, int type, int lineno); // insert entry
listNode *lookup(char *name); // search for entry
void symtab_dump(FILE *of); // dump file

// Scope management
void hide_scope(); // hide the current scope
void incr_scope(); // go to next scope

// Type function
void set_type(char *name, int st_type, int inf_type); // set the type of an entry (declaration)
int get_type(char *name); // get the type of an entry

// Function Declaration and Parameters
Parameter def_param(int par_type, char *param_name, int passing); // define parameter
int func_declare(char *name, int ret_type, int num_of_pars, Parameter *parameters); // declare function
int func_param_check(char *name, int num_of_pars, Parameter *parameters); // check parameters

// Revisit Queue function
void add_to_queue(char *name, int type); // add to queue
int revisit(char *name); // revisit entry by also removing it from queue
void revisit_dump(FILE *of); // dump file
