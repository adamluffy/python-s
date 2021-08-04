#include<stdio.h>
#include"semantic.h"
#include"symtab.h"
#include"ast/ast.c"


int main(){

    Value val1, val2;

    val1.ival = 1;
    val2.ival = 2;

    ASTNode *const_node_1 = newASTConstNode(INT_TYPE, val1);
    ASTNode *const_node_2 = newASTConstNode(INT_TYPE, val2);
    ASTNode *bool_node = newASTBoolNode(OR,const_node_1,const_node_2);
    ASTNode *simple = newASTSimpleNode(0);
    ASTNode *if_node = newASTIfNode(bool_node,simple,NULL,0,NULL);
    astTraversal(if_node);
}