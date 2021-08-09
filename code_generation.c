#include"code_generation.h"

void generate_code(){

	FILE *fp;

	fp = fopen("out.asm", "w");

	generate_data_intialization(fp);
    generate_data_declarations(fp);

	fclose(fp);
}

void generate_data_intialization(FILE *fp){

	 /* print .data */
    fprintf(fp, "section .data\n");	

    /* loop through the symbol table's lists */
    int i;
    for (i = 0; i < SIZE; i++){ 
        /* if hashtable list not empty */
        if (hash_table[i] != NULL){ 
            listNode *l = hash_table[i];
            /* loop through list */
            while (l != NULL){ 
                /* Simple Variables */
                if (l->st_type == INT_TYPE){
                    fprintf(fp, "%s dd %d\n", l->st_name, l->val.ival);
                }
                else if (l->st_type == REAL_TYPE){
                    fprintf(fp, "%s dd %f\n", l->st_name, l->val.fval);
                }
                else if (l->st_type == CHAR_TYPE){
                    fprintf(fp, "%s db \'%c\'\n", l->st_name, l->val.cval);
                }

                /* Array */
                else if (l->st_type == ARRAY_TYPE && l->vals != NULL){
                    if (l->inf_type == INT_TYPE){
                        fprintf(fp, "%s dd %d\n", l->st_name, l->array_size);
                    }
                    else if (l->inf_type  == REAL_TYPE){
                        fprintf(fp, "%s dd %d\n", l->st_name, l->array_size);
                    }
                    else if (l->inf_type  == CHAR_TYPE){
                        fprintf(fp, "%s db %d\n", l->st_name, l->array_size);
                    }
                }

                /* Function (ignore) */
                else if (l->st_type == FUNCTION_TYPE){

                }

                l = l->next;
            }
        }
    }

    fprintf(fp, "\n\n");
}

void generate_data_declarations(FILE *fp){

    /* print .data */
    fprintf(fp, "section .bss\n");	

    /* loop through the symbol table's lists */
    int i;
    for (i = 0; i < SIZE; i++){ 
        /* if hashtable list not empty */
        if (hash_table[i] != NULL){ 
            listNode *l = hash_table[i];
            /* loop through list */
            while (l != NULL){ 

                /* Array */
                if (l->st_type == ARRAY_TYPE && l->vals == NULL){
                    if (l->inf_type == INT_TYPE){
                        fprintf(fp, "%s dd %d\n", l->st_name, l->array_size);
                    }
                    else if (l->inf_type  == REAL_TYPE){
                        fprintf(fp, "%s dd %d\n", l->st_name, l->array_size);
                    }
                    else if (l->inf_type  == CHAR_TYPE){
                        fprintf(fp, "%s db %d\n", l->st_name, l->array_size);
                    }
                }

                /* Function (ignore) */
                else if (l->st_type == FUNCTION_TYPE){

                }

                l = l->next;
            }
        }
    }
}