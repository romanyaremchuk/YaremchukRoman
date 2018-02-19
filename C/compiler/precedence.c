/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: precedence.c
 * Autori: Masarova Maria <xmasar13>
 *         Pristas Jan <xprist06>
 */

#include "precedence.h"

tStack *stack = NULL;
tQueue *queue = NULL;
int a;
int b;
int tmp_a = 0;
int tmp_b = 0;
int count = 0;
int tmp;
int type_flag; //0 - int, 1 - double, 2 - string

int tab[15][15] =
{
            //  +      -      *      /      \\     <      >      =      <=     >=    <>      (      )      i      $
    /* + */ { TMORE, TMORE, TLESS, TLESS, TLESS, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TLESS, TMORE, TLESS, TMORE },
    /* - */ { TMORE, TMORE, TLESS, TLESS, TLESS, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TLESS, TMORE, TLESS, TMORE },
    /* * */ { TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TLESS, TMORE, TLESS, TMORE },
    /* / */ { TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TLESS, TMORE, TLESS, TMORE },
    /* \ */ { TMORE, TMORE, TLESS, TLESS, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TLESS, TMORE, TLESS, TMORE },
    /* < */ { TLESS, TLESS, TLESS, TLESS, TLESS, TX,    TX,    TX,    TX,    TX,    TX,    TLESS, TMORE, TLESS, TMORE },
    /* > */ { TLESS, TLESS, TLESS, TLESS, TLESS, TX,    TX,    TX,    TX,    TX,    TX,    TLESS, TMORE, TLESS, TMORE },
    /* = */ { TLESS, TLESS, TLESS, TLESS, TLESS, TX,    TX,    TX,    TX,    TX,    TX,    TLESS, TMORE, TLESS, TMORE },
    /* <= */{ TLESS, TLESS, TLESS, TLESS, TLESS, TX,    TX,    TX,    TX,    TX,    TX,    TLESS, TMORE, TLESS, TMORE },
    /* >= */{ TLESS, TLESS, TLESS, TLESS, TLESS, TX,    TX,    TX,    TX,    TX,    TX,    TLESS, TMORE, TLESS, TMORE },
    /* <> */{ TLESS, TLESS, TLESS, TLESS, TLESS, TX,    TX,    TX,    TX,    TX,    TX,    TLESS, TMORE, TLESS, TMORE },
    /* ( */ { TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TEQL,  TLESS, TX    },
    /* ) */ { TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TX,    TMORE, TX,    TMORE },
    /* i */ { TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TMORE, TX,    TMORE, TX,    TMORE },
    /* $ */ { TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TLESS, TX,    TLESS, TX    }
};


