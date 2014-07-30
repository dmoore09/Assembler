//Main class for the assembler. Contains the main method
//#include "staticdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "labelHandler.h"
#include "instructionprinter.h"
#include "preprocessor.h"
#include <string.h>
#include "wordHangler.h"
//forward declaration of functions
void decodeInstruction(char* line);
void createSimpleLtype(char* line, int* opCode, bool sw);
int decodeRegister(char* line, int* reg, int prevReg, int prevReg1);
void copyReg(int* source, int* dest);
void createSimpleRtype(char* line, int* funct, int atIndicator);
void createShamtRtype(char* line, int* funct, int srlIndicator);
void dTobConverter(int Imm, int* binary, int length);
int findLtypeImmediate(char* line);
int findShamtSa(char* line, int length, int pos, int zeros);
void createImmRtype(char* line, int* opcode, int startPos, int liIndicator);
void createBranch(char* line, int* opCode, int numberOfRegisters, int atIndicator);
void createJtype(char* line, int* opCode, int size);
void createJr(char* line, int* opCode);
void printTable();
void createLa(char* line, int* opCode);
char* getRegister(char* line, int regNum);
char* getLRegister(char* line);
void createBlteSlt(char* line, int* opCode, int bleIndicator);
void createBlteBne(char* line, int* opCode);
char* getJRegister(char* line, int size);
//all the different opcodes needed stored as int arrays
//NEED BLTZ couldnt found it
//NEED special cases for BLT, BLE
//BLT, slt + bne
//BLE, slt + beq
//structure that holds all opCode/func bits of instructions
static RegistersT registers = {{0, 0, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 1}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 1},
{0, 0, 1, 1, 0}, {0, 0, 1, 1, 1}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 1}, {0, 1, 0, 1, 0},
{0, 1, 0, 1, 1}, {0, 1, 1, 0, 0}, {0, 1, 1, 0, 1}, {0, 1, 1, 1, 0}, {0, 1, 1, 1, 1},
{1, 0, 0, 0, 0}, {1, 0, 0, 0, 1}, {1, 0, 0, 1, 0}, {1, 0, 0, 1, 1}, {1, 0, 1, 0, 0},
{1, 0, 1, 0, 1}, {1, 0, 1, 1, 0}, {1, 0, 1, 1, 1}, {1, 1, 0, 0, 0}, {1, 1, 0, 0, 1},
{1, 1, 1, 0, 0}, {1, 1, 1, 0, 1}, {1, 1, 1, 1, 0}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 1}};


static opCode opCodes = {
 {1, 0, 0, 0, 1, 1}, {1, 0, 1, 0, 1, 1}, {0, 0, 1, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 1, 0}, 
{0, 0, 1, 0, 0, 0}, {0, 0, 1, 0, 0, 1}, {1, 0, 0, 1, 0, 1}, {1, 0, 0, 1, 0, 0}, {0, 0, 1, 1, 0, 1}, {0, 0, 1, 1, 0, 0},
{1, 0, 1, 0, 1, 0}, {0, 0, 1, 0, 1, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 0, 0}, {0, 0, 0, 1, 0, 1}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 1}};
//file pointers
static FILE *input;
static FILE *output;
static FILE *preprocessed;

//table for label information
static LabelTable labels;

//number of instructions processed
static int currInstruction;

