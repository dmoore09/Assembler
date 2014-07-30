#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordHangler.h"

//driver function, reads in all lines and converts them to binary form stored in char* arrays
//it then stores them in a 2d array and writes them to a output 32 bits per line

//determine what type of .word declaration line contains
int processWord(char* line, FILE *output){
	//if there are commas then initializ array
	if (strstr(line, ",") != NULL){
		printf("Initialize Array\n");
		int ret = createArrayInit(line, output);
		return ret;	
	}
	//if there are 2 colons init all
	else if (colonCount(line) == 2){
		printf("Array\n");
		int ret = createArrayInitAll(line, output);
		return ret;
	}
	//otherwise it is a variable
	else{
		printf("Var\n");
		createVar(line, output);
		return 0;
	}	
	
}

//create an array with elements a, b, c, d, e, ....
int createArrayInit(char* line, FILE *output){
	//before we can split the string into tokens we must copy all values after the colon into a new string
	char afterColon[100];
	//get colon position
	char *colon = strchr(line, ':');
        int colonPos = (int)(colon - line);
	int counter = 0;
	//copy string
	for (int i = colonPos; i < strlen(line); i++){
		afterColon[counter] = line[i];
		counter++;
	}

	int elementCount = 0;
	//declare a pointer to be used by strtok
	char* token;
	token = strtok(afterColon, ",abcdefghijklmnopqrstuvwxyz.:");
	//get all tokens
	while (token != NULL){
		int element = atoi(token);
		dTobPrinter(element, output);
		token = strtok(NULL, ",abcdefghijklmnopqrstuvwxyz.:");
		elementCount = elementCount + 1;
	}
	return elementCount;
}

//create an array of size n, with m characters m:n
int createArrayInitAll(char* line, FILE *output){
	//find colon position 
	char *colon = strchr(line, ':');
	colon = strchr(colon + 1, ':');
	int colonPos = (int)(colon - line);
	
	//character array holding size of new array
	int strLength = strlen(line);
	char size[strLength - colonPos + 1];
	int counter1 = 0;
	for (int i = colonPos + 1; i < strLength; i++){
		size[counter1] = line[i];
		counter1 = counter1 + 1;
	}
	int decSize = atoi(size);
	
	//character array holding value of array elements
	//find position of .
	char* period = strchr(line, '.');
	int periodPos = (int)(period - line);

	char element[colonPos - (periodPos + 5)];
	int counter2 = 0;
	for(int i = periodPos + 5; i < colonPos; i++){
		element[counter2] = line[i];
		counter2 = counter2 + 1;
	}
	int elementValue = atoi(element);

	//print out size lines with value of element
	for (int i = 0; i < decSize; i++){
		dTobPrinter(elementValue, output);
	}
	return decSize - 1;
}

//create a variable from .data section
//variable always occurs 5 positions after the .
void createVar(char* line, FILE *output){
	//find position of period 
	char* period = strchr(line, '.');
        int periodPos = (int)(period - line);

	int strLength = strlen(line);
        char var[strLength - (periodPos + 5)];
        int counter = 0;
	for (int i = periodPos + 5; i < strLength; i++)
	{
		var[counter] = line[i];
		counter = counter + 1;
	}
	//convert to a decimal
	int varInt = atoi(var);
	dTobPrinter(varInt, output);
}	

//grabs the ascii string between two quotes and prints them
int processASC(char* line, FILE *output){
	int lineCount = 0;
	//get just the string to be printed
	char* token;
        token = strtok(line, "\"");
	token = strtok(NULL, "\"");
	
	
	//print out the ascii string in big endian order
	//read fours byte at a time and print backwards
	int strLength = strlen(token);
	printf("%s %d\n", token, strLength);
	//if string is less the 4 then it can be printed by simple cases
	if (strLength >= 4){
		lineCount = -1;
		//remainder is the amount of letters left over that need to be printed in last 4 byte chunk
		int remainder = strLength % 4;
		for (int i = 0; i < strLength - remainder; i = i + 4){
			printBinaryChar(token[i + 3], output);
			printBinaryChar(token[i + 2], output);
			printBinaryChar(token[i + 1], output);
			printBinaryChar(token[i], output);
			fprintf(output, "\n");
			lineCount = lineCount + 1;
		}
		lineCount = lineCount + 1;
		//now number of characters left = remainder + 1 0 byte
		int currPos = strLength - remainder;
		//no remainder need 32 0s
		if (remainder == 0){
			for (int i = 0; i < 8; i++){
				fprintf(output, "0");
			}
			fprintf(output, "\n");

		}
		//could possibly fit more on this line?
		else if (remainder == 1){
			for (int i = 0; i < 8; i++){
                        	fprintf(output, "0");
                	}
			printBinaryChar(token[currPos], output);
                	fprintf(output, "\n");
		}
		else if (remainder == 2){
               		for (int i = 0; i < 16; i++){
                        	fprintf(output, "0");
                	}
			printBinaryChar(token[currPos + 1], output);
                        printBinaryChar(token[currPos], output);
                	fprintf(output, "\n");
        	}
		else{
                        for (int i = 0; i < 8; i++){
                	        fprintf(output, "0");
                	}
			printBinaryChar(token[currPos + 2], output);
                        printBinaryChar(token[currPos + 1], output);
                        printBinaryChar(token[currPos], output);
                	fprintf(output, "\n");
        	}
		return lineCount;
	}
	//only print out 2 bytes because it is possible to fit another word on this line if it will fit
	else if (strLength == 1){
                for (int i = 0; i < 8; i++){
                        fprintf(output, "0");
                }
		printBinaryChar(token[0], output);
                fprintf(output, "\n");
		return 0;
        }
	else if (strLength == 2){
		for (int i = 0; i < 16; i++){
                        fprintf(output, "0");
                }
		printBinaryChar(token[1], output);
                printBinaryChar(token[0], output);
                fprintf(output, "\n");
		return 0;
        }
	else if (strLength == 3){
                for (int i = 0; i < 8; i++){
                        fprintf(output, "0");
                }
		printBinaryChar(token[2], output);
                printBinaryChar(token[1], output);
                printBinaryChar(token[0], output);

                fprintf(output, "\n");
		return 0;
        }


	return lineCount;
}

//converts c to binary
void printBinaryChar(char c, FILE *output){
	for (int i = 7; i >= 0; --i) {
		int bin = ((c  >> i) & 1);
    		fprintf(output, "%d", bin);
	}
}

//count the number of colons in a line
int colonCount(char* line){
	int length = strlen(line);
	int count = 0;
	for (int i = 0; i < length; i++){
		if (line[i] == ':'){
			count = count + 1;
		}
	}
	return count;
}


void dTobPrinter(int Imm, FILE* output){
        int temp;
        for (int i = 31; i >= 0; i--){
                temp = Imm >> i;
                if (temp & 1){
                       fprintf(output, "1");
                }
                else{
                       fprintf(output, "0");
                }
        }
	fprintf(output, "\n");
}


