
// node for function call
typedef struct ASTNodeFuncCall{

    enum Node_Type type;

    // function identifier
    listNode *entry;

    // call parameters;
    struct ASTNode **params;
    int num_of_pars;

}ASTNodeFuncCall;


// node for function declaration
typedef struct ASTNodeFuncDecl{

    enum Node_Type type;

    // return type;
    int ret_type;

    // symbol table entry
    listNode *entry;

    // declarations, statements and return
	struct ASTNode *declarations;
	struct ASTNode *statements;
	struct ASTNode *return_node;

}ASTNodeFuncDecl;


// node for function parameter declarations
typedef struct ASTNodeDeclParams{
	
    enum Node_Type type; // node type

	// parameters
	Parameter *parameters;
	int num_of_pars;

}ASTNodeDeclParams;


// node for return type
typedef struct ASTNodeRetType{

    enum Node_Type type; // node type

	// return type
	int ret_type;

}ASTNodeRetType;


// node for return
typedef struct ASTNodeReturn{

    enum Node_Type type;

    // return type;
    int ret_type;

    // return value;
    struct ASTNode *ret_val;
    
}ASTNodeReturn;

ASTNode *newASTFuncCallNode(listNode *entry, ASTNode **params, int num_of_pars);
ASTNode *newASTFuncDeclNode(int ret_type, listNode *entry);
ASTNode *newASTDeclParamsNode(Parameter *parameters, int num_of_pars, Parameter param);
ASTNode *newASTRetTypeNode(int ret_type);
ASTNode *newASTReturnNode(int ret_type, ASTNode *ret_val);