int main( int argc, const char* argv[] )
{
	currInstruction = 0;
	//process input commands
	const char* outputPath;
	const char* inputPath;
	bool list = false;
	if (strcmp("-symbols", argv[1]) == 0)
	{
		list = true;
		inputPath = argv[2];
		outputPath = argv[3];
	}
	else{
		inputPath = argv[1];
		outputPath = argv[2];
	}
	

	//open input stream
	input = fopen(inputPath, "r");
	output = fopen(outputPath, "w");
	
	if (input == NULL || output == NULL){
		printf("Either the input or output file does not exist. Program will now stop");
		return 0;
	}	
	
	//create a new temporary file to be written to
	preprocessed = fopen("preTemp.txt", "ab+");
	
	char pre[100];
	char pre1[100];
	char pre2[100];
	pre[0] = 0;
	pre1[0] = 0;
	pre2[0] = 0;
	//first preprocessor pass remove comments and whitespace
	while(fgets(pre, 100, input) != NULL){
		if (strstr(pre, ".asciiz") == NULL){
		
			preProcess(pre, pre1, preprocessed);
                }
		else if (strstr(pre, ".asciiz") != NULL){
			fprintf(preprocessed, "%s", pre);
		}
	}
        
	//at this point we loop to the .data section process is completely writing the final output
	//to a temporary file once the .text section is completed we will write the contents of this file
	//to the final output and remove it
	rewind(preprocessed);
	char data[100];
        data[0] = 0;
	
	//create a temp file to hold the final representation of .data
	FILE *preData = fopen("preData.txt", "ab+");

	//set file pointer to .data section
	fgets(data, 100, preprocessed);
        while (strstr(data, ".data") == NULL){
		fgets(data, 100, preprocessed);
	}
	
	//in order to keep track of addresses that need to be assigned to labels and array with the same number
        //of elements as the labels is created with each index storing the address as is they all took up one line
        //as we make our initial pass through the .data section writing to the first temp file the addresses are 
        //updated if a label value goes over more then 1 line
        int addressHolder[100];
        for (int i = 0; i < 100; i++){
                addressHolder[i] = 2048 + i;
        }
	
	int labelNumber = 0;
        //loop through until end of the file
        while(fgets(data, 100, preprocessed) != NULL){
                   int lines = 0;
		   //has a .word declaration
                   if (strstr(data, ".word")){
                           lines = processWord(data, preData);
			   
                    }
                    //has a .asciiz declaration
                    else if (strstr(data, ".asciiz")){
                            lines = processASC(data, preData);
                    }
			
		labelNumber = labelNumber + 1;
		//lines will be the number of 32 bit segments it takes to store the value past 1 line, 0 if only 1 line
		//all other address values must be pushed back by that ammount
		for (int i = labelNumber; i < 100; i++){
			addressHolder[i] = addressHolder[i] + lines;
		}
          }

	rewind(preData);
	//all declarations in data section are printed out 1 per line
	//now rewrite them 32bits per line to preData1.txt
	FILE *preData1 = fopen("preData1.txt", "ab+");
	char data1[100];
	char data2[100];
	data2[0] = 0;
	data1[0] = 0;
	

	while(fgets(data1, 100, preData) != NULL){
		//check length of the line
		int strLength = strlen(data1);
		//if the line has a length of 16 the next line can be added on if it
		//is length 16 as well
		if (strLength == 17 || strLength == 9){	
			//need to check next line and see if it is also length 16
			char* next = fgets(data2, 100, preData);
			if (next != NULL){
				int nextLength = strlen(data2);
				//need to update addresses
				if (nextLength == 17 && strLength == 17){
					 data2[16] = '\0';
					 fprintf(preData1, "%s", data2);
					 fprintf(preData1, "%s", data1);
				}
				else if (nextLength == 25 && strLength == 9){
					data2[24] = '\0';
					fprintf(preData1, "%s", data2);
                                        fprintf(preData1, "%s", data1);
				}	
				else
				{
					if (strLength == 17){	
						//next line wont fit, pad with zeros
						for (int i = 0; i < 16; i++){
							fprintf(preData1, "0");
						} 
						fprintf(preData1, "%s", data1);
						fprintf(preData1, "%s", data2);	
					}
					else{
						 for (int i = 0; i < 24; i++){
                                                        fprintf(preData1, "0");
                                                }
                                                fprintf(preData1, "%s", data1);
                                                fprintf(preData1, "%s", data2);
					}
				}
			}
			else{
				//end of file write out the line
				for (int i = 0; i < 16; i++){
                                	fprintf(preData1, "0");
                        	} 
                        		fprintf(preData1, "%s", data1);
			} 
		}
		//write to next temp file
		else{
			fprintf(preData1, "%s", data1);
		}
	}	
	rewind(preData1);
	remove("preData.txt");	

	labels.size = 0;	
	//first preprocessor pass gather label data to be used in next pass
	char labelSearch[100];
	labelSearch[0] = 0;
	int textCounter = 0;
	rewind(preprocessed);
	//make sure file pointer starts at .text
	fgets(labelSearch, 100, preprocessed);
	while(strstr(labelSearch, ".text") == NULL){
		fgets(labelSearch, 100, preprocessed);
	}
	//get labels from .text section	
	fgets(labelSearch, 100, preprocessed);
	while (strstr(labelSearch, ".data") == NULL){
		Label* newLabel = detectLabel(labelSearch, textCounter);
		//if a label is found add it to the label table
		if (newLabel != NULL){
			newLabel->lineNumber = textCounter;
			InsertLabel(&labels, newLabel);
		}
		else{
			textCounter = textCounter + 1;
		}
		fgets(labelSearch, 100, preprocessed);		
	}

	//find labels in the .data section addresses set by addressHolder array
	int counter6 = 0;
	while (fgets(labelSearch, 100, preprocessed) != NULL){
		Label* newLabel = detectLabel(labelSearch, addressHolder[counter6]);
		if (newLabel != NULL){
			newLabel->lineNumber = addressHolder[counter6];
			InsertLabel(&labels, newLabel);
		}
		counter6 = counter6 + 1;
	
	}


	//reset file pointer
	rewind(preprocessed);

	//read in first line of the input file, it is assumed a line will not have more then 100 characters
        char nextLn[100];
	nextLn[0] = 0;
	if (list == false){
		fgets(nextLn, 100, preprocessed);
		//loop through file reading one line at a time until .data section is reached
		while (strstr(nextLn, ".data") == NULL){

			//do not process the line if it is empty
			if (nextLn[0] != '\n' && strstr(nextLn, ":") == NULL)
			{
				//read in line and decode the instruction at the end next ln will hold the instruction
				decodeInstruction(nextLn);
			}
			fgets(nextLn, 100, preprocessed);
		}

		//all instructions have been processed .data section left
		fprintf(output, "\n");
              	char data[100]; 
		data[0] = 0;
		//loop read through preData1 temp file and print to output
		while(fgets(data, 100, preData1) != NULL){	
			fprintf(output, "%s", data);
		}
		//code is translated!
		
	}
	//print out list table for output
	else{
		printTable();
	}

	//finished close input and output files
	remove("preData1.txt");
	remove("preTemp.txt");
	fclose(input);
	fclose(output);
	
	return 0;		
}

