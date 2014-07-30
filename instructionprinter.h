#include "staticdata.h"
#ifndef INSTRUCTIONPRINTER_H
#define INSTRUCTIONPRINTER_H
//header file for insructionprinter
void printSimpleLtype(lType* instruc, FILE *output);

void printImmRtype(lType* instruc, FILE *output);

void printSimpleRtype(rType* instruc, FILE *output);

void printSimpleJtype(jType* instruc, FILE *output);

void printNop(FILE *output);

void printsyscall(FILE *output);

void printSw(lType* instruc, FILE *output);
#endif
