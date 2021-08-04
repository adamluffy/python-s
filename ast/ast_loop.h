

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

ASTNode *newASTForNode(ASTNode *initialize, ASTNode *condition, 
                    ASTNode *increment, ASTNode *for_branch);
ASTNode *newASTWhileNode(ASTNode *condition, ASTNode *while_branch);