//print out a table of all labels
void printTable(){
	printf("in function");
	fprintf(output, "Address      Symbol\n");
	fprintf(output, "-------------------------------\n");
	//print out address in hex and then 
	for (int i = 0; i < labels.size; i++){
	        fprintf(output, "0x%08x", labels.table[i].lineNumber * 4);
		fprintf(output, "   %s\n", labels.table[i].name);
	}

}

//examine first few characters and determine what instruction the line represents. Once the instruction is recognized the opcode
//is found in the table of opcodes and the instruction is created. Pesudo instructions create two strings for broken down 
//command finds opcodes and then translates the instructions 
//@param lineNum is the instructions line number. this is used for calculating address offsets
//@param line instruction being decoded
//@pre assumes that line contains no whitespace, labels, or comments
void decodeInstruction(char* line){
	//get a substring of 4 to avoid any conflicts with labels and instruction names
	char linei[4];
	linei[0] = line[0];
	linei[1] = line[1];
	linei[2] = line[2];	
	linei[3] = line[3];
	//Begin L-type checks
	//lw check
	if (strstr(linei, "lw") != NULL){
		
		createSimpleLtype(line, opCodes.lw, false);
	}
	//sw check
	else if (strstr(linei, "sw") != NULL){
		createSimpleLtype(line, opCodes.sw, true);
	}
	//la check
	else if (strstr(linei, "la") != NULL){
		createLa(line, opCodes.addi);
        }
	//li check
	else if (strstr(linei, "li") != NULL){
		createImmRtype(line, opCodes.addiu, 4, 1);
        }
	//begin R-Type checks
	//add check
	else if (strstr(linei, "addiu") != NULL){
		createImmRtype(line, opCodes.addiu, 11, 0);
        }
	//sub check	
	else if (strstr(linei, "sub") != NULL){
		createSimpleRtype(line, opCodes.sub, 0);
        }
	//addi check
	else if (strstr(linei, "addi") != NULL){
		createImmRtype(line, opCodes.addi, 10, 0);
        }
	//addiu check
	else if (strstr(linei, "add") != NULL){
		createSimpleRtype(line, opCodes.add, 0);
        }
	//or check
	else if (strstr(linei, "ori") != NULL){
		createImmRtype(line, opCodes.ori, 9, 0);
        }
	//and check
	else if (strstr(linei, "andi") != NULL){
		createImmRtype(line, opCodes.andi, 10, 0);
        }
	//ori check
	else if (strstr(linei, "or") != NULL){
		createSimpleRtype(line, opCodes.orin, 0);
        }
	//andi check
	else if (strstr(linei, "and") != NULL){
		createSimpleRtype(line, opCodes.andin,  0);
        }
	//slt check
	else if (strstr(linei, "slti") != NULL){
		createImmRtype(line, opCodes.slti, 10, 0);
        }
	//slti check
	else if (strstr(linei, "slt") != NULL){
		createSimpleRtype(line, opCodes.slt, 0);
        }
	//sll check
	else if (strstr(linei, "sll") != NULL){
		createShamtRtype(line, opCodes.sll, 0);
        }
	//srl check
	else if (strstr(linei, "srl") != NULL){
		createShamtRtype(line, opCodes.srl, 1);
        }
	//sra check
	else if (strstr(linei, "sra") != NULL){
		createShamtRtype(line, opCodes.sra, 0);
        }	
	//nop check
	else if (strstr(linei, "nop") != NULL){
		currInstruction = currInstruction + 1;
		printNop(output);
        }
	//Begin Branch L-type checks
	//beq check
	else if (strstr(linei, "beq") != NULL){
		createBranch(line, opCodes.beq, 2, 0);
        }
	//bne check
	else if (strstr(linei, "bne") != NULL){
		createBranch(line, opCodes.bne, 2, 0);
        }
	//bltz check
	else if (strstr(linei, "bltz") != NULL){
		createBranch(line, opCodes.bltz, 1, 0);
        }
	//blez check
	else if (strstr(linei, "blez") != NULL){
		createBranch(line, opCodes.blez, 1, 0);
        }
	//blt check
	else if (strstr(linei, "blt") != NULL){
		 createBlteSlt(line, opCodes.slt, 0);
		 createBlteBne(line, opCodes.bne);
        }
	//ble check
	else if (strstr(linei, "ble") != NULL){
		createBlteSlt(line, opCodes.slt, 1);
		createBlteBne(line, opCodes.beq);
        }
	//j check
	else if (strstr(linei, "jal") != NULL){
		createJtype(line, opCodes.jal, 3);
        }
	//jr check
	else if (strstr(linei, "jr") != NULL){
		createJr(line, opCodes.jr);
        }
	//jal check
	else if (strstr(linei, "j") != NULL){
		createJtype(line, opCodes.j, 1);
        }
	//syscall check
	else if (strstr(line, "syscall") != NULL){
		currInstruction = currInstruction + 1;
		printsyscall(output);
        }

}

