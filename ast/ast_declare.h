
typedef struct ASTNodeDecl{

    enum Node_Type type; // node type

	// data type
	int data_type;

	// symbol table entries of the variables
	listNode** names;
	int names_count;
    
}ASTNodeDecl;

ASTNode *newASTDeclNode(int data_type, listNode** names, int names_count);