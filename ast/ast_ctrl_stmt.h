

// Node for if statement
typedef struct ASTNodeIf{

    enum Node_Type type;

    //condition
    struct ASTNode *condition;

    // body
    struct ASTNode *if_branch;

    // elif branches
    struct ASTNode **elif_branch;
    int elif_count;

    // else branch
    struct ASTNode *else_branch;

}ASTNodeIf;


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
    struct ASTNode *else_branch;

}ASTNodeWhen;


// Node for when body
typedef struct ASTNodeWhenBody{

    enum Node_Type type;

    // body 
    struct ASTNode **when_entries;
    int entries_count;

    

}ASTNodeWhenBody;

// Node for when condition expressions
typedef struct ASTNodeWhenConds{

    enum Node_Type type;

    struct ASTNode **expressions;
    int expr_count;

}ASTNodeWhenConds;


// Node for single entry
typedef struct ASTNodeWhenEntry{

    enum Node_Type type;

    // constant values or boolean expression
    struct ASTNode **expressions;
    int expr_count;

    //body
    struct ASTNode *entry_branch;
    

}ASTNodeWhenEntry;



ASTNode *newASTIfNode(ASTNode *condition, ASTNode *if_branch, ASTNode **elsif_branches, 
	int elseif_count, ASTNode *else_branch);
ASTNode *newASTElsifNode(ASTNode *condition, ASTNode *elsif_branch);

ASTNode *newASTWhenNode(ASTNode *conditon_expr, ASTNode *when_branch ,ASTNode *else_branch);
ASTNode *newASTWhenBodyNode(ASTNode **entries,  int entries_count);
ASTNode *newASTWhenEntryNode(ASTNode **expressions, int expr_count, ASTNode *entry_branch);
ASTNode *newASTWhenCondsNode(ASTNode **expressions, int expr_count, ASTNode *expression);