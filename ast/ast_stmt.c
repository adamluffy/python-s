ASTNode *newASTStatementsNode(ASTNode **statements, 
            int statement_count, ASTNode *statement){

    
    ASTNodeStatements *val = malloc(sizeof(newASTStatementsNode));

    val->type = STATEMENTS;

    if(statements == NULL){
        statements = (ASTNode**)malloc(sizeof(ASTNode*));
        statements[0] = statement;
        statement_count = 1;
    }else{
        statements = (ASTNode**)realloc(statements, 
            (statement_count+1)*sizeof(ASTNode*));
        statements[statement_count] = statement;
        statement_count++;
    }

    val->statements = statements;
    val->statement_count = statement_count;

    return (struct ASTNode*)val;
}