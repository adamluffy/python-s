

ASTNode *newASTForNode(ASTNode *condition, ASTNode *for_branch){
    

    ASTNodeFor *val = malloc(sizeof(ASTNodeFor));
    
    val->type = FOR_NODE;
    val->condition = condition;
    val->for_branch = for_branch;
    
    return (struct ASTNode*)val;
}

ASTNode *newASTForConditionNode(int condition_type, ASTNode *initialize,
            ASTNode *condition, ASTNode *increment){

    
    ASTNodeForCondition *val = malloc(sizeof(ASTNodeForCondition));

    val->type = FOR_CONDITION_NODE;
    val->condition_type = condition_type;
    val->initialize = initialize;
    val->condition = condition;
    val->increment = increment;

    return (struct ASTNode*)val;
}


ASTNode *newASTWhileNode(ASTNode *condition, ASTNode *while_branch){

    ASTNodeWhile *val = malloc(sizeof(ASTNodeWhile));

    val->type = WHILE_NODE;
    val->condition = condition;
    val->while_branch = while_branch;

    return (struct ASTNode*)val;
}

// set the loop counter 
void set_loop_counter(ASTNode *node){

   ASTNodeForCondition *for_condition = (ASTNodeForCondition*)node;

   switch (for_condition->condition_type)
   {
      case 0: ;

        /* find the counter */
        ASTNodeAssign *assign_node = (ASTNodeAssign *) for_condition->initialize;
        for_condition->counter = assign_node->entry;

        /* check if the same one occurs in increment! */
        ASTNodeIncr *incr_node = (ASTNodeIncr *) for_condition->increment;
        if( strcmp(incr_node->entry->st_name, assign_node->entry->st_name) ){
          fprintf(stderr, "Variable used in init and incr of for are not the same!\n");
          exit(1);
        }
        break;
   
      case 1: ;

        ASTNodeIn *in_node = (ASTNodeIn*)for_condition->condition;
        ASTNodeVariable *var = (ASTNodeVariable*)in_node->left;
        for_condition->counter = var->symtab_item;
        break;
   }

   node = (ASTNode*)for_condition;
}
