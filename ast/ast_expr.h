

// Node for arithmetic operation
typedef struct ASTNodeArithm{

    enum Node_Type type;

    // data type result
    int data_type;

    // operator
    enum Arithm_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right; // left child

}ASTNodeArithm;


// node for boolean expression
typedef struct ASTNodeBool{

    enum Node_Type type;

    // data type result
    int data_type;

    // operator 
    enum Bool_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right;  //right child

}ASTNodeBool;


// node for relation expression
typedef struct ASTNodeRel{

    enum Node_Type type;

    int data_type;

    // operator 
    enum Relation_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right;  //right child

}ASTNodeRel;


// node for equality expression
typedef struct ASTNodeEql{

    enum Node_Type type;

    int data_type;

    // operator 
    enum Equal_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right;  //right child

}ASTNodeEql;


// node for in expression to check if 
// the lhs is an element of rhs element
typedef struct ASTNodeIn{

    enum Node_Type type;

    int data_type;

    // operator
    enum In_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right;  //right child

}ASTNodeIn;


// node for creating a range of number or 
// characters
typedef struct ASTNodeRange{

    enum Node_Type type;

    int data_type;
    
    // operator
    enum Range_op op;

    struct ASTNode *left; // left child
    struct ASTNode *right;  //right child

}ASTNodeRange;


ASTNode *newASTArithmNode(enum Arithm_op op, ASTNode *left, ASTNode *right);
ASTNode *newASTBoolNode(enum Bool_op op, ASTNode *left, ASTNode *right);
ASTNode *newASTRelNode(enum Relation_op op, ASTNode *left, ASTNode *right);
ASTNode *newASTEqlNode(enum Equal_op op, ASTNode *left, ASTNode *right);
ASTNode *newASTInNode(enum In_op op, ASTNode *left, ASTNode *right);
ASTNode *newASTRangeNode(enum Range_op op, ASTNode *left, ASTNode *right);