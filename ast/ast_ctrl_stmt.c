

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


ASTNode *newASTWhenNode(ASTNode *conditon_expr, ASTNode *when_branch){

    ASTNodeWhen *val = malloc(sizeof(ASTNodeWhen));

    val->type = WHEN_NODE;
    val->condition_expr = conditon_expr;
    val->when_branch = when_branch;

    return (struct ASTNode*)val;
}


ASTNode *newASTWhenBodyNode(ASTNode **entries, ASTNode *else_branch, int entries_count){

    ASTNodeWhenBody *val = malloc(sizeof(ASTNodeWhenBody));

    val->type = WHEN_BODY_NODE;
    val->when_entries = entries;
    val->else_branch = else_branch;

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