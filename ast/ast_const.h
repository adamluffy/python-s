#include "ast.h"

typedef struct ASTNodeConst
{
    enum Node_Type type;

    // data type
    int const_type;

    // value of the constant
    Value val;

}ASTNodeConst;

ASTNode *newASTConstNode(int const_type, Value val);
