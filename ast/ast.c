#include"ast.h"

ASTNode *newASTNode(Node_Type *type, ASTNode *left, ASTNode *right){

    ASTNode *val = (ASTNode*)malloc(sizeof(ASTNode));

    val->type = type;
    val->left = left;
    val->right = right;

    return val;
}