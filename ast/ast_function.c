


ASTNode *newASTFuncCallNode(listNode *entry, ASTNode **params, int num_of_pars){

    ASTNodeFuncCall *val = malloc(sizeof(ASTNodeFuncCall));

    val->type = FUNC_CALL;
    val->entry = entry;
    val->params = params;
    val->num_of_pars = num_of_pars;

    return (struct ASTNode*)val;
}


ASTNode *newASTFuncDeclNode(int ret_type, listNode *entry){

    ASTNodeFuncDecl *val = malloc(sizeof(ASTNodeFuncDecl));

    val->type = FUNC_DECL;
    val->ret_type = ret_type;
    val->entry = entry;

    return (struct ASTNode*)val;
}


ASTNode *newASTReturnNode(int ret_type, ASTNode *ret_val){

    ASTNodeReturn *val = malloc(sizeof(ASTNodeReturn));

    val->type = RETURN_NODE;
    val->ret_type = ret_type;
    val->ret_val = ret_val;

    return (struct ASTNode*)val;
}