
ASTNode *newASTArithmNode(enum Arithm_op op, ASTNode *left, ASTNode *right){

    ASTNodeArithm *val = malloc(sizeof(ASTNodeArithm));

    val->type = ARITHM_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    val->data_type = get_result_type(
        expressionDataType(left),
        expressionDataType(right),
        ARITHM_OP
    );

    return (struct ASTNode*)val;
}


ASTNode *newASTBoolNode(enum Bool_op op, ASTNode *left, ASTNode *right){

    ASTNodeBool *val = malloc(sizeof(ASTNodeBool));

    val->type = BOOL_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    if(val->op != NOT){

        val->data_type = get_result_type(
            expressionDataType(left),
            expressionDataType(right),
            BOOL_OP
        );  
    }else{

        val->data_type = get_result_type(
            expressionDataType(left),
            UNDEF,
            NOT_OP
        );
    }

    return (struct ASTNode*)val;
}


ASTNode *newASTRelNode(enum Relation_op op, ASTNode *left, ASTNode *right){

    ASTNodeRel *val = malloc(sizeof(ASTNodeRel));

    val->type = REL_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    val->data_type = get_result_type(
            expressionDataType(left),
            expressionDataType(right),
            REL_OP
        );

    return (struct ASTNode*)val;
}


ASTNode *newASTEqlNode(enum Equal_op op, ASTNode *left, ASTNode *right){

    ASTNodeEql *val = malloc(sizeof(ASTNodeEql));

    val->type = EQU_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    val->data_type = get_result_type(
            expressionDataType(left),
            expressionDataType(right),
            EQU_OP
        );

    return (struct ASTNode*)val;
}


ASTNode *newASTInNode(enum In_op op, ASTNode *left, ASTNode *right){

    ASTNodeIn *val = malloc(sizeof(ASTNodeIn));

    val->type = IN_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    val->data_type = get_result_type(
             expressionDataType(left),
             expressionDataType(right),
             IN_OP
         );

    return (struct ASTNode*)val;
}


ASTNode *newASTRangeNode(enum Range_op op, ASTNode *left, ASTNode *right){

    ASTNodeRange *val = malloc(sizeof(ASTNodeRange));

    val->type = RANGE_NODE;
    val->op = op;
    val->left = left;
    val->right = right;

    val->data_type = get_result_type(
            expressionDataType(left),
            expressionDataType(right),
            RANGE_OP
        );

    return (struct ASTNode*)val;
}