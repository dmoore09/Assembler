#include <stdbool.h>
//handles all .word declarations
int processWord(char* line, FILE *output);

int processASC(char* line, FILE *output);
 
void createVar(char* line, FILE *output);

int createArrayInit(char* line, FILE *output);

int createArrayInitAll(char* line, FILE *output);

void printBinaryChar(char c, FILE *output);

int colonCount(char* line);

void dTobPrinter(int Imm, FILE* output);