//creates a special version of slt when blt/ble is called
void createBlteSlt(char* line, int* opCode, int bleIndicator){
	currInstruction = currInstruction + 1;
        rType instruc;
	//set opCode
	for (int i = 0; i < 6; i++){
                instruc.funct[i] = opCode[i];
        }
	
	for (int i = 0; i < 5; i++){
		instruc.rd[i] = registers.at[i];
		instruc.shamt[i] = 0;
	}

	//get rs and rt
	char regb[100];
        char regb1[100];
        strcpy(regb, line);
        strcpy(regb1, line);
		
	if (bleIndicator == 1){
		char* reg1 = getRegister(regb, 1);
         	decodeRegister(reg1, instruc.rt, 0, 0);
         	char* reg2 = getRegister(regb1, 2);
         	decodeRegister(reg2, instruc.rs, 0, 0);
	}
	else{
		char* reg1 = getRegister(regb, 1);
                decodeRegister(reg1, instruc.rs, 0, 0);
                char* reg2 = getRegister(regb1, 2);
                decodeRegister(reg2, instruc.rt, 0, 0);	
	}
	printSimpleRtype(&instruc, output);	
}

//create special branch instruciont beq/bne for blt/ble
void createBlteBne(char* line, int* opCode){
	currInstruction = currInstruction + 1;
        lType instruc;
	
	for (int i = 0; i < 6; i++){
                instruc.op[i] = opCode[i];
        }

	for (int i = 0; i < 5; i++){
		instruc.rt[i] = registers.at[i];
		instruc.rs[i] = 0;
	}

	
	//get label offset
	char* label = getRegister(line, 3);
        int labelPos = findLabel(label, &labels);
	int offset = labels.table[labelPos].lineNumber - currInstruction;
	
	for (int i = 16; i >= 0; i--){
                int temp = offset >> i;
                instruc.Imm[i] = temp & 1;
        }

        printSimpleLtype(&instruc, output);
}

