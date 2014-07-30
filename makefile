assembler: assembler.c preprocessor.c preprocessor.h instructionprinter.c instructionprinter.h labelHandler.c labelHandler.h wordHangler.c wordHangler.h staticdata.h
	gcc -std=c99 -o assemble -Wall assembler.c preprocessor.c instructionprinter.c labelHandler.c wordHangler.c

