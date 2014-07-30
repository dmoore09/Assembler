#include <stdio.h>
#include "instructionprinter.h"
//#include "staticdata.h"
//print out an ltype record based on instru
void printSimpleLtype(lType* instruc, FILE *output){
	//print opCode
	for (int i = 0; i < 6; i++){
		fprintf(output,"%d", instruc->op[i]);
	}
	//print out rs
	for (int i = 0; i < 5; i++){
                fprintf(output,"%d", instruc->rt[i]);
        }
	//print out rt
	for (int i = 0; i < 5; i++){
                fprintf(output,"%d", instruc->rs[i]);
        }
	//print out 16 bit Immediate
	for (int i = 15; i >= 0; i--){
                fprintf(output,"%d", instruc->Imm[i]);
        }
	//end the line
	fprintf(output, "\n");
}

void printImmRtype(lType* instruc, FILE *output){
	for (int i = 0; i < 6; i++){
                fprintf(output,"%d", instruc->op[i]);
        }
	//rs
	for (int i = 0; i < 5; i++){
                fprintf(output,"%d", instruc->rs[i]);
        }
	//rt
	 for (int i = 0; i < 5; i++){
                fprintf(output,"%d", instruc->rt[i]);
        }
	//16 bit
	for (int i = 15; i >= 0; i--){
                fprintf(output,"%d", instruc->Imm[i]);
        }
	fprintf(output, "\n");
}

//print out an rtype record based on instruc
void printSimpleRtype(rType* instruc, FILE *output){
	//print opCode
	for (int i = 0; i < 6; i++){
		fprintf(output, "0");
	}
	//print out rs
	for (int i = 0; i < 5; i++){
		fprintf(output,"%d", instruc->rs[i]);
	}
	//print out rt
	for (int i = 0; i < 5; i++){
		fprintf(output,"%d", instruc->rt[i]);
	}
	//print out rd bit Immediate
	for (int i = 0; i < 5; i++){
		fprintf(output,"%d", instruc->rd[i]);
	}
	//print out shamt
	for (int i = 4; i >= 0; i--){
		fprintf(output,"%d", instruc->shamt[i]);
	}
	//print out funct
	for (int i = 0; i < 6; i++){
		fprintf(output,"%d", instruc->funct[i]);
	}
	fprintf(output,"\n");
}

//TODO make sure IMM prints out in right order
//print out a jtype record based on instruc
void printSimpleJtype(jType* instruc, FILE *output){
	//print opCode
	for (int i = 0; i < 6; i++){
		fprintf(output, "%d", instruc->op[i]);
	}
	//print out Imm
	for (int i = 25; i >=0; i--){
		fprintf(output,"%d", instruc->Imm[i]);
	}
	fprintf(output, "\n");
}

//print out a nop, 32 zeros
void printNop(FILE *output){
	for (int i = 0; i < 32; i++){
		fprintf(output, "0");
	}
	fprintf(output, "\n");
}

//print out syscall
void printsyscall(FILE *output){
	for (int i = 0; i < 28; i++){
		fprintf(output, "0");
	}
	fprintf(output, "1");
	fprintf(output, "1");
	fprintf(output, "0");
	fprintf(output, "0 \n");
}

void printSw(lType* instruc, FILE *output){
	 for (int i = 0; i < 6; i++){
                fprintf(output,"%d", instruc->op[i]);
        }
	for (int i = 0; i < 5; i++){
                fprintf(output,"%d", instruc->rs[i]);
        }
	for (int i = 0; i < 5; i++){
                fprintf(output,"%d", instruc->rt[i]);
        }
	for (int i = 15; i >= 0; i--){
                fprintf(output,"%d", instruc->Imm[i]);
        }
	fprintf(output, "\n");	

}
