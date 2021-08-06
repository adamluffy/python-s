typedef enum Node_Type{
    BASIC_NODE,  // no special usage (for roots only)
	// declarations
    STATEMENTS,
	DECL_NODE,   // declaration
	CONST_NODE,  // constant
	// statements
	IF_NODE,     // if statement
	ELSIF_NODE,  // else if branch
    WHEN_NODE,
    WHEN_BODY_NODE,
    WHEN_ENTRY_NODE,
	FOR_NODE,    // for statement
    FOR_CONDITION_NODE,
	WHILE_NODE,  // while statement
	ASSIGN_NODE, // assigment
	SIMPLE_NODE, // continue, break and "main" return statements
	INCR_NODE,   // increment statement (non-expression one)
	FUNC_CALL,   // function call
	// expressions
	ARITHM_NODE, // arithmetic expression
    // RANGE_NODE,   // range expression
	BOOL_NODE,   // boolean expression
	REL_NODE,    // relational expression
	EQU_NODE,    // equality 
    IN_NODE,    // is in expression
    VARIABLE_NODE,  // variable 
    RANGE_NODE, // range expression
	// functions
	FUNC_DECL,   // function declaration
	RETURN_NODE // return statement of functions
}Node_Type;

/*
    Operation type definition
*/
typedef enum Arithm_op{
    ADD,
    SUB,
    MUL,
    DIV,
    INC,
    DEC
}Arithm_op;

typedef enum Bool_op{
    OR,
    AND,
    NOT
}Bool_op;

typedef enum Relation_op{
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL
}Relation_op;

typedef enum Equal_op{
    EQUAL,
    NOT_EQUAL
}Equal_op;


typedef enum In_op{
    INOP
}In_op;


typedef enum Range_op{
    TO
}Range_op;


/**
 * Type value definition
 **/
typedef union  Value
{
    int ival;
    double fval;
    char cval;
    char* sval;
}Value;


// Abstract syntax tree structure 
typedef struct ASTNode
{
    enum Node_Type type;
    struct ASTNode *left;
    struct ASTNode *right;

}ASTNode;

ASTNode *newASTNode(Node_Type type, ASTNode *left, ASTNode *right);
void astPrintNode(ASTNode *node);
void astTraversal(ASTNode *node);