int precedence(int b_flag, SToken token, char * function_name, tTable * glob_table, union v_f data, int variable_type, char * variable_name) {
   //  int if_a = 0;
   //  int if_b = 0;
  	// int pomoc_a=5;
   //  int pomoc_b=5;

    int in_param = 0;
    int count_of_bool = 0;
    queue = (tQueue*)malloc(sizeof(tQueue) * MAX_QUEUE);
    queueInit(queue);

    if (queue == NULL) {
        free((void*) queue);
        error_print(EIN);
        exit(EIN);
    }

    while ((token.type != token_eol) && (token.type != token_then) && (token.type != token_semicolom)) {     
        switch (token.type) {
            case token_plus:
                queuePush(queue, EADD);
                break;
            case token_minus:
                queuePush(queue, ESUB);
                break;
            case token_mul:
                queuePush(queue, EMUL);
                break;
            case token_slash:
                queuePush(queue, EDIV);
                break;
            case token_mod:
                queuePush(queue, EMOD);
                break;
            case token_less:
                if (b_flag == 0){
                    exit(2);
                }
                queuePush(queue, ELESS);
                count_of_bool++;
                break;
            case token_greater:
                if (b_flag == 0){
                    exit(2);
                }
                queuePush(queue, EGRT);
                count_of_bool++;
                break;
            case token_assignment:
                if (b_flag == 0){
                    exit(2);
                }
                queuePush(queue, EEQL);
                count_of_bool++;
                break;
            case token_lessOrEq:
                if (b_flag == 0){
                    exit(2);
                }
                queuePush(queue, EEQLL);
                count_of_bool++;
                break;
            case token_greaOrEq:
                if (b_flag == 0){
                    exit(2);
                }
                queuePush(queue, EEQLG);
                count_of_bool++;
                break;
            case token_notEqual:
                if (b_flag == 0){
                    exit(2);
                }
                count_of_bool++;
                queuePush(queue, EDIF);
                break;
            case token_leftBracket:
                queuePush(queue, ELB);
                break;
            case token_rightBracket:
                queuePush(queue, ERB);
                break;
            case token_id:
                ;
                /*************************************************************/
                for(int i = 0; i < MAX_SIZE; i++){ //volanie funkcie
                    tItem * ptr = glob_table->ptr[i];
                    if (ptr != NULL) {
                        if (strcmp(ptr->key, token.name) == 0){
                            return token.type;
                        }
                    }      
                }

                tItem * tmp = tableSearch(glob_table, function_name);
                if (tmp == NULL){
                    exit(ESEM_DEF);
                }
                int rt = tmp->data.func->return_type;

                tItem * tmp2;
                tmp2 = tableSearch(tmp->data.func->local_table, token.name);
                if (tmp2 == NULL){
                    for (int k = 0; k < tmp->data.func->count_params; k++){
                        if ( strcmp(tmp->data.func->params[k]->param_name, token.name) == 0 ){
                            in_param = 1;
                            int vt = tmp->data.func->params[k]->type;
                            if ((rt == t_INT) && (vt == t_STRING)){
                                exit(ESEM_TKP);
                            } else if ((rt == t_DOUBLE) && (vt == t_STRING)){
                                exit(ESEM_TKP);
                            } else if ((rt == t_STRING) && (vt == t_INT)){
                                exit(ESEM_TKP);
                            } else if ((rt == t_STRING) && (vt == t_DOUBLE)){
                                exit(ESEM_TKP);
                            } 
                        } else {
                            if ((k == tmp->data.func->count_params -1) && (in_param == 0)){
                                exit(ESEM_DEF);
                            }
                        }
                    }
                } else {
                    int vt = tmp2->data.var->type;
                    int rt = variable_type;

                    if ((rt == t_INT) && (vt == t_STRING)){
                        exit(ESEM_TKP);
                    } else if ((rt == t_DOUBLE) && (vt == t_STRING)){
                        exit(ESEM_TKP);
                    } else if ((rt == t_STRING) && (vt == t_INT)){
                        exit(ESEM_TKP);
                    } else if ((rt == t_STRING) && (vt == t_DOUBLE)){
                        exit(ESEM_TKP);
                    }
                }
                in_param = 0;
                /*************************************************************/
                queuePush(queue, EI);
                break;
            case token_val:    
                if (b_flag == 0){
                    if ((strcmp(variable_name, "print") !=0 ) && (strcmp(variable_name, "input") !=0 ) && (strcmp(variable_name, "return") != 0)){
                        if (token.var_type == 0){
                            if (variable_type == t_INT){
                                tItem * tmp = tableSearch(glob_table, function_name);
                                if (tmp == NULL){
                                    exit(ESEM_DEF);
                                }

                                tItem * tmp2;
                                tmp2 = tableSearch(tmp->data.func->local_table, variable_name);

                                tmp2->data.var->val.int_val = token.value.number;

                                //tablePrint(glob_table);
                                
                            } else if (variable_type == t_DOUBLE){
                                tItem * tmp = tableSearch(glob_table, function_name);
                                if (tmp == NULL){
                                    exit(ESEM_DEF);
                                }

                                tItem * tmp2;
                                tmp2 = tableSearch(tmp->data.func->local_table, variable_name);

                                tmp2->data.var->val.double_val = (double)token.value.number_double;

                                //tablePrint(glob_table);
                            } else {
                                exit(ESEM_TKP);
                            }
                        } else if (token.var_type == 1){
                            if (variable_type == t_DOUBLE){
                                tItem * tmp = tableSearch(glob_table, function_name);
                                if (tmp == NULL){
                                    exit(ESEM_DEF);
                                }

                                tItem * tmp2;
                                tmp2 = tableSearch(tmp->data.func->local_table, variable_name);

                                tmp2->data.var->val.double_val = token.value.number_double;

                                //tablePrint(glob_table);
                                
                            } else if (variable_type == t_INT){
                                tItem * tmp = tableSearch(glob_table, function_name);
                                if (tmp == NULL){
                                    exit(ESEM_DEF);
                                }

                                tItem * tmp2;
                                tmp2 = tableSearch(tmp->data.func->local_table, variable_name);

                                tmp2->data.var->val.int_val = (int)token.value.number;

                                //tablePrint(glob_table);
                            } else {
                                exit(ESEM_TKP);
                            }
                        } 
                    }
                } else if (b_flag == 1){
                    ;
                }

                queuePush(queue, EI);
                break;
            case token_string:
                if (token.var_type == 2){
                    if ((strcmp(variable_name, "print") !=0 ) && (strcmp(variable_name, "input") !=0 ) && (strcmp(variable_name, "return") != 0)){
                        if (variable_type == t_STRING){
                            tItem * tmp = tableSearch(glob_table, function_name);
                            if (tmp == NULL){
                                exit(ESEM_DEF);
                            }

                            tItem * tmp2;
                            tmp2 = tableSearch(tmp->data.func->local_table, variable_name);
                            if (tmp2 == NULL){
                                exit(ESEM_DEF);
                            }

                            //tmp2->data.var->val.string_val = token.value.array;

                            //tablePrint(glob_table);
                        
                        } else {
                            exit(ESEM_TKP);
                        }
                    }     
                }

                queuePush(queue, EI);
                break;
            default:
                error_print(ESYN);
                exit(ESYN);
        }
        token = GetToken();
    }
    queuePush(queue, EDOLLAR);
    //printQueue(queue);
    

    if (count_of_bool > 1){
        error_print(ESYN);
        exit(ESYN);
    }

    stack = (tStack*)malloc(sizeof(tStack) * MAX_STACK);
    stackInit(stack);
    stackPush(stack, EDOLLAR);
    stackTop(stack, &a);


    queueFront(queue, &b);

    while (1){
        // printf("\n**************STACK***********\n");
        // printStack(stack);
        // printf("\n**************FRONTA***********\n");
        // printQueue(queue);
        // printf("\n-----------------------------------------------\n");


        if ((tmp_a == a) && (a != EDOLLAR) && (tmp_b == b)) {
            count++;
            if (count > 1) {
                error_print(ESYN);
                exit(ESYN);
            }
        } else {
            tmp_a = a;
            tmp_b = b;
            count = 0;
        }
        if ((a == EDOLLAR) && (b == EDOLLAR)){
            queueDispose(queue);
            if (((a != E) && (a != EDOLLAR)) || ((b != E) && (b != EDOLLAR))){
                error_print(ESYN);
                exit(ESYN);
            } 
            return token.type;
        } 
        // else if (a == b){
        //     stackTop(stack, &a);
        //     if (a != E){
        //         error_print(ESYN);
        //         exit(ESYN);
        //         }    
        //     }

        switch(tab[a][b]){
            case TX:
                error_print(ESYN);
                exit(ESYN);
                break;
            case TEQL:
                stackPush(stack, b);
                queuePop(queue);
                stackTop(stack, &a);
                queueFront(queue, &b);
                break;
            case TLESS:
                shift();
                break;
            case TMORE:
                reduce();
                break; 
        }
    }

    queueDispose(queue);
    return token.type;
}

