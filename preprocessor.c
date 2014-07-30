#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
//PreProcessor functions gets rid of comments and labels
//@param line from the input file
//@return 1 if a comment is found a removed
//@return 2 if no comment was found
void preProcess(char* line, char* dest, FILE* output){
	dest[0] = 0;
	int commentInd = 0;
	//check to see if line contains comments
	char* comment = strchr(line, '#');
	if (comment != NULL){
		//a comment is found, get position of beginning
		int index = (int)(comment - line);
		//delete everything after comment
		strncat(dest, line, index);
		commentInd = 1;
		
	}
	//string has no comment 
	else
	{
		strcpy(dest, line);
		
	
	}	
	
	//dont proceed if the line is empty
	if (dest[0] != '\n'){
		//remove whitespace
		char* token = strtok(dest, " \t");
		while (token != NULL){
			fprintf(output, "%s", token);
			token = strtok(NULL, " \t");
		}
		if (commentInd == 1){
			fprintf(output, "\n");
		}
	}
}

//examine line and determine if it is a label
//if it is a label return a new label
//@param instructionNumber # of instruction in section
Label* detectLabel(char* line, int instructionNumber){
	//check to see if the line is a label
	if(strrchr(line, ':') != NULL){
		//create a new label to be added to the list
		Label *newLabel = malloc(sizeof(Label));
		printf("Hex Address: %p  ", instructionNumber * 4);
		int addressInt = (instructionNumber * 4);
		newLabel->lineNumber = instructionNumber;		
		//add address info
		printf("Address: ");
		for (int i = 16; i >= 0; i--){
			int temp = addressInt >> i;
			newLabel->address[i] = (temp) & 1;
			printf("%d", newLabel->address[i]);
			
		}
		printf("   ");
		//assign name without :
		char* colon = strchr(line, ':');
		int colonPos = colon - line;
		strncpy(newLabel->name, line, colonPos);
		printf("%s\n", newLabel->name);
		return newLabel;
	}
	return NULL;	
}
