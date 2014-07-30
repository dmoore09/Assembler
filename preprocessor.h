#include "labelHandler.h"
#ifndef PREPROCCESSOR_H
#define PREPROCCESSOR_H
//header file for preproccessor
void preProcess(char* line, char* dest, FILE* output);

void removeWhiteSpace(char* line, char* dest);

Label* detectLabel(char* line, int instructionNumber);
#endif
