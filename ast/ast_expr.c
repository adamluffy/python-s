
ASTNode *newASTArithmNode(enum Arithm_op op, ASTNode *left, ASTNode *right){

    ASTNodeArithm *val = malloc(sizeof(ASTNodeArithm));

    val->type = ARITHM_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    return (struct ASTNode*)val;
}


ASTNode *newASTBoolNode(enum Bool_op op, ASTNode *left, ASTNode *right){

    ASTNodeBool *val = malloc(sizeof(ASTNodeBool));

    val->type = BOOL_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    return (struct ASTNode*)val;
}


ASTNode *newASTRelNode(enum Relation_op op, ASTNode *left, ASTNode *right){

    ASTNodeRel *val = malloc(sizeof(ASTNodeRel));

    val->type = REL_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    return (struct ASTNode*)val;
}


ASTNode *newASTEqlNode(enum Equal_op op, ASTNode *left, ASTNode *right){

    ASTNodeEql *val = malloc(sizeof(ASTNodeEql));

    val->type = EQU_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    return (struct ASTNode*)val;
}


ASTNode *newASTInNode(enum In_op op, ASTNode *left, ASTNode *right){

    ASTNodeIn *val = malloc(sizeof(ASTNodeIn));

    val->type = IN_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    return (struct ASTNode*)val;
}


ASTNode *newASTRangeNode(enum Range_op op, ASTNode *left, ASTNode *right){

    ASTNodeRange *val = malloc(sizeof(ASTNodeRange));

    val->type = RANGE_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    return (struct ASTNode*)val;
}