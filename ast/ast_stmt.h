//node for statements
typedef struct ASTNodeStatements
{
    enum Node_Type type;

    struct ASTNode **statements;
    int statement_count;

}ASTNodeStatements;

ASTNode *newASTStatementsNode(ASTNode **statements, 
            int statement_count, ASTNode *statement);