

ASTNode *newASTDeclNode(int data_type, listNode** names, int name_count){

    ASTNodeDecl *val = malloc(sizeof(ASTNodeDecl));

    val->type = DECL_NODE;
    val->data_type = data_type;
    val->names = names;
    val->names_count = name_count;

    return (struct ASTNode*)val;
}