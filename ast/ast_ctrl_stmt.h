#include "ast.h"

// Node for if statement
typedef struct ASTNodeIfStmt{

    enum Node_Type type;

    //condition
    struct ASTNode *condition;

    // body
    struct ASTNode *if_branch;

    // elif branches
    struct ASTNode **elif_branch;

    // else branch
    struct ASTNode *else_branch;

}ASTNodeIfStmt;


// Node for elif statement
typedef struct ASTNodeElif{

    enum Node_Type type;

    struct ASTNode *condition;

    struct ASTNode *elif_branch;

}ASTNodeElif;


// Node for when statement
typedef struct ASTNodeWhen{

    enum Node_Type type;

    // constant expression (constant value)
    struct ASTNode *condition_expr;

    // body
    struct ASTNode *when_branch;


}ASTNodeWhen;


// Node for when body
typedef struct ASTNodeWhenBody{

    enum Node_Type type;

    // body 
    struct ASTNode **when_entries;

    struct ASTNode *else_branch;

}ASTNodeWhenBody;


// Node for single entry
typedef struct ASTNodeWhenEntry{

    enum Node_Type type;

    // constant values or boolean expression
    struct ASTNode **expressions;

    //body
    struct ASTNode *entry_branch;

}ASTNodeWhenEntry;