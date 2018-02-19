/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: parser.c
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

#include "parser.h"

SToken token; //navratova hodnota z lexikalnej analyzy
struct tTable * glob_table;
struct tParams **p;
struct tTable * local_table;

union v_f data;
union v_f v_data;
int count_params;

char * function_name;
char * variable_name;
char * parameter_name;
char * call_func;
int parameter_type;
int variable_type;
int parameter_count;

bool scope_flag = 0;

int if_flag = 0;

int if_count = 0;

int while_flag = 0;
int while_count = 0;

int table_size = MAX_SIZE;

void end_function(int err) {
	error_print(err);
	exit(err);
}

void functionInsert(){
	tItem * tmp;
	tmp = tableSearch(glob_table, function_name);
	if (tmp != NULL){
		if (data.func->defined == false){
			return;
		} else {
			end_function(ESEM_DEF);
		}
	}
	tFunc * f = malloc(sizeof(tFunc));
	f->func_name = function_name;
	data.func = f;
	tableInsert(glob_table, f->func_name, data, FUNC);
}

void functionParamsInitAndFirst(int type, char * param_name){
	tItem * tmp;
	tmp = tableSearch(glob_table, function_name);
	if (tmp != NULL){
		if (tmp->data.func->count_params > 0){
			if (tmp->data.func->params[0]->type == type){
				tmp->data.func->params[0]->param_name = param_name;
			} else {
				end_function(ESEM_TKP);
			}
		} else {
			struct tParams **p = malloc(sizeof(struct tParams) * 10);
			for (int i = 0; i < 10; i++){
				p[i] = malloc(sizeof(struct tParams));
			}
			p[0]->type = type;
			p[0]->param_name = param_name;

			tmp->data.func->params = p;

			tmp->data.func->count_params++;
		}	

		parameter_count++;

	} else {
		end_function(ESEM_DEF);
	}
}

void functionParamsNext(int type, char * param_name){
	tItem * tmp;
	tmp = tableSearch(glob_table, function_name);
	if (tmp != NULL){
		if ((tmp->data.func->defined == 1) && (tmp->data.func->declared == 1)){
			if (tmp->data.func->params[parameter_count]->type == type){
				tmp->data.func->params[parameter_count]->param_name = param_name;
			} else {
				end_function(ESEM_TKP);
			}
		} else {
			tmp->data.func->params[parameter_count]->type = type;
			tmp->data.func->params[parameter_count]->param_name = param_name;

			tmp->data.func->count_params++;
		}

		parameter_count++;

	} else {
		end_function(ESEM_DEF);
	}
}

void functionEdit(){
	tableEdit(glob_table, function_name, data);
}