//creates a new non psuedo Ltype instruction based on a string and then prints it
//@param line contains registers for one instruction with registers seperated by a comma 
//@param dest holds machine code for Ltype instruction
void createSimpleLtype(char* line, int* opCode, bool sw){
	currInstruction = currInstruction + 1;
	//create a new Ltype
	lType instruc;
	//initialize opCode
	for (int i = 0; i < 6; i++){
                instruc.op[i] = opCode[i];
        }
	
	char regb[100];
        char regb1[100];
        strcpy(regb, line);
        strcpy(regb1, line);
	
        //find rt and rd
        	char* reg1 = getRegister(regb, 1);
		decodeRegister(reg1, instruc.rs, 0, 0);
        	char* reg2 = getLRegister(regb1);
        	decodeRegister(reg2, instruc.rt, 0, 0);

        	//insert 16bit offset
        	//TODO find offset in string and turn into binary format something better then hard coded values for array
        	//for now all 0
		int Immediate = findLtypeImmediate(line);
		int immBinary[16] = {0};        
		dTobConverter(Immediate, immBinary, 16);	
	
		//print instruction
		for (int i = 0; i < 16; i++){
			instruc.Imm[i] = immBinary[i];
		}
		printSimpleLtype(&instruc, output);

}


//find immediate in Ltype instructions will be between ',' and '('
int findLtypeImmediate(char* line){
	//used to store trimmed string
	char Imm[4];
	//find first occurence of ,
	char* comma = strchr(line, ',');
	int commaPos = (int) (comma - line) + 1;

	//find first occurence of (
	char* paren = strchr(line, '(');
	int parenPos = (int)( paren - line);
	int immCount = 0;

	//loop through and copy integer offset over
	for (int i =  commaPos; i < parenPos; i++){
		Imm[immCount] = line[i];
		
		immCount++;
	}

	int immediate = atoi(Imm);

	return immediate;
}

//get the second register of a ltype
char* getLRegister(char* line){
	char* token;
	token = strtok(line, "()");
	token = strtok(NULL, "()");

	return token;
}

//creates a new non psuedo Rtype instruction, with a shamt field of all 0s based on the string entered and prints it
//supported instructions add, sub, or, and, slt
//@indicator if 30 then use at as the first register
void createSimpleRtype(char* line, int* funct, int atIndicator){
	 currInstruction = currInstruction + 1;
	 rType instruc;
	 //initialize funct
	 for (int i = 0; i < 6; i++){
		instruc.funct[i] = funct[i];
	 }
	//create buffers for line
	char regb[100];
	char regb1[100];
	char regb2[100];
	strcpy(regb, line);
	strcpy(regb1, line);
	strcpy(regb2, line);

	char* reg1 = getRegister(regb, 1); 	 
        decodeRegister(reg1, instruc.rd, atIndicator, 0);
	
	char* reg2 = getRegister(regb1, 2);
	decodeRegister(reg2, instruc.rs, 0, 0);

	char* reg3 = getRegister(regb2, 3);
        decodeRegister(reg3, instruc.rt, 0, 0);    

        //shamt field for supported instructions is always 0
        for (int i = 0; i < 5; i++){
		instruc.shamt[i] = 0;
        }
	printSimpleRtype(&instruc, output);
}

