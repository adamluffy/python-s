// type 
#define UNDEF 0
#define INT_TYPE 1
#define REAL_TYPE 2
#define CHAR_TYPE 3
#define ARRAY_TYPE 4
#define STR_TYPE 5
#define FUNCTION_TYPE 6

// operation type
#define NONE 0
#define ARITHM_OP 1 // ADDOP, MULOP, DIVOP (+, -, *, /)
#define INCR_OP 2   // INCR (++, --)
#define BOOL_OP 3   // OROP, ANDOP (or, and)
#define NOT_OP 4    // NOTOP (not)
#define REL_OP 5    // RELOP (>, <, >=, <=)
#define EQU_OP 6    // EQUOP (==, !=)
#define RANGE_OP 7  // RANGEOP (..)

int get_result_type(int type1, int type2, int opType);
void type_error(int type1, int type2, int opType);