void start() {
	eol_check();
	//tablePrint(glob_table);
	if (token.type == token_scope) {
		scope_flag = 1;
		scope();
	} else if ((token.type == token_declare) || (token.type == token_function)) {
		if (scope_flag == 0) {
			func();
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}
}

void func() {
	if (token.type == token_declare) {
		declare();
	} else if (token.type == token_function) {
		define();
	}
}

void declare() {
	token = GetToken();
	if ((token.type == token_function) && ((token = GetToken()).type == token_id)) {
		function_name = token.name;
		parameter_count = 0;

		/**********************************/
		functionInsert();
		/**********************************/

        token = GetToken();

		if (token.type == token_leftBracket){
			token = GetToken();
			func_param();
		} else {
			end_function(ESYN);
		}

		token = GetToken();
		if (token.type == token_rightBracket){
			token = GetToken();
		}

		if (token.type == token_as) {
			token = GetToken();
			if ((token.type == token_integer) || (token.type == token_string) || (token.type == token_double)){
				/******************************************************/

				tItem * tmp = tableSearch(glob_table, function_name);
		    	if (tmp == NULL){
		    		end_function(ESEM_DEF);
		    	}

		    	tmp->data.func->declared = true;
				tmp->data.func->defined = false;

	            if (token.type == token_integer){
	            	tmp->data.func->return_type = t_INT;
	            } else if (token.type == token_double){
	            	tmp->data.func->return_type = t_DOUBLE;
	            } else if (token.type == token_string){
	            	tmp->data.func->return_type = t_STRING;
	            } 

	            //functionEdit();
	            
	            /******************************************************/
	            //tablePrint(glob_table);
	            parameter_count = 0;
				token = GetToken();
				if (token.type == token_eol) {
					token = GetToken();
					start();
				} else {
					end_function(ESYN);
				}					
			} else {
				end_function(ESYN);
			}
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}
}

void define() {
	if ((token.type == token_function) && ((token = GetToken()).type == token_id)) {
		function_name = token.name;
		parameter_count = 0;
		/***********************************************************/
		functionInsert();

        tItem * tmp = tableSearch(glob_table, function_name);
    	if (tmp == NULL){
    		end_function(ESEM_DEF);
    	}

    	//struct tTable * local_table;
	 	local_table = tableInit(MAX_SIZE);
	 	tmp->data.func->defined = true;
		tmp->data.func->local_table = local_table;
		//functionEdit();
    	/***********************************************************/

        token = GetToken();

		if (token.type == token_leftBracket){
			token = GetToken();
			func_param();

			if (tmp->data.func->count_params != parameter_count){
				end_function(ESEM_TKP);
			}

		} else {
			end_function(ESYN);
		}

		token = GetToken();
		if (token.type == token_as) {
			token = GetToken();
			if ((token.type == token_integer) || (token.type == token_string) || (token.type == token_double)){
				/******************************************************/
				if ((tmp->data.func->defined == 1) && (tmp->data.func->declared == 1)){
					if (((tmp->data.func->return_type == t_INT ) && (token.type != token_integer)) || 
						((tmp->data.func->return_type == t_DOUBLE ) && (token.type != token_double)) || 
						((tmp->data.func->return_type == t_STRING ) && (token.type != token_string))) {
						end_function(ESEM_TKP);
					}
				}
				if (token.type == token_integer){
	            	tmp->data.func->return_type = t_INT;
	            } else if (token.type == token_double){
	            	tmp->data.func->return_type = t_DOUBLE;
	            } else if (token.type == token_string){
	            	tmp->data.func->return_type = t_STRING;
	            } 

	            //functionEdit();
				/******************************************************/

				token = GetToken();
				if (token.type == token_eol) {
					token = GetToken();
					eol_check();
					func_dim();
				} else {
					end_function(ESYN);
				}
			} else {
				end_function(ESYN);
			}
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}

}

void func_param() {
	if (token.type == token_rightBracket) {
		return;
	} else if (token.type == token_id) {
		/***********************************************************/
		parameter_name = token.name;

        token = GetToken();
        /***********************************************************/
		if (token.type == token_as) {
			token = GetToken();
			if ((token.type == token_integer) || (token.type == token_string) || (token.type == token_double)){
				/******************************************************/
				if (token.type == token_integer) {
					parameter_type = 0;
				} else if (token.type == token_double) {
					parameter_type = 1;
				} else if (token.type == token_string) {
					parameter_type = 2;
				}
				
				functionParamsInitAndFirst(parameter_type, parameter_name);

	            /******************************************************/
				token = GetToken();
				func_params();
				return;
			} else {
				end_function(ESYN);
			}
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}
}

void func_params() {
	if (token.type == token_rightBracket) {
		return;
	} else if (token.type == token_colom){
		token = GetToken();

		if (token.type == token_id){
		/***********************************************************/
	    	parameter_name = token.name;

	        token = GetToken();
	        /***********************************************************/
			if (token.type == token_as) {
				token = GetToken();
				if ((token.type == token_integer) || (token.type == token_string) || (token.type == token_double)) {
					/******************************************************/
					if (token.type == token_integer) {
						parameter_type = 0;
					} else if (token.type == token_double) {
						parameter_type = 1;
					} else if (token.type == token_string) {
						parameter_type = 2;
					}

					tItem * tmp = tableSearch(glob_table, function_name);
			    	if (tmp == NULL){
			    		end_function(ESEM_DEF);
			    	}
					
					functionParamsNext(parameter_type, parameter_name);

		            /******************************************************/
					token = GetToken();
					func_params();
					return;
				} else {
					end_function(ESYN);
				}
			} else {
				end_function(ESYN);
			}
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}
}

void func_dim() {
    switch(token.type){
        case token_dim:
            token = GetToken();
            if (token.type == token_id){
            	/********************************************************/
            	variable_name = token.name;
            	/***************KONTROLA CI JE ROVNAKE MENO FUNKCIE A PREMENNEJ*********************/
            	tItem * fun = tableSearch(glob_table, variable_name);
			    if (fun != NULL){
			    	end_function(ESEM_DEF);
			   	}

            	tItem * tmp = tableSearch(glob_table, function_name);
			    if (tmp == NULL){
			    	end_function(ESEM_DEF);
			   	}
            	for (int t = 0; t < tmp->data.func->count_params; t++){
            		if ((strcmp(tmp->data.func->params[t]->param_name, token.name)) == 0){
            			end_function(ESEM_DEF);
            		}
            	}

				tVar * v = malloc(sizeof(tVar));
				v->var_name = token.name;
				v->defined = true;
				v_data.var = v;

				tableInsert(local_table, token.name, v_data, VAR);
				/********************************************************/

            	token = GetToken();

            	if (token.type == token_as) {
	            	/***********************************************************/
	                token = GetToken();

	                if (token.type == token_integer){
	                	v->type = t_INT;
	                	v->val.int_val = 0;
	                } else if (token.type == token_double){
	                	v->type = t_DOUBLE;
	                	v->val.double_val = 0.0;
	                } else if (token.type == token_string){
	                	v->type = t_STRING;
	                	v->val.string_val = "";
	                } else {
	                	end_function(ESYN);
	                }
	                v_data.var = v;
	                tableEdit(local_table, v->var_name, v_data);
	                /***********************************************************/
	            
	                dim_n();
	                token = GetToken();
	                func_dim();
	            } else {
	            	end_function(ESYN);
	            }
            } else {
                end_function(ESYN);
            }
        default:
            func_body();
            func_dim();
    }
}

void f_cond_body() {
	func_body();
	if (token.type == token_else || token.type == token_end_if || token.type == token_loop) {
		return;
	}
	f_cond_body();
}

void func_body() {
	switch(token.type){
		case token_eol:
			token = GetToken();
			break;
		case token_id:
			;
			variable_name = token.name;
			
			if ((token = GetToken()).type == token_assignment) {
				/***********************************************************/
				tItem * tmp = tableSearch(glob_table, function_name);
	        	if (tmp == NULL){
	        		end_function(ESEM_DEF);
	        	}

	        	tItem * tmp2 = tableSearch(tmp->data.func->local_table, variable_name);
	        	if (tmp2 == NULL){
	                for (int k = 0; k < tmp->data.func->count_params; k++){
	                    if ( strcmp(tmp->data.func->params[k]->param_name, variable_name) == 0 ){
	                        variable_type = tmp->data.func->params[k]->type;
	                    } else {
	                        exit(ESEM_DEF);
	                    }
	                } 
	        	} else {
	        		variable_type = tmp2->data.var->type;
	        	}
	        	/***********************************************************/
				token = GetToken();
				expr();
				token = GetToken();
				break;
			} else {
				end_function(ESYN);
			}
		case token_if:
			if_flag = 1;
			if_count++;
			token = GetToken();
			func_condition();
			break;
		case token_do:
			//while_flag = 1;
			while_count++;
			if ((token = GetToken()).type == token_while) {
				token = GetToken();
				loop();
				break;
			} else {
				end_function(ESYN);
			}
		case token_print:
			variable_name = "print";
			token = GetToken();
			printable();
			token = GetToken();
			break;
		case token_input:
			token = GetToken();
			if (token.type == token_id) {
				/*************************************************************/
				tItem * tmp = tableSearch(glob_table, function_name);
	        	if (tmp == NULL){
	        		end_function(ESEM_DEF);
	        	}

	        	tItem * tmp2 = tableSearch(tmp->data.func->local_table, token.name);
	        	if (tmp2 == NULL){
	        		//osetrenie ze to moze byt parameter
	        		end_function(ESEM_DEF);
	        	}
                /*************************************************************/
				token = GetToken();
				if (token.type == token_eol) {
					token = GetToken();
					break;
				} else {
					end_function(ESYN);
				}
			} else {
				end_function(ESYN);
			}
		case token_else:
			if (if_flag == 1){
				break;
			} else {
				end_function(ESYN);
			}
		case token_end_if:
			if (if_flag == 1){
				break;
			} else {
				end_function(ESYN);
			}
		case token_loop:
			// if (while_flag == 1){
			// 	token = GetToken();
			// } else {
			// 	end_function(ESYN);
			// }
			break;
		case token_return:
			variable_name = "return";
			//tablePrint(glob_table);
			token = GetToken();
			if (token.type == token_eol) {
				end_function(ESYN);
			}
			expr();
			if (token.type == token_eol) {
				token = GetToken();
				eol_check();
				if ((token.type == token_end_function) && ((token = GetToken()).type == token_eol) && (if_count == 0) && (while_count == 0)) {
					token = GetToken();
					start();
					break;
				} else if(token.type == token_else) {
					break;
				} else if(token.type == token_end_if) {
					break;
				} else {
					end_function(ESYN);
				}
			} else {
				end_function(ESYN);
			}
		default:
			if ((token.type == token_end_function) && ((token = GetToken()).type == token_eol) && (if_count == 0) && (while_count == 0)) {
				token = GetToken();
				start();
				break;
			} else {
				end_function(ESYN);
			}
	}
	return;
}

void scope() {
	token = GetToken();
	function_name = "scope";
	/**********************************/
	functionInsert();
	data.func->defined = true;

	tItem * tmp;
	tmp = tableSearch(glob_table, function_name);
	if (tmp == NULL){
		end_function(ESEM_DEF);
	}

	local_table = tableInit(MAX_SIZE);
	tmp->data.func->local_table = local_table;

	functionEdit();
	/**********************************/
	if (token.type == token_eol) {
		eol_check();
	} else {
		end_function(ESYN);
	}

	if ((token.type == token_end_scope) && (if_count == 0) && (while_count == 0)) {
		end_scope_check();
	} else {
		scope_body();
	}
}

void scope_body() {

    switch(token.type){
        case token_dim:
            token = GetToken();
            if (token.type == token_id){
				/********************************************************/
				variable_name = token.name;

				/***************KONTROLA CI JE ROVNAKE MENO FUNKCIE A PREMENNEJ*********************/
            	tItem * fun = tableSearch(glob_table, variable_name);
			    if (fun != NULL){
			    	end_function(ESEM_DEF);
			   	}

				tVar * v = malloc(sizeof(tVar));
				v->var_name = token.name;
				v->defined = true;
				v_data.var = v;

				tableInsert(local_table, token.name, v_data, VAR);
				/********************************************************/

            	token = GetToken();

            	if (token.type == token_as) {
	            	/***********************************************************/
	                token = GetToken();

	                if (token.type == token_integer){
	                	v_data.var->type = t_INT;
	                	v_data.var->val.int_val = 0;
	                } else if (token.type == token_double){
	                	v_data.var->type = t_DOUBLE;
	                	v_data.var->val.double_val = 0.0;
	                } else if (token.type == token_string){
	                	v_data.var->type = t_STRING;
	                	v_data.var->val.string_val = "";
	                } else {
	                	end_function(ESYN);
	                }

	                v_data.var = v;
	                tableEdit(local_table, v->var_name, v_data);
	                /***********************************************************/
	            
	                dim_n();
	                token = GetToken();
	                scope_body();
	            } else {
	            	end_function(ESYN);
	            }
            } else {
                end_function(ESYN);
            }
        default:
            body();
            scope_body();
    }
}

void cond_body() {
	body();
	if (token.type == token_else || token.type == token_end_if || token.type == token_loop) {
		return;
	}
	cond_body();
}

void body() {
	switch(token.type){
		case token_eol:
			token = GetToken();
			break;
		case token_id:
			;

			variable_name = token.name;
			
			if ((token = GetToken()).type == token_assignment) {
				/***********************************************************/
				tItem * tmp = tableSearch(glob_table, function_name);
	        	if (tmp == NULL){
	        		end_function(ESEM_DEF);
	        	}

	        	tItem * tmp2 = tableSearch(tmp->data.func->local_table, variable_name);
	        	if (tmp2 == NULL){
	                for (int k = 0; k < tmp->data.func->count_params; k++){
	                    if ( strcmp(tmp->data.func->params[k]->param_name, variable_name) == 0 ){
	                        variable_type = tmp->data.func->params[k]->type;
	                    } else {
	                        exit(ESEM_DEF);
	                    }
	                } 
	        	} else {
	        		variable_type = tmp2->data.var->type;
	        	}

	        	token = GetToken();
	        
	        	/***********************************************************/
				expr();
				token = GetToken();
				break;
			} else {
				end_function(ESYN);
			}
		case token_if:
			if_flag = 2;
			if_count++;
			token = GetToken();
			condition();
			break;
		case token_do:
			//while_flag = 2;
			while_count++;
			if ((token = GetToken()).type == token_while) {
				token = GetToken();
				loop();
				break;
			} else {
				end_function(ESYN);
			}
		case token_print:
			variable_name = "print";
			token = GetToken();
			printable();
			token = GetToken();
			break;
		case token_input:
			if ((token = GetToken()).type == token_id) {
				/*************************************************************/
				tItem * tmp = tableSearch(glob_table, function_name);
	        	if (tmp == NULL){
	        		end_function(ESEM_DEF);
	        	}

	        	tItem * tmp2 = tableSearch(tmp->data.func->local_table, token.name);
	        	if (tmp2 == NULL){
	        		//osetrenie ze to moze byt parameter
	        		end_function(ESEM_DEF);
	        	}
                /*************************************************************/
				token = GetToken();
				if (token.type == token_eol) {
					token = GetToken();
					break;
				} else {
					end_function(ESYN);
				}
			} else {
				end_function(ESYN);
			}
		case token_else:
			if (if_flag == 2){
				break;
			} else {
				end_function(ESYN);
			}
		case token_end_if:
			if (if_flag == 2){
				break;
			} else {
				end_function(ESYN);
			}
		case token_loop:
			break;
		default:
			if ((token.type == token_end_scope) && (if_count == 0) && (while_count == 0)) {
				end_scope_check();
			} else {
				end_function(ESYN);
			}
	}
}

void dim_n(){
	if ((token.type == token_integer) || (token.type == token_string) || (token.type == token_double)){
		token = GetToken();
		if (token.type == token_assignment) {
			token = GetToken();
			if (token.type == token_eol) {
				end_function(ESYN);
			}
			expr();	
			if (token.type == token_eol) {
				return;
			} else {
				end_function(ESYN);
			}			
		} else if (token.type == token_eol) {
			return;
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}
}

void printable() {
	expr();

	if (token.type == token_semicolom) {
		token = GetToken();
		printable_n();
		return;
	} else {
		end_function(ESYN);
	}
}

void printable_n() {
	if (token.type == token_eol) {
		return;
	} else {
		token = GetToken();
		printable();
		return;
	}
}

void expr() {

    token.type = precedence(0, token, function_name, glob_table, data, variable_type, variable_name);

    if (token.type == token_id) {
    	call_func = token.name;
    	token = GetToken();
    	if (token.type == token_leftBracket){
    		token = GetToken();
    		int count_for_param = 0;

    		while (token.type != token_rightBracket){
    			if (token.type == token_string) {
    				tItem * tmp = tableSearch(glob_table, call_func);
		        	if (tmp == NULL){
		        		end_function(ESEM_DEF);
		        	}
		        	if (tmp->data.func->count_params == count_params){
		        		end_function(ESEM_TKP);
		        	}

		        	int vt = tmp->data.func->params[count_for_param]->type;
		        	int rt = 2;
					
					if ((rt == 2) && (vt == 0)){ //STRING a INT
                        exit(ESEM_TKP);
                    } else if ((rt == 2) && (vt == 1)){ //STRING a DOUBLE
                        exit(ESEM_TKP);
                    } 

                    count_for_param++;

	    			token = GetToken();
		        	if (token.type == token_colom){
		        		token = GetToken();
		        		continue;
		        	} else if (token.type == token_rightBracket){
		        		break;
		        	} else {
		        		end_function(ESYN);
		        	}
		        	
	    		} else if (token.type == token_val){

	    		/***********************PARAMETREEEEE**************************************/
	    			tItem * tmp = tableSearch(glob_table, call_func);
		        	if (tmp == NULL){
		        		end_function(ESEM_DEF);
		        	}
		        	if (tmp->data.func->count_params == count_params){
		        		end_function(ESEM_TKP);
		        	}

		        	int rt = token.var_type;
		        	int vt = tmp->data.func->params[count_for_param]->type;

                    if ((rt == 0) && (vt == 2)){ //INT a STRING
                        exit(ESEM_TKP);
                    } else if ((rt == 1) && (vt == 2)){ //DOUBLE a STRING
                        exit(ESEM_TKP);
                    } else if ((rt == 2) && (vt == 0)){ //STRING a INT
                        exit(ESEM_TKP);
                    } else if ((rt == 2) && (vt == 1)){ //STRING a DOUBLE
                        exit(ESEM_TKP);
                    } 

	    			count_for_param++;

	    			token = GetToken();
		        	if (token.type == token_colom){
		        		token = GetToken();
		        		continue;
		        	} else if (token.type == token_rightBracket){
		        		break;
		        	} else {
		        		end_function(ESYN);
		        	}

		        /*************************************************************/

	    		} else if (token.type == token_id) {
	    				
	    		/***********************PARAMETREEEEE**************************************/
					tItem * tmp = tableSearch(glob_table, call_func);
		        	if (tmp == NULL){
		        		end_function(ESEM_DEF);
		        	}
		        	if (tmp->data.func->count_params == count_params){
		        		end_function(ESEM_TKP);
		        	}

		        	tItem * ptr = tableSearch(glob_table, function_name);
		        	if (ptr == NULL){
		        		end_function(ESEM_DEF);
		        	}

		        	tItem * ptr2 = tableSearch(ptr->data.func->local_table, token.name);
		        	if (ptr2 == NULL){
		        		for (int k = 0; k < ptr->data.func->count_params; k++){
		                    if ( strcmp(ptr->data.func->params[k]->param_name, token.name) == 0 ){

		                        int rt = ptr->data.func->params[k]->type;
				        		int vt = tmp->data.func->params[count_for_param]->type;

		                        if ((rt == 0) && (vt == 2)){ //INT a STRING
		                            exit(ESEM_TKP);
		                        } else if ((rt == 1) && (vt == 2)){ //DOUBLE a STRING
		                            exit(ESEM_TKP);
		                        } else if ((rt == 2) && (vt == 0)){ //STRING a INT
		                            exit(ESEM_TKP);
		                        } else if ((rt == 2) && (vt == 1)){ //STRING a DOUBLE
		                            exit(ESEM_TKP);
		                        } 
					        } 
					    } 

		        	} else {
		        		int rt = ptr2->data.var->type;
		        		int vt = tmp->data.func->params[count_for_param]->type;

                        if ((rt == 0) && (vt == 2)){ //INT a STRING
                            exit(ESEM_TKP);
                        } else if ((rt == 1) && (vt == 2)){ //DOUBLE a STRING
                            exit(ESEM_TKP);
                        } else if ((rt == 2) && (vt == 0)){ //STRING a INT
                            exit(ESEM_TKP);
                        } else if ((rt == 2) && (vt == 1)){ //STRING a DOUBLE
                            exit(ESEM_TKP);
                        } 
		        	}
		        	count_for_param++;

		        	token = GetToken();
		        	if (token.type == token_colom){
		        		token = GetToken();
		        		continue;
		        	} else if (token.type == token_rightBracket){
		        		break;
		        	} else {
		        		end_function(ESYN);
		        	}

		        /*************************************************************/

		        } else {
		        	end_function(ESYN);
		        }
		        //token = GetToken();
		    };
		    
	    	if (token.type == token_rightBracket) {
    			token = GetToken();
    			if (token.type == token_eol){
    				return;
    			} else {
    				end_function(ESYN);
    			}
    		} else {
    			end_function(ESYN);
    		}
    		// token = GetToken();
    		// if (token.type == token_colom){
    		// 	continue;
    		// } else if (token.type == token_rightBracket){
    		// 	token = GetToken();
    		// 	if (token.type == token_eol){
    		// 		break;
    		// 	} else {
    		// 		end_function(ESYN);
    		// 	}	
    		// } else {
    		// 	end_function(ESYN);
    		// }
    		
    	} else {
    		end_function(ESYN);
    	}
    } else if ((token.type == token_eol) || (token.type == token_then) || (token.type == token_semicolom)) {
    	return;
    } else {
    	end_function(ESYN);
    }

    return;
}

void b_expr() {

    token.type = precedence(1, token, function_name, glob_table, data, variable_type, variable_name);

    if (token.type == token_id){
    	end_function(ESYN);
    }

    return;
}

void condition() {
	if (token.type == token_then) {
		end_function(ESYN);
	}
	b_expr();
	if ((token.type == token_then) && ((token = GetToken()).type == token_eol)) {
		token = GetToken();
		eol_check();
		if (if_flag == 2){
			cond_body();
		} else {
			end_function(ESYN);
		}
		
		if ((token.type == token_else) && ((token = GetToken()).type == token_eol)) {
			token = GetToken();
			eol_check();
			cond_body();
			if ((token.type == token_end_if) && ((token = GetToken()).type == token_eol)) {
				if_count--;
				token = GetToken();
				return;
			} else {
				end_function(ESYN);
			}
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}
}

void func_condition() {
	if (token.type == token_then) {
		end_function(ESYN);
	}
	b_expr();
	if ((token.type == token_then) && ((token = GetToken()).type == token_eol)) {
		token = GetToken();
		eol_check();
		if (if_flag == 1){
			f_cond_body();
		} else {
			end_function(ESYN);
		}
		
		if ((token.type == token_else) && ((token = GetToken()).type == token_eol)) {
			token = GetToken();
			eol_check();
			f_cond_body();
			if ((token.type == token_end_if) && ((token = GetToken()).type == token_eol)) {
				if_count--;
				token = GetToken();
				return;
			} else {
				end_function(ESYN);
			}
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}
}

void loop() {
	b_expr();
	if (token.type == token_eol) {
		token = GetToken();
		eol_check();
		
		cond_body();

		if ((token.type == token_loop) && ((token = GetToken()).type == token_eol)) {
			while_count--;
			token = GetToken();
			return;
		} else {
			end_function(ESYN);
		}
	} else {
		end_function(ESYN);
	}
}

void eol_check(){
	while (token.type == token_eol) {
		if (token.type == token_eof) {
			end_function(ESYN);
		}
		token = GetToken();
	}
	return;
}

void end_scope_check(){
	while(1){
		//tablePrint(glob_table);
		token = GetToken();
		if (token.type == token_eol){
			;
		} else if (token.type == token_eof){

			for(int i = 0; i < table_size; i++){
				tItem * ptr = glob_table->ptr[i];

				while(ptr != NULL){
					if (strcmp(ptr->key, "UNDEFINED")){
						if (ptr->info == FUNC && ptr->data.func->defined == false){
							end_function(ESEM_DEF);
						}
						ptr = ptr->next;
					} else {
						ptr = ptr->next;
					}
				}
			}
			end_function(EOK);
		} else {
			end_function(ESYN);
		}
	}
} 

void inBuildFunctions(){
	function_name = "length";
	functionInsert();

	data.func->return_type = t_INT;
	data.func->declared = 0;
	data.func->defined = 1;
	data.func->count_params = 0;

	functionParamsInitAndFirst(2, "s");
	functionEdit();

	parameter_count = 0;

	/**********************************/

	function_name = "substr";
	functionInsert();

	data.func->return_type = t_STRING;
	data.func->declared = 0;
	data.func->defined = 1;
	data.func->count_params = 0;

	functionParamsInitAndFirst(2, "s");
	functionParamsNext(0, "i");
	functionParamsNext(0, "n");
	functionEdit();

	parameter_count = 0;
	
	/**********************************/

	function_name = "asc";
	functionInsert();

	data.func->return_type = t_INT;
	data.func->declared = 0;
	data.func->defined = 1;
	data.func->count_params = 0;

	functionParamsInitAndFirst(2, "s");
	functionParamsNext(0, "i");
	functionEdit();

	parameter_count = 0;

	/**********************************/

	function_name = "chr";
	functionInsert();

	data.func->return_type = t_STRING;
	data.func->declared = 0;
	data.func->defined = 1;
	data.func->count_params = 0;

	functionParamsInitAndFirst(0, "i");
	functionEdit();	

	parameter_count = 0;
}


int Parse(){ //este argument na odkaz do symtable
	glob_table = tableInit(MAX_SIZE);

	inBuildFunctions();

	token = GetToken();
	start();

	return 0;
}











