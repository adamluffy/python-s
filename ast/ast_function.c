


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


ASTNode *newASTDeclParamsNode(Parameter *parameters, int num_of_pars, Parameter param){

    ASTNodeDeclParams *val = malloc(sizeof(ASTNodeDeclParams));

    val->type = DECL_PARAMS;

    if(parameters == NULL){
        parameters = (Parameter*)malloc(sizeof(Parameter));
        parameters[0] = param;
        num_of_pars = 1;
    }else{
        parameters = (Parameter*)realloc(parameters,
            (num_of_pars+1)*sizeof(Parameter));
        parameters[num_of_pars] = param;
        num_of_pars++;
    }

    val->parameters = parameters;
    val->num_of_pars = num_of_pars;

    return (struct ASTNode*)val;
}


ASTNode *newASTRetTypeNode(int ret_type){

    ASTNodeRetType *val = malloc(sizeof(ASTNodeRetType));

    val->type = RET_TYPE;
    val->ret_type = ret_type;

    return (struct ASTNode*)val;
}

ASTNode *newASTReturnNode(int ret_type, ASTNode *ret_val){

    ASTNodeReturn *val = malloc(sizeof(ASTNodeReturn));

    val->type = RETURN_NODE;
    val->ret_type = ret_type;
    val->ret_val = ret_val;

    return (struct ASTNode*)val;
}