//create Rtype instruction that uses shamt field held in line with funct field= funct
//supported instructions sll, srl, sra
void createShamtRtype(char* line, int* funct, int srlIndicator){
	currInstruction = currInstruction + 1;
	rType instruc;
        //initialize funct
    	for (int i = 0; i < 6; i++){
               instruc.funct[i] = funct[i];
        }
	//initialize rs
	if (srlIndicator == 0){
		//if not srl rs all 0s
		for (int i = 0; i < 5; i++){
			instruc.rs[i] = 0;
		}
	}
	else{
		 //TODO it is srl 
		 for (int i = 0; i < 5; i++){
                        instruc.rs[i] = 0;
                }
	}
	//buffers for finding registers
	char regb[100];
        char regb1[100];
        strcpy(regb, line);
        strcpy(regb1, line);

	//find rt and rd
	char* reg1 = getRegister(regb, 1);
        int temp1 = decodeRegister(reg1, instruc.rd, 0, 0);

	char* reg2 = getRegister(regb1, 2);
        int temp2 = decodeRegister(reg2, instruc.rt, 0, 0);

	int zeroInd = 0;
	if (temp1 == 1){
		zeroInd++;
	}	
	if (temp2 ==  1){
		zeroInd++;
	}
	//find the sa
	int sa[5];
	int saInt = findShamtSa(line, 5, 9, zeroInd);

	dTobConverter(saInt, sa, 5);
	for (int i = 0; i < 5; i++){

		instruc.shamt[i] = sa[i];
	}
	printSimpleRtype(&instruc, output);
}

//create an Rtype instructions that uses immediates as parameters
//same as lytype except the position of the immediate
//supported instructions addi, addiu, ori, andi, slti, LI
//li is a special case where rs is $zero, so there is an int idicator set to 1 if it is LI
void createImmRtype(char* line, int* opCode, int startPos, int liIndicator){
	currInstruction = currInstruction + 1;
	lType instruc;
	//copy opcode
	for (int i = 0; i < 6; i++){
                instruc.op[i] = opCode[i];
        }
	//create buffers to back up line	
	char regb[100];
        char regb1[100];
        strcpy(regb, line);
        strcpy(regb1, line);

	//get rt
	char* reg1 = getRegister(regb, 1);
        int temp = decodeRegister(reg1, instruc.rt, 0, 0);
	
	int zeroInd = 0;
	if (temp == 1){
		zeroInd++;
	}
	
	//find rs		
	if (liIndicator != 1){
		//find rs
		char* reg2 = getRegister(regb1, 2);
	        int temp2 = decodeRegister(reg2, instruc.rs, 0, 0);
		if (temp2 == 1){
			zeroInd++;
		}
		

	}
	//if instruc is li then rs is all zeros
	else{
		for (int i = 0; i < 5; i++){
			instruc.rs[i] = 0;
		}
	}
	
	int Immediate = findShamtSa(line, 16, startPos, zeroInd);
        int immBinary[16] = {0};
        dTobConverter(Immediate, immBinary, 16);

        for (int i = 0; i < 16; i++){
                instruc.Imm[i] = immBinary[i];
        }
	printImmRtype(&instruc, output);

}

//finds sa for instructions that use shamt field can also be used to find rtype immediates
//@param 16, or 5
//starting position of the immediate
//@return integer representation of sa
//@param zeros tells the number of zero registers used
int findShamtSa(char* line, int size, int pos, int zeros ){
	char sa[size];
	//sa will be after third ',' in line
	//get array index of start of sa, always 11
	//counter for sa
	int saCount = 0;
	
	
	for(int i = pos + 2 + (zeros * 2); i < strlen(line); i++){
		sa[saCount] = line[i];
		printf("%c", line[i]);
		saCount++;
	}
	int saInt = atoi(sa);
	return saInt;
}

//create a branch type instruction based on line
//@param line current line in input
//@param opCode of instruction
//@param numberOfregisters the instruction takes as an input
//supported instructions: beq, bne, bltz, blez
void createBranch(char* line, int* opCode, int numberOfregisters, int atIndicator){
	currInstruction = currInstruction + 1;
	lType instruc;
	//copy opcode
	for (int i = 0; i < 6; i++){
                instruc.op[i] = opCode[i];
        }
		
	char regb[100];
        char regb1[100];
        strcpy(regb, line);
        strcpy(regb1, line);
	
	//decode registers
	char* reg1 = getRegister(regb, 1);
        decodeRegister(reg1, instruc.rt, 0, 0);
        
	if (numberOfregisters == 2){
		 char* reg2 = getRegister(regb1, 2);
        	 decodeRegister(reg2, instruc.rs, 0, 0);
	}
	else{
		for (int i = 0; i < 5; i++){
			instruc.rs[i] = 0;
		}
	}
	
	//find label
	char* label;
	if (numberOfregisters == 2){
		label = getRegister(line, 3);
	}
	else{
		label = getRegister(line, 2);
	}
	int labelPos = findLabel(label, &labels);
	//calculate address offset
	int offset = labels.table[labelPos].lineNumber - currInstruction;		
	
	//set Imm based on label
	for (int i = 16; i >= 0; i--){
		int temp = offset >> i;
		instruc.Imm[i] = temp & 1;
	}

	printSimpleLtype(&instruc, output);
}

