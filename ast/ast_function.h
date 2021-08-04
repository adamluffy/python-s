#include "ast.h"
#include "symtab.h"

// node for function call
typedef struct ASTNodeFuncCall{

    enum Node_Type type;

    // function identifier
    listNode *entry;

    // call parameters;
    struct ASTNode **params;

}ASTNodeFuncCall;


// node for function declaration
typedef struct ASTNodeFuncDecl{

    enum Node_Type type;

    // return type;
    int ret_type;

    // symbol table entry
    listNode *entry;

}ASTNodeFuncDecl;


// node for return
typedef struct ASTNodeReturn{

    enum Node_Type type;

    // return type;
    int ret_type;

    // return value;
    struct ASTNode *ret_val;
    
}ASTNodeReturn;