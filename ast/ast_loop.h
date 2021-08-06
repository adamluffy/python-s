

// Node for 'for statement'
typedef struct ASTNodeFor{

    enum Node_Type type;

    // condition
    struct ASTNode *condition;
   
    // body
    struct ASTNode *for_branch;

    

}ASTNodeFor;


// Node for condition
typedef struct ASTNodeForCondition
{
    enum Node_Type type;

    int condition_type; // 0: initialize type, 1: in range type

     // intialization
    struct ASTNode *initialize;

    // condition
    struct ASTNode *condition;

    // increment
    struct ASTNode *increment;

    // loop counter variable
    listNode *counter;

}ASTNodeForCondition;



// Node for While statement
typedef struct ASTNodeWhile {

    enum Node_Type type;

    // condition
    struct ASTNode *condition;

    // body
    struct ASTNode *while_branch;

}ASTNodeWhile;

ASTNode *newASTForNode(ASTNode *condition, ASTNode *for_branch);
ASTNode *newASTForConditionNode(int condition_type, ASTNode *initialize,
            ASTNode *condition, ASTNode *increment);
ASTNode *newASTWhileNode(ASTNode *condition, ASTNode *while_branch);

void set_loop_counter(ASTNode *node);