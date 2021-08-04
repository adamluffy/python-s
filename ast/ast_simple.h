

// Node for assignment
typedef struct ASTNodeAssign{

    enum Node_Type type;

    // symbol table entry
    listNode *entry;

    struct ASTNode *assign_val;

}ASTNodeAssign;


// Node for Simple statement (break or continue)
typedef struct ASTNodeSimple{

    enum Node_Type type;

    // continue, break
    int statement_type;

}ASTNodeSimple;


// Node for increment
typedef struct ASTNodeIncr{

    enum Node_Type type;

    listNode *entry;

    // increment or decrement;
    int incr_type;

    // post fix or prefix
    int pf_type; // 0: postfix, 1: prefix

}ASTNodeIncr;

ASTNode *newASTAssignNode(listNode *entry, ASTNode *assign_val);
ASTNode *newASTSimpleNode(int statement_type);
ASTNode *newASTIncrNode(listNode *entry, int incr_type, int pf_type);