

ASTNode *newASTIfNode(ASTNode *condition, ASTNode *if_branch, ASTNode **elsif_branches, 
	int elseif_count, ASTNode *else_branch){

    ASTNodeIf *val = malloc(sizeof(ASTNodeIf));
    val->type = IF_NODE;
    val->condition = condition;
    val->if_branch = if_branch;
    val->elif_branch = elsif_branches;
    val->elif_count = elseif_count;
    val->else_branch = else_branch;
    return (struct ASTNode*)val;
}


ASTNode *newASTElsifNode(ASTNode *condition, ASTNode *elsif_branch){

    ASTNodeElif *val = malloc(sizeof(ASTNodeElif));

    val->type = ELSIF_NODE;
    val->condition = condition;
    val->elif_branch = elsif_branch;

    return (struct ASTNode*)val;
}


ASTNode *newASTWhenNode(ASTNode *conditon_expr, ASTNode *when_branch, ASTNode *else_branch){

    ASTNodeWhen *val = malloc(sizeof(ASTNodeWhen));

    val->type = WHEN_NODE;
    val->condition_expr = conditon_expr;
    val->when_branch = when_branch;
    val->else_branch = else_branch;

    return (struct ASTNode*)val;
}


ASTNode *newASTWhenBodyNode(ASTNode **entries , int entries_count){

    ASTNodeWhenBody *val = malloc(sizeof(ASTNodeWhenBody));

    val->type = WHEN_BODY_NODE;
    val->when_entries = entries;
    val->entries_count = entries_count;

    return (struct ASTNode*)val;
}


ASTNode *newASTWhenEntryNode(ASTNode **expressions, int expr_count, ASTNode *entry_branch){

    ASTNodeWhenEntry *val = malloc(sizeof(ASTNodeWhenEntry));

    val->type = WHEN_ENTRY_NODE;
    val->expressions = expressions;
    val->entry_branch = entry_branch;
    val->expr_count = expr_count;

    return (struct ASTNode *)val;
}


ASTNode *newASTWhenCondsNode(ASTNode **expressions, int expr_count, ASTNode *expression){

    ASTNodeWhenConds *val = malloc(sizeof(ASTNodeWhenConds));

    val->type =  WHEN_CONDS_EXPR_NODE;

    if(expressions == NULL){
        expressions = (ASTNode**)malloc(sizeof(ASTNode*));
        expressions[0] = expression;
        expr_count = 1;
    }else{
        expressions = (ASTNode**)realloc(expressions,
            (expr_count+1)*sizeof(ASTNode*));
        expressions[expr_count] = expression;
        expr_count++;
    }

    val->expressions = expressions;
    val->expr_count = expr_count;

    return (struct ASTNode*)val;
}