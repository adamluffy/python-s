ASTNode *newASTVariableNode(listNode *entry){

    ASTNodeVariable *val = malloc(sizeof(ASTNodeVariable));

    val->type = VARIABLE_NODE;
    val->symtab_item = entry;

    return (struct ASTNode*)val;
}