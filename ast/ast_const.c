#include "ast_const.h"

ASTNode *newASTConstNode(int const_type, Value val){

    ASTNodeConst *v = malloc(sizeof(ASTNodeConst));

    v->type = CONST_NODE;
    v->const_type = const_type;
    v->val = val;

    return (struct ASTNode*)v; 
}