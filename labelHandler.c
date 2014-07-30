#include "labelHandler.h"
#include <string.h>
#include<stdio.h>

//insert a label into table
//@param tableof labels
//@param name of label
//@param address of label
void InsertLabel(LabelTable *table, Label* label){
	int size = table->size;
	if (size != 20){
		table->table[size] = *label;
		table->size++;
	}		
}

//search for a label, based on name
//if found return position, if nothing is found return -1
int findLabel(char* line, LabelTable* table){
	int lineLen = strlen(line);
	for (int i = 0; i < table->size;  i++){
		//check to see if line contains name
		printf("Search %sn", line);
		printf("   Name: %sn\n", table->table[i].name); 
		if (strncmp(line, table->table[i].name, lineLen - 1) == 0){
			return i;
		}
	}
	return -1;
}

