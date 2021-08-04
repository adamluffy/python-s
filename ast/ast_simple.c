

ASTNode *newASTAssignNode(listNode *entry, ASTNode *assign_val){

    ASTNodeAssign *val = malloc(sizeof(ASTNodeAssign));

    val->type = ASSIGN_NODE;
    val->entry = entry;
    val->assign_val = assign_val;

    return (struct ASTNode*)val;
}


ASTNode *newASTSimpleNode(int statement_type){

    ASTNodeSimple *val = malloc(sizeof(ASTNodeSimple));

    val->type = SIMPLE_NODE;
    val->statement_type = statement_type;

    return (struct ASTNode*)val;
}


ASTNode *newASTIncrNode(listNode *entry, int incr_type, int pf_type){

    ASTNodeIncr *val = malloc(sizeof(ASTNodeIncr));

    val->type = INCR_NODE;
    val->entry = entry;
    val->incr_type = incr_type;
    val->pf_type = pf_type;

    return (struct ASTNode*)val;
}