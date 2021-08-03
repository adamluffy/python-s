#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

int get_result_type(int type1, int type2, int opType){
   
  	switch(opType){

         case NONE: /* type compatibility only, '1': compatible */
			// first type INT
			if(type1 == INT_TYPE){
				// second type INT or CHAR
				if(type2 == INT_TYPE || type2 == CHAR_TYPE){
					return 1;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			// first type REAL
			else if(type1 == REAL_TYPE){
				// second type INT, REAL or CHAR
				if(type2 == INT_TYPE || type2 == REAL_TYPE || type2 == CHAR_TYPE){
					return 1;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			// first type CHAR
			else if(type1 == CHAR_TYPE){
				// second type INT or CHAR
				if(type2 == INT_TYPE || type2 == CHAR_TYPE){
					return 1;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			break;
		/* ---------------------------------------------------------- */
		case ARITHM_OP: /* arithmetic operator */
			// first type INT
			if(type1 == INT_TYPE){
				// second type INT or CHAR
				if(type2 == INT_TYPE || type2 == CHAR_TYPE){
					return INT_TYPE;
				}
				// second type REAL
				else if(type2 == REAL_TYPE){
					return REAL_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			// first type REAL
			else if(type1 == REAL_TYPE){
				// second type INT, REAL or CHAR
				if(type2 == INT_TYPE || type2 == REAL_TYPE || type2 == CHAR_TYPE){
					return REAL_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			// first type CHAR
			else if(type1 == CHAR_TYPE){
				// second type INT or CHAR
				if(type2 == INT_TYPE || type2 == CHAR_TYPE){
					return CHAR_TYPE;
				}
				// second type REAL
				else if(type2 == REAL_TYPE){
					return REAL_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			else{
				type_error(type1, type2, opType);
			}
			break;
		/* ---------------------------------------------------------- */
		case INCR_OP: /* special case of INCR */
			// type INT
			if(type1 == INT_TYPE){
				return INT_TYPE;
			}
			// type REAL
			else if(type1 == REAL_TYPE){
				return REAL_TYPE;
			}
			// type CHAR
			else if(type1 == CHAR_TYPE){
				return CHAR_TYPE;
			}
			else{
				type_error(type1, type2, opType);
			}
			break;
		/* ---------------------------------------------------------- */
		case BOOL_OP: /* Boolean operator */
			// first type INT
			if(type1 == INT_TYPE){
				// second type INT or CHAR
				if(type2 == INT_TYPE || type2 == CHAR_TYPE){
					return INT_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			// first type CHAR
			else if(type1 == CHAR_TYPE){
				// second type INT or CHAR
				if(type2 == INT_TYPE || type2 == CHAR_TYPE){
					return CHAR_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			else{
				type_error(type1, type2, opType);
			}
			break;
		/* ---------------------------------------------------------- */
		case NOT_OP: /* special case of NOTOP */
			// type INT
			if(type1 == INT_TYPE){
				return INT_TYPE;
			}
			// type CHAR
			else if(type1 == CHAR_TYPE){
				return INT_TYPE;
			}
			else{
				type_error(type1, type2, opType);
			}
			break;
		/* ---------------------------------------------------------- */
		case REL_OP: /* Relational operator */
			// first type INT
			if(type1 == INT_TYPE){
				// second type INT, REAL or CHAR
				if(type2 == INT_TYPE || type2 == REAL_TYPE || type2 == CHAR_TYPE){
					return INT_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			else if(type1 == REAL_TYPE){
				// second type INT, REAL or CHAR
				if(type2 == INT_TYPE || type2 == REAL_TYPE || type2 == CHAR_TYPE){
					return INT_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			// first type CHAR
			else if(type1 == CHAR_TYPE){
				// second type INT, REAL or CHAR
				if(type2 == INT_TYPE || type2 == REAL_TYPE || type2 == CHAR_TYPE){
					return INT_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			else{
				type_error(type1, type2, opType);
			}
			break;
		/* ---------------------------------------------------------- */
		case EQU_OP: /* Equality operator */
			// first type INT
			if(type1 == INT_TYPE){
				// second type INT or CHAR
				if(type2 == INT_TYPE || type2 == CHAR_TYPE){
					return INT_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			else if(type1 == REAL_TYPE){
				// second type REAL
				if(type2 == REAL_TYPE){
					return INT_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			// first type CHAR
			else if(type1 == CHAR_TYPE){
				// second type INT or CHAR
				if(type2 == INT_TYPE || type2 == CHAR_TYPE){
					return INT_TYPE;
				}
				else{
					type_error(type1, type2, opType);
				}
			}
			else{
				type_error(type1, type2, opType);
			}
			break;

		case RANGE_OP:

			if(type1 == INT_TYPE){
				
				if(type2 == INT_TYPE) {
					return INT_TYPE;
				}
				else { 
					type_error(type1, type2, opType);
				}

			}else if(type2 == CHAR_TYPE){

				if(type2 == CHAR_TYPE) { 
					return CHAR_TYPE;
				}
				else {
					type_error(type1, type2, opType);
				}
			}

			break;

		/* ---------------------------------------------------------- */
		default: /* wrong choice case */
			fprintf(stderr, "Error in operator selection!\n");
			exit(1);
    }
}

void type_error(int type_1, int type_2, int op_type){ /* print type error */
	fprintf(stderr, "Type conflict between %d and %d using op type %d\n", type_1, type_2, op_type);
	exit(1);
}