typedef enum Node_Type{
    BASIC_NODE,  // no special usage (for roots only)
	// declarations
	DECL_NODE,   // declaration
	CONST_NODE,  // constant
	// statements
	IF_NODE,     // if statement
	ELSIF_NODE,  // else if branch
	FOR_NODE,    // for statement
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
	EQU_NODE,    // equality expression
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
    LESS_EQUAL,
    IN
}Relation_op;

typedef enum Equal_op{
    EQUAL,
    NOT_EQUAL
}Equal_op;


/**
 * Type value definition
 **/
typedef union  Value
{
    int ival;
    double fval;
    char cval;
    char *sval;
}Value;


// Abstract syntax tree structure 
typedef struct ASTNode
{
    enum Node_Type type;
    struct ASTNode *left;
    struct ASTNode *right;

}ASTNode;

ASTNode *newASTNode(Node_Type *type, ASTNode *left, ASTNode *right);