//create new jtype instruction based on line
//@param line holds current instruction
//@param opCode for jtype
//supported instructions: j, jal
void createJtype(char* line, int* opCode, int size){
	currInstruction = currInstruction + 1;
	jType instruc;
	
	//set opCode
	for (int i = 0; i < 6; i++){
                instruc.op[i] = opCode[i];
        }
	//find label
	char* label = getJRegister(line, size);
	int labelPos = findLabel(label, &labels);
	
	for (int i = 0; i < 16; i++){
	
                instruc.Imm[i] = labels.table[labelPos].address[i];
        }
	//set the last bits to 0
	for (int i = 16; i < 26; i++){
		instruc.Imm[i] = 0;
	}
	printSimpleJtype(&instruc, output);
}

//get label assiciate with jump instruction
char* getJRegister(char* line, int size){
	char* label = malloc(sizeof(char) * (strlen(line) - size));
	int counter = 0;
        for (int i = size; i < strlen(line); i++){
		label[counter] = line[i];
		counter++;
	}
        return label;
}


//print out jr special case different from the rest of the registers
void createJr(char* line, int* opCode){
	currInstruction = currInstruction + 1;
	rType instruc;
	//set opcode, rs, rt
	for (int i = 0; i < 5; i++){
		instruc.rt[i] = 0;
		instruc.rd[i] = 0;
		instruc.funct[i] = opCode[i];
        }
	instruc.funct[5] = opCode[5];
	for (int i = 0; i < 5; i++){
		instruc.shamt[i] = 0;
	}
	//find register
	decodeRegister(line, instruc.rs, 0, 0);
	printSimpleRtype(&instruc, output);
	
}

//create a la instruction
void createLa(char* line, int* opCode){
	currInstruction = currInstruction + 1;
	lType instruc;
	
	for (int i = 0; i < 6; i++){
		instruc.op[i] = opCode[i];
	}
	
	decodeRegister(line, instruc.rs, 0,  0);
	for (int i = 0; i < 5; i++){
		instruc.rt[i] = 0;
	}
        	
	//create a substring that contains the label
	char* label = getRegister(line, 2);

	int labelPos = findLabel(label, &labels);
	for (int i = 0; i < 16; i++){
                instruc.Imm[i] = labels.table[labelPos].address[i];
        }

        printSimpleLtype(&instruc, output);

}

//add$s2,$s0,$s1
//get specified register from the line and retrun it
char* getRegister(char* line, int regNum){
	char* token;
	token = strtok(line, ",$");
	for (int i = 0; i < regNum; i++){
		token = strtok(NULL, ",$");
	}
	return token;
}

