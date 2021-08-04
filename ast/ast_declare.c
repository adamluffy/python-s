

ASTNode *newASTNodeDecl(int data_type, listNode** names){

    ASTNodeDecl *val = malloc(sizeof(ASTNodeDecl));

    val->type = DECL_NODE;
    val->data_type = data_type;
    val->names = names;

    return (struct ASTNode*)val;
}