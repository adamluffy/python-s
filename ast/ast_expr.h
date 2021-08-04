#include "ast.h"

// Node for arithmetic operation
typedef struct ASTNodeArithm{

    enum Node_Type type;

    // operator
    enum Arithm_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right; // left child

}ASTNodeArithm;


// node for boolean expression
typedef struct ASTNodeBool{

    enum Node_Type type;

    // operator 
    enum Bool_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right;  //right child

}ASTNodeBool;


// node for relation expression
typedef struct ASTNodeRel{

    enum Node_Type type;

    // operator 
    enum Relation_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right;  //right child

}ASTNodeRel;


// node for equality expression
typedef struct ASTNodeEql{

    enum Node_Type type;

    // operator 
    enum Equal_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right;  //right child

}ASTNodeEql;