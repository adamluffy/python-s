// Node for variable 
typedef struct ASTNodeVariable
{
    enum Node_Type type;

    listNode *symtab_item;

}ASTNodeVariable;

ASTNode *newASTVariableNode(listNode *entry);