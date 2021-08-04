#include "ast.h"

// Node for 'for statement'
typedef struct ASTNodeFor{

    enum Node_Type type;

    // intialization
    struct ASTNode *initialize;

    // condtion
    struct ASTNode *condition;

    // increment
    struct ASTNode *increment;

    // body
    struct ASTNode *for_branch;

}ASTNodeFor;


// Node for While statement
typedef struct ASTNodeWhile {

    enum Node_Type type;

    // condition
    struct ASTNode *condition;

    // body
    struct ASTNode *while_branch;

}ASTNodeWhile;