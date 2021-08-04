

ASTNode *newASTForNode(ASTNode *initialize, ASTNode *condition, 
                    ASTNode *increment, ASTNode *for_branch){
    

    ASTNodeFor *val = malloc(sizeof(ASTNodeFor));
    
    val->type = FOR_NODE;
    val->initialize = initialize;
    val->condition = condition;
    val->increment = increment;
    val->for_branch = for_branch;
    
    return (struct ASTNode*)val;
}


ASTNode *newASTWhileNode(ASTNode *condition, ASTNode *while_branch){

    ASTNodeWhile *val = malloc(sizeof(ASTNodeWhile));

    val->type = WHILE_NODE;
    val->condition = condition;
    val->while_branch = while_branch;

    return (struct ASTNode*)val;
}
