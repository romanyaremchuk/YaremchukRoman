/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: parser.h
 * Autori: 	Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 */

#include "symtable.h"

int SIZE = MAX_SIZE;

unsigned int hashCode(tTable * table, char * key){
	int retval = 1;
	int keylen = strlen(key);

	for (int i = 0; i < keylen; i++){
		retval += key[i];
	}
	return (retval % SIZE);




 //    unsigned int hash;
	// int i = 0;

	// while(hash < ULONG_MAX && i < strlen(key)) {
	// 	hash = hash << 8;
	// 	hash += key[i];
	// 	i++;
	// }

	// return (hash % SIZE);
}

tTable * tableInit(int size){	

	struct tTable * table;

	table = malloc (sizeof(struct tTable) + size * sizeof(struct tItem));
	if (table == NULL){
		exit(99);
	}

	for (int i = 0; i < size; i++){	
		table->ptr[i] = NULL;
	}

	table->size = size;

	return table;
}

tItem * tableSearch(tTable * table, char * key){
	unsigned int i = hashCode(table, key);

	tItem * tmp = table->ptr[i];

	while(tmp != NULL){
		if(strcmp(key, tmp->key) == 0){
			return tmp;
		} else {
			tmp = tmp->next;
		}
	}
	return NULL;

}

void tableInsert(tTable * table, char * key, union v_f data, enum tInfo info){
	if (table == NULL){
		exit(99);
	}
	tItem * tmp;
	unsigned int i = hashCode(table, key);

	tmp = tableSearch(table, key);

	if (tmp != NULL){
		if (data.func->defined == false){
			return;
		} else {
			exit(3);	
		}
	}

	tmp = malloc(sizeof(struct tItem));
	if (tmp == NULL){
		exit(99);
	}
	tmp->key = key;
	tmp->data = data;
	tmp->info = info;

	tmp->next = table->ptr[i];
	table->ptr[i] = tmp;

}

void tableEdit(tTable * table, char * key, union v_f data){
	if (table == NULL){
		exit(99);
	}

	tItem *s = tableSearch(table, key);

	if (s == NULL){
		exit(3);
	} else {
		s->data = data;
	}

}

void tableDelete(tTable * table, char * key){
	if (table == NULL){
		exit(99);
	} else {
		unsigned int i = hashCode(table, key);
		tItem * tmp1 = table->ptr[i];
		tItem * tmp2 = table->ptr[i];

		if (tmp1 != NULL){
			if (tmp1->key == key){
				table->ptr[i] = tmp1->next;
				free(tmp1);
			} else {
				while(tmp1->next != NULL){
					tmp2 = tmp1;
					tmp1 = tmp1->next;

					if (tmp1->key == key){
						tmp1 = tmp1->next;
						tmp2->next = tmp1;
						free(tmp1);
						return;
					}
				}
			}
		}
	}
}

void tableClearAll(tTable * table){
	if (table == NULL){
		exit(99);
	} else {
		tItem * tmp1;
		tItem * tmp2;
		
		for (int i = 0; i < SIZE; i++){
			tmp1 = table->ptr[i];
			
			while(tmp1 != NULL){
				tmp2 = tmp1->next;
				free(tmp1);
				tmp1 = tmp2;
			}

			table->ptr[i] = NULL;
		}
	}
}
void tablePrint2(tTable * table){
	;
	if (table == NULL){
		exit(3);
	}

	for (int l = 0; l < SIZE; l++){
		tItem * loc = table->ptr[l];

		while(loc != NULL){
			if (strcmp(loc->key, "UNDEFINED")){
				printf(" PREMENNA -> Key/name: %s,\t", loc->key);
				printf("Defined? : %d,\t", loc->data.var->defined);
				switch(loc->data.var->type){
					case t_INT:
						printf("Type: %d,\t", loc->data.var->type);
						printf("Value: %d\n", loc->data.var->val.int_val);
						break;
					case t_DOUBLE:
						printf("Type: %d,\t", loc->data.var->type);
						printf("Value: %f\n", loc->data.var->val.double_val);
						break;
					case t_STRING:
						printf("Type: %d,\t", loc->data.var->type);
						printf("Value: %s\n", loc->data.var->val.string_val);
						break;
				}
				loc = loc->next;
			} else {
				loc = loc->next;
			}
		}	
	}
}

void tablePrint(tTable * table){
	;
	int count = 0;

	printf("\n***********************************HASH_TABLE***********************************\n\n");
	printf("\t   TYPE - integer(0), double(1), string(2)\n\n");

	if (table == NULL){
		exit(3);
	}

	for(int i = 0; i < SIZE; i++){
		tItem * ptr = table->ptr[i];

		while(ptr != NULL){
			if (strcmp(ptr->key, "UNDEFINED")){
				printf("---------------------------------------------------------------------------------------------------\n");
				printf(" Key/name: %s,\t", ptr->key);
				if (ptr->info == FUNC){
					printf("Return t: %d,\t", ptr->data.func->return_type);
					printf("Defined? : %d,\t", ptr->data.func->defined);
					printf("Pocet params: %d\n", ptr->data.func->count_params);
					for (int k = 0; k < ptr->data.func->count_params; k++){
					 	printf("[%d] Meno: %s Typ: %d\n", k, ptr->data.func->params[k]->param_name, ptr->data.func->params[k]->type);
					}
					tTable * local_table = ptr->data.func->local_table;
					if (local_table == NULL){
					 	;
					 } else {
					 	tablePrint2(local_table);
					}
				}
				ptr = ptr->next;
				count++;
				printf("\n");
			} else {
				ptr = ptr->next;
			}
		}

	}

	printf("\n\t\tPocet poloziek v tabulke je: %d\n", count);
	printf("********************************************************************************\n\n");
}