void shift (){
    stackTop(stack, &a);
    if (a == E){
        stackPop(stack);
        stackPush(stack, ESHIFT);
        stackPush(stack, E);
    } else {
        stackPush(stack, ESHIFT);
    }

    queueFront(queue, &a);
    stackPush(stack, a);
    queuePop(queue);
    queueFront(queue, &b);
}


void reduce (){
    if (a == EI){
        stackPop(stack);
        stackTop(stack, &a);
        if (a == ESHIFT){
            stackPop(stack);
            stackTop(stack, &a);
            stackPush(stack, E);
        }
    } else if (a < 13){ 
        if (a == ERB){
            stackPop(stack);
            stackPop(stack);
            stackPop(stack);
            stackPop(stack);
            stackTop(stack, &a);
            stackPush(stack, E);     
        } else { 
            stackPop(stack);
            stackTop(stack, &a);
            if (a == E){
                stackPop(stack);
                stackTop(stack, &a);
                if (a < 13) {
                    stackPop(stack);
                    stackTop(stack, &a);
                    if (a == E){
                        stackPop(stack);
                        stackTop(stack, &a);
                        stackPush(stack, E);
                    } else {
                        error_print(ESYN);
                        exit(ESYN);
                    }                   
                } else {
                    error_print(ESYN);
                    exit(ESYN);
                }
            } else {
                stackPop(stack);
                stackTop(stack, &a);
                if (a == ESHIFT){
                    error_print(ESYN);
                    exit(ESYN);
                }
                stackPop(stack);
                stackPop(stack);
                stackTop(stack, &a);
                stackPush(stack, E);
            }
        }    
    } else {
        error_print(ESYN);
        exit(ESYN);
    }
}
