//structure and functions for keeping track of labels and their addresses 
#ifndef LABELHANDLER_H
#define LABELHANDLER_H

//strucure that represents a label
typedef struct label{
	//name of label, used as search key
	char* name[32];
	char* hexAddress[10];
	//binary representation
	int* address[16];
	//so it is possible to tell is it is ahead or behind a instruction
	int lineNumber;
} Label;

//structure that keeps track of labels
typedef struct labelTable{
	Label table[20];
	int size;
} LabelTable;

void InsertLabel(LabelTable *table, Label *label);

int findLabel(char* line, LabelTable *table);
#endif