//@param line current line being decoded
//@param reg empty array to be initialized as register code in binary
//@return register that was found to signify first register in instruction 0-31
int decodeRegister(char* line, int* reg, int prevReg, int prevReg1){
	if (prevReg == 30){
                copyReg(registers.at, reg);
        }
	else if (strstr(line, "zero") && prevReg != 1 && prevReg1 != 1){
	      copyReg(registers.zero, reg);
	      return 1;
	}
	else if (strstr(line, "v0") && prevReg != 2 && prevReg1 != 2){
	      copyReg(registers.v0, reg);
	      return 2;
	}
	else if (strstr(line, "v1") && prevReg != 3 && prevReg1 != 3){
	      copyReg(registers.v1, reg);
	      return 3;
	}
	else if (strstr(line, "a0") && prevReg != 4 && prevReg1 != 4){
	      copyReg(registers.a0, reg);
	      return 4;
	}
	else if (strstr(line, "a1") && prevReg != 5 && prevReg1 != 5){
	      copyReg(registers.a1, reg);
	      return 5;
	}
	else if (strstr(line, "a2") && prevReg != 6 && prevReg1 != 6){
	      copyReg(registers.a2, reg);
	      return 6;
	}
	else if (strstr(line, "a3") && prevReg != 7 && prevReg1 != 7){
	      copyReg(registers.a3, reg);
	      return 7;
	}
	else if (strstr(line, "t0") && prevReg != 8 && prevReg1 != 8){
	      copyReg(registers.t0, reg);
	      return 8;
	}
	else if (strstr(line, "t1") && prevReg != 9 && prevReg1 != 9){
	      copyReg(registers.t1, reg);
	      return 9;
	}
	else if (strstr(line, "t2") && prevReg != 10 && prevReg1 != 10){
	      copyReg(registers.t2, reg);
	      return 10;
	}
	else if (strstr(line, "t3") && prevReg != 11 && prevReg1 != 11){
	      copyReg(registers.t3, reg);
	      return 11;
	}
	else if (strstr(line, "t4") && prevReg != 12 && prevReg1 != 12){
	      copyReg(registers.t4, reg);
	      return 12;
	}
	else if (strstr(line, "t5") && prevReg != 13 && prevReg1 != 13){
	      copyReg(registers.t5, reg);
	      return 13;
	}
	else if (strstr(line, "t6") && prevReg != 14 && prevReg1 != 14){
	      copyReg(registers.t6, reg);
	      return 14;
	}
	else if (strstr(line, "t7") && prevReg != 15 && prevReg1 != 15){
	      copyReg(registers.t7, reg);
	      return 15;
	}
	else if (strstr(line, "s0") && prevReg != 16 && prevReg1 != 16){
	      copyReg(registers.s0, reg);
	      return 16;
	}
	else if (strstr(line, "s1") && prevReg != 17 && prevReg1 != 17){
	      copyReg(registers.s1, reg);
	      return 17;
	}
	else if (strstr(line, "s2") && prevReg != 18 && prevReg1 != 18){
	      copyReg(registers.s2, reg);
	      return 18;
	}
	else if (strstr(line, "s3") && prevReg != 19 && prevReg1 != 19){
	      copyReg(registers.s3, reg);
	      return 19;
	}
	else if (strstr(line, "s4") && prevReg != 20 && prevReg1 != 20){
	      copyReg(registers.s4, reg);
	      return 20;
	}
	else if (strstr(line, "s5") && prevReg != 21 && prevReg1 != 21){
	      copyReg(registers.s5, reg);
	      return 21;
	}
	else if (strstr(line, "s6") && prevReg != 22 && prevReg1 != 22){
	      copyReg(registers.s6, reg);
	      return 22;
	}
	else if (strstr(line, "s7") && prevReg != 23 && prevReg1 != 23){
	      copyReg(registers.s7, reg);
	      return 23;
	}
	else if (strstr(line, "t8") && prevReg != 24 && prevReg1 != 24){
	      copyReg(registers.t8, reg);
	      return 24;
	}
	else if (strstr(line, "t9") && prevReg != 25 && prevReg1 != 25){
	      copyReg(registers.t9, reg);
	      return 25;
	}
	else if (strstr(line, "gp") && prevReg != 26 && prevReg1 != 26){
	      copyReg(registers.gp, reg);
	      return 26;
	}
	else if (strstr(line, "sp") && prevReg != 27 && prevReg1 != 27){
	      copyReg(registers.sp, reg);
	      return 27;
	}
	else if (strstr(line, "fp") && prevReg != 28 && prevReg1 != 28){
	      copyReg(registers.fp, reg);
	      return 28;
	}
	else if (strstr(line, "ra") && prevReg != 29 && prevReg1 != 29){
	      copyReg(registers.ra, reg);
	      return 29;
	}
	else if (strstr(line, "at") ){
		copyReg(registers.at, reg);
	}
	return 0;
}


// copy one register array to anothe array
void copyReg(int* source, int* dest){
	for(int i = 0; i < 5; i++){
	      dest[i] = source[i];
	}
}

//converts a integer to binary in the form of an integer array
//@param Imm integer to be converted
//@param binary array binary representation will be stored in
//@param length of binary array
void dTobConverter(int Imm, int* binary, int length){
	int temp;
	for (int i = 31; i >= 0; i--){
		temp = Imm >> i;
		if (i < length){
			if (temp & 1){
				binary[i] = 1;
			}
			else{
				binary[i] = 0;
			}
		}
	}
}


