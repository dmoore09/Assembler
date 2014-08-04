CS 2506 Computer Organization II 
C Programming 3: Simple MIPS Assembler 

The Assignment 

Recall that an assembler translates code written in mnemonic form in assembly language into machine code. 

You will implement an assembler that supports a subset of the MIPS32 assembly language (specified below). The 
assembler will be implemented in C and executed on Linux. Your assembler will take a file written in assembly language, 
and produce an output file containing the corresponding MIPS machine code (in text format). 

Supported MIPS32 Assembly Language 

The subset of the MIPS assembly language that you need to implement includes the following instructions. The following 
conventions are observed in this section: 

• The notation (m:n)refers to a range of bits in the value to which it is applied. For example, the expression 
  (PC+4)(31:28) refers to the high 4 bits of the address PC+4. 

• imm16refers to a 16-bit immediate value; no assembly instruction will use a longer immediate

• offsetrefers to a literal applied to an address in a register; e.g., offset(rs); offsets will be signed 16-bit 
  values 
  
• labelfields map to addresses, which will always be 16-bit values if you follow the instructions 

• sarefers to the shift amount field of an R-format instruction; shift amounts will be nonnegative 5-bit values 

• targetrefers to a 28-bit word address for an instruction; usually a symbolic notation 

• Sign-extension of immediates is assumed where necessary and not indicated in the notation. 

• C-like notation is used in the comments for arithmetic and logical bit-shifts: >>a , <<l and >>l 

• The C ternary operator is used for selection: condition ? if-true : if-false 

• Concatenation of bit-sequences is denoted by ||. 

• A few of the specified instructions are actually pseudo-instructions. That means your assembler must replace each 
  of them with a sequence of one or more other instructions; see the comments for details. 
  You will find the MIPS32 Architecture Volume 2: The MIPS32 Instruction Set to be a useful reference for machine 
  instruction formats and opcodes, and even information about the execution of the instructions. See the Resources page on 
  the course website. 

MIPS32 assembly .data section: 

.word This is used to hold one or more 32 bit quantities, initialized with given values. For example: 

var1: .word 15 
# creates one 32 bit integer called var1 and initializes 
# it to 15. 


array1: .word 2, 3, 4, 5, 6, 7 
# creates an array of 6 32-bit integers, 
# initialized as indicated (in order). 


array2: .word 2:10 
# creates an array of 10 32 bit integers and initializes 
# element of the array to the value 2. 


.asciiz This is used to hold a NULL (0) terminated ASCII string. For example: 

hello_w: .asciiz “hello world” 
# this declaration creates a NULL 
# terminated string with 12 characters 
# including the terminating 0 byte 


Version 2.0. 


CS 2506 Computer Organization II 
C Programming 3: Simple MIPS Assembler 

MIPS32 assembly .text section: 

Your assembler must support translation of all of the following MIPS32 assembly instructions. You should consult the 
MIPS32 Instruction Set reference for opcodes and machine instruction format information. That said, we will evaluate your 
output by comparing it to the output produced by MARS 4.4; if there are any disagreements between the MIPS32 manual 
and MARS, follow the approach taken by MARS. 

Your assembler must support the following load and store instructions: 

lw rt, offset(rs) 
# Transfers a word from memory to a register. 
# GPR[rt] <--Mem[GPR[rs] + offset] 


sw rt, offset(rs) 
# Transfers a word from a register to memory 
# Mem[GPR[rs] + offset] <--GPR[rt] 


Note that the constant offset in the load and store instructions (lwand sw) may be positive or negative. 

la rd, label 
# load address label to register 
# GPR[rd] <--label 
# 
# pseudo-translation for 16-bit label: 
# addi rd, $zero, label 


li rd, imm16 
# load address label to register 
# GPR[rd] <--imm 
# 
# pseudo-translation: 
# addiu rd, $zero, imm16 


Your assembler must support the following arithmetic/logical instructions: 

add rd, rs, rt 
# signed addition of integers 
# GPR[rd] <--GPR[rs] + GPR[rt] 


sub rd, rs, rt 
# signed subtraction of integers 
# GPR[rd] <--GPR[rs] -GPR[rt] 


addi rt, rs, imm16 
# signed addition with 16-bit immediate 
# GPR[rt] <--GPR[rs] + imm16 


addiu rt, rs, imm16 
# unsigned addition with 16-bit immediate 
# GPR[rt] <--GPR[rs] + imm16 


or rd, rs, rt 
# bitwise logical OR 
# GPR[rd] <--GPR[rs] OR GPR[rt] 


and rd, rs, rt 
# bitwise logical AND 
# GPR[rd] <--GPR[rs] AND GPR[rt] 


ori rd, rs, imm16 
# bitwise logical OR with 16-bit immediate 
# GPR[rd] <--GPR[rs] OR imm16 


andi rd, rs, imm16 
# bitwise logical AND with 16-bit immediate 
# GPR[rd] <--GPR[rs] AND imm16 


slt rd, rs, rt 
# set register to result of comparison 
# GPR[rd] <--(GPR[rs] < GPR[rt] ? 0 : 1) 


Version 2.0. 


CS 2506 Computer Organization II 
C Programming 3: Simple MIPS Assembler 

slti rd, rs, imm16 
# set register to result of comparison 
# GPR[rd] <--(GPR[rs] < imm16 ? 0 : 1) 


sll rd, rs, sa 
# logical shift left a fixed number of bits 
# GPR[rd] <--GPR[rs] <<l sa 


srl rd, rs, sa 
# logical shift right a fixed number of bits 
# GPR[rd] <--GPR[rs] >>l sa 


sra rd, rs, sa 
# arithmetic shift right a fixed number of bits 
# GPR[rd] <--GPR[rs] >>a sa 


nop 
# no operation 
# executed as: 
# sll $zero, $zero, 0 


Your assembler must support the following control-of-flow instructions: 

beq rs, rt, offset 
# conditional branch if rs == rt 
# PC <--(rs == rt ? PC + 4 + offset <<l 2) 
# :PC+4) 


bne rs, rt, offset 
# conditional branch if rs != rt 
# PC <--(rs != rt ? PC + 4 + offset <<l 2) 
# :PC+4) 


bltz rs, offset 
# conditional branch if rs < 0 
# PC <--(rs < 0 ? PC + 4 + offset <<l 2) 
# :PC+4) 


blez rs, offset 
# conditional branch if rs <= 0 
# PC <--(rs <= 0 ? PC + 4 + offset <<l 2) 
# :PC+4) 


blt rs, rt, offset 
# conditional branch if rs < rt 
# PC <--(rs < rt ? PC + 4 + offset <<l 2) 
# :PC+4) 
# 
# pseudo-translation: 
# sub at, rs, rt slt at, rs, rt 
# bltz at, offset bne at, zero, offset 


ble rs, rt, offset 
# conditional branch if rs <= rt 
# PC <--(rs <= rt ? PC + 4 + offset <<l 2) 


# : PC + 4) 
# 
# pseudo-translation: 
# sub at, rs, rt slt at, rt, rs 
# blez at, offset beq at, zero, offset 

j target 
# unconditional branch 
# PC <--( (PC+4)(31:28) || (target <<l 2)) 


jr rs 
# unconditional branch 
# PC <--GPR[rs] 


Version 2.0. 


CS 2506 Computer Organization II 
C Programming 3: Simple MIPS Assembler 

jal target 
# unconditional branch for procedure call 
# GPR[ra] <--PC + 8 
# PC <--( (PC+4)(31:28) || (target <<l 2)) 


syscall 
# invoke exception handler, which examines $v0 
# to determine appropriate action; if it returns, 
# returns to the succeeding instruction; see the 
# MIPS32 Instruction Reference for format 


MIPS32 assembly format constraints: 

The assembly programs will satisfy the following constraints: 

• 
Labels will begin in the first column of a line, and will be no more than 32 characters long. Labels are restricted to 
alphanumeric characters and underscores, and are always followed immediately by a colon character (‘:’). 
• 
Labels in the .textsegment will always be on a line by themselves. 
• 
Labels in the .datasegment will always occur on the same line as the specification of the variable being defined. 
• 
Labels are case-sensitive; that actually makes your task a bit simpler. 
• 
MIPS instructions do not begin in a fixed column; they are preceded by an arbitrary amount of whitespace 
(possibly none). 
• 
Blank lines may occur anywhere; a blank line will always contain only a newline character. 
• 
Whitespace will consist of spaces, tab characters, or a mixture of the two. Your parsing logic must handle that. 
• 
Registers will be referred to by symbolic names ($zero, $t5) rather than by register number. 
• 
Instruction mnemonics and register names will use lower-case characters. 
• 
Assembly source files will always be in UNIX format. 
You must be sure to test your implementation with all the posted test files; that way you should avoid any unfortunate 
surprises when we test your implementation. 

Input 

The input files will be MIPS assembly programs in ASCII text. The assembly programs will be syntactically correct, 
compatible with the MARS MIPS simulator, and restricted to the subset of the MIPS32 instruction set defined above. 
Example programs will be available from the course website. 

Each line in the input assembly file will contain either an assembly instruction, a section header directive (such as .data) 
or a label (a jump or branch target). The maximum length of a line is 256 bytes. 

Your input file may also contain comments. Any text after a ‘#’symbol is a comment and should be discarded by your 
assembler. Section header directives, such as .dataand .textwill be in a line by themselves. Similarly, labels (such as 
loop:) will be on a line by themselves. The input assembly file will only contain one data section and one text section. 
The first section in the file will be the text section, followed by the data section. 

Your assembler can be invoked in the following ways: 

assemble <input file> <output file> 
assemble –symbols <input file> <output file> 
assemble –list <input file> <output file> 


The specified input file must already exist; if not, your program should exit gracefully with an error message to the console 
window. The specified output file may or may not already exist; if it does exist, the contents should be overwritten. 

Version 2.0. 


CS 2506 Computer Organization II 
C Programming 3: Simple MIPS Assembler 

Output 
Output when invoked as: assemble <input file> <output file> 


Your assembler will resolve all references to branch targets in the .text section and variables in the .data section and convert 
the instructions in the .textsection into machine code. 

To convert an instruction into machine code follow the instruction format rules specified in the class textbook. For each 
format (R-format, I-format or J-format), you should determine the opcode that corresponds to instruction, the values for the 
register fields and any optional fields such as the function code and shift amount fields for arithmetic instructions (R-
format) and immediate values for I-format instructions. 

The output machine code should be saved to the output file specified in the command line. The output file should contain 
the machine code corresponding to instructions from the .textsection followed by a blank line followed by variables 
from the .datasection in human readable binary format (0s and 1s). For example to represent the decimal number 40in 
16-bit binary you would write 0000000000101000, and to represent the decimal number -40 in 16-bit binary you would 
write 1111111111011000. 

The output file is a text file, not a binary file; that’s a concession to the need to evaluate your results. 

Your output file should match the machine file generated by the MARS simulator version 4.4 (see following section). A 
sample showing the assembler’s translation of the adder.asmprogram is given at the end of this specification. 

Output when invoked as: assemble –symbols <input file> <output file> 

Your assembler will write (to the specified output file) a well-formatted table, listing every symbolic name used in the 
MIPS32 assembly code and the address that corresponds to that label. Addresses will be written in hex. 

Note that MARS makes the following assumptions about addresses: 

• The base address for the text segment is 0x00000000, so that’s the address of the first machine instruction. 
• The base address of the data segment is 0x00002000, so that’s the address of the first thing declared in the data 
  segment. 
  
The second fact above implies that the text segment cannot be longer than 8 KiB or 2048 machine instructions. You don’t 
need to do anything special about that fact. 

Output when invoked as: assemble –list <input file> <output file>* 

Your assembler will translate the assembly program as when invoked with no switch (i.e., as described in the first output 
description above). Your assembler will then write a listing file with the following format: 

For each machine instruction in the text segment, write a single line containing the address, machine instruction, and 
assembly instruction on the same line. For pseudo-instructions, show the original assembly instruction in the listing. For 
pseudo-instruction translations that result in “extra” machine instructions, such as blt, write a separate line for each 
“extra” instruction. Write labels that correspond to instructions in the text segment on the line before the corresponding 
instruction. Write a single blank line after the text segment listing. 

For each variable declared in the data segment, write the variable name (label) followed by a colon on a line, then write the 
address and corresponding data content, one 32-bit word per line. Write a single blank line after the data segment listing. 

Sample output files are given later in this specification. 

* worth 10% extra credit 
Version 2.0. 


CS 2506 Computer Organization II 
C Programming 3: Simple MIPS Assembler 

How can I verify my output or test my code? 

In order to test your assembler you should use the MARS simulator, version 4.4. You will need to do the following steps in 
the MARS simulator: 

1. First, ensure that MARS is configured to start the text segment at address 0x00000000. If you do not do this, 
MARS will use default values for the start addresses for the text and data segments, and that will result in 32-bit 
addresses for various labels. 

i) Open the MARS simulator and modify the memory configuration settings through Settings->Memory 
Configuration and select Compact, Text at Address 0 and DO NOT modify any of the remaining 
addresses on the right. Click Apply and Close to exit memory configuration settings. 

2. Now, in order to generate the actual machine file for the assembly program, you will need to dump the binary text 
format for the text and data sections. Load your program into MARS and select Run->Assemble. 

i) Generate machine code for .text section of your assembly program. 

Open the assembly program and select File->Dump memory. Select the .text (0x00000000 0x00000044) 
in the Memory Segment and select Binary Text for dump format and click on dump 
to a file button and specify an output file (say text_segment_of_add_asm.txt) to dump the machine code 
of the assembly program. Now you have the machine instructions for your text segment of your assembly 
program. 

Note that the actual end address of .textsegment will vary depending on your assembly program. For 
the sample input file add.asm, attached with this project, you will find that the text segment starts at 
0x00000000and extends up to 0x00000044. 

ii) Generate machine code for your .datasegment of your assembly program. 

a) Open the assembly program and select File->Dump memory from the MARS IDE. 

b) Select the .data (0x00002000 -0x00002ffc) in the Memory Segment and select Binary Text 
for dump format and click on dump to a file button. Give an output file (say 
data_segment_of_add_asm.txt) to dump the machine code of the assembly program. Now you have 
the machine instructions for your data segment of your assembly program. 

Note that the actual end address of .datasection will vary depending on your assembly program. 
For the add.asmexample above, you will find that data segment starts at 0x00002000and 
extends up to 0x00002ffc. Similar to the text segment, the actual size of the data segment depends 
on your input file. 

iii) Copy the contents of your “text_segment_of_adder_asm.txt” and your “data_segment_of_adder_asm.txt” 
with a blank line in between them to distinguish the two segments into another file (say add_asm.txt). 
Now you have your output machine file ready. Your assembler program should produce an output 
identical to this machine file, except that your representation of the .datasegment should end with the 
last byte of the final variable declared within it. 

The sample machine file posted with this project on the course website was generated using the above approach. Note that 
the text segment should always start at 0x00000000 and the data segment should start at 0x00002000. 

MARS generates a fixed-size data segment, and will pad with 0’s to achieve that. Your assembler should not do this. 

It is also possible to run MARS from the command-line and generate the dumps of the text and data segments that way; be 
aware that MARS generates somewhat different results when run in that manner. 

Version 2.0. 


CS 2506 Computer Organization II C Programming 3: Simple MIPS Assembler 

Sample Assembler Input 

# adder.asm 
# 
# The following program initializes an array of 10 elements and computes 
# a running sum of all elements in the array. The program prints the sum 
# of all the entries in the array. 


.text 


main: 
la $a0, array # load address of array 
la $a1, array_size # load address of array_size 
lw $a1, 0($a1) # load value of array_size variable 


loop: 
sll $t1,$t0,2 #t1=(i*4) 
add $t2, $a0, $t1 # t2 contains address of array[i] 
sw $t0, 0($t2) # array[i] = i 
addi$t0,$t0,1 #i=i+1 
add $t4, $t4, $t0 # sum($t4) = ($t4 + array[i]) 
slt $t3, $t0, $a1 # $t3 = ( i < array_size) 
bne $t3, $zero, loop # if ( i < array_size ) then loop 


li $v0, 4 # system call to print string 
la $a0, message # load address of message into arg register 
syscall # make call 


li $v0, 1 # system call to print an integer 
or $a0, $t4, $zero # load value to print into arg register 
syscall # make call 


li $v0, 10 # system call to terminate program 
syscall # make call 


.data 
message: .asciiz "The sum of numbers in array is: " 
array: .word 0:10 # array of 10 words 
array_size: .word 10 # size of array 


Version 2.0. 


CS 2506 Computer Organization II C Programming 3: Simple MIPS Assembler 

Sample Assembler Output 

Here’s my assembler’s output when invoked as assemble adder.asm adder.asm: 

00100000000001000010000000100100 
00100000000001010010000001001100 
10001100101001010000000000000000 
00000000000010000100100010000000 
00000000100010010101000000100000 
10101101010010000000000000000000 
00100001000010000000000000000001 
00000001100010000110000000100000 
00000001000001010101100000101010 
00010101011000001111111111111001 
00100100000000100000000000000100 
00100000000001000010000000000000 
00000000000000000000000000001100 
00100100000000100000000000000001 
00000001100000000010000000100101 
00000000000000000000000000001100 
00100100000000100000000000001010 
00000000000000000000000000001100 
00100000011001010110100001010100 
00100000011011010111010101110011 
01101110001000000110011001101111 
01100101011000100110110101110101 
01101001001000000111001101110010 
01110010011000010010000001101110 
00100000011110010110000101110010 
00100000001110100111001101101001 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000000000 
00000000000000000000000000001010 
la 
la 
lw 
sll 
add 
sw 
addi 
add 
slt 



Text segment of adder.asm 
bne 
li 
la 
syscall 
li 
or 
syscall 
li 
syscall 

blank line 

message 


Data segment of adder.asm 

array 


array_size 


Data segment notes: 

The characters making up messageare written in a rather puzzling manner. If you examine the bytes of the first line 
of the data segment: 

00100000 01100101 01101000 01010100represents the characters “ ehT” 

So, the characters are displayed in 4-byte chunks, in something like big-endian order. The last character stored for the 
message is 01101001or ‘i’. That is followed by a zero-byte to terminate the string. That is followed by three 
more zero-bytes to pad to the next value; that’s because the next value is a 32-bit integer and MIPS requires 32-bit 
values to be aligned on word boundaries. 

Next, arrayis stored as a sequence of 32-bit values, all zeros. 

Version 2.0. 


CS 2506 Computer Organization II C Programming 3: Simple MIPS Assembler 

The value of array_sizeis 10, which is expressed in hex as 0x0000000A. In the data segment display above, the 
value is displayed in big-endian byte order: 

00000000 00000000 00000000 00001010 
Low address High address 


You must be sure that you write the bytes of integers in big-endian order as well. 

Here’s my assembler’s output when invoked as assemble -symbols adder.asm adder.sym: 

Address Symbol 


0x00000000 main 
0x0000000C loop 
0x00002000 message 
0x00002024 array 
0x0000204C array_size 


The order in which you display the symbols is up to you. My implementation writes them in the order they occur in the 
source file, but that is not a requirement. 

Version 2.0. 


CS 2506 Computer Organization II C Programming 3: Simple MIPS Assembler 

Here’s my assembler’s output when invoked as assemble -list adder.asm adder.lst: 

main: 
0x00000000 00100000000001000010000000100100 la $a0, array 
0x00000004 00100000000001010010000001001100 la $a1, array_size 
0x00000008 10001100101001010000000000000000 lw $a1, 0($a1) 
loop: 
0x0000000C 00000000000010000100100010000000 sll $t1, $t0, 2 
0x00000010 00000000100010010101000000100000 add $t2, $a0, $t1 
0x00000014 10101101010010000000000000000000 sw $t0, 0($t2) 
0x00000018 00100001000010000000000000000001 addi $t0, $t0, 1 
0x0000001C 00000001100010000110000000100000 add $t4, $t4, $t0 
0x00000020 00000001000001010101100000101010 slt $t3, $t0, $a1 
0x00000024 00010101011000001111111111111001 bne $t3, $zero, loop 
0x00000028 00100100000000100000000000000100 li $v0, 4 
0x0000002C 00100000000001000010000000000000 la $a0, message 
0x00000030 00000000000000000000000000001100 syscall 
0x00000034 00100100000000100000000000000001 li $v0, 1 
0x00000038 00000001100000000010000000100101 or $a0, $t4, $zero 
0x0000003C 00000000000000000000000000001100 syscall 
0x00000040 00100100000000100000000000001010 li $v0, 10 
0x00000044 00000000000000000000000000001100 syscall 
message: 
0x00002000 00100000011001010110100001010100 
0x00002004 00100000011011010111010101110011 
0x00002008 01101110001000000110011001101111 
0x0000200C 01100101011000100110110101110101 
0x00002010 01101001001000000111001101110010 
0x00002014 01110010011000010010000001101110 
0x00002018 00100000011110010110000101110010 
0x0000201C 00100000001110100111001101101001 
0x00002020 00000000000000000000000000000000 
array: 
0x00002024 00000000000000000000000000000000 
0x00002028 00000000000000000000000000000000 
0x0000202C 00000000000000000000000000000000 
0x00002030 00000000000000000000000000000000 
0x00002034 00000000000000000000000000000000 
0x00002038 00000000000000000000000000000000 
0x0000203C 00000000000000000000000000000000 
0x00002040 00000000000000000000000000000000 
0x00002044 00000000000000000000000000000000 
0x00002048 00000000000000000000000000000000 
array_size: 
0x0000204C 00000000000000000000000000001010 

Version 2.0. 


CS 2506 Computer Organization II C Programming 3: Simple MIPS Assembler 

Advice 

The following observations are purely advisory, but are based on my experience, including that of implementing a solution 
to this assignment. These are advice, not requirements. 

First, and most basic, analyze what your assembler must do and design a sensible, logical framework for making those 
things happen. There are fundamental decisions you must make early in the process of development. For example, you 
could represent the machine instructions in a number of ways as you build them. They can be represented as arrays of 
individual bits (which could be integers or characters), or they can be represented in binary format, which would be the 
expected format for a "real" assembler's final output. I am not convinced that either of those approaches is inherently 
better, or that there are not reasonable alternatives. But, this decision has ramifications that will propagate throughout your 
implementation. 

It helps to consider how you would carry out the translation from assembly code to machine instructions by hand. If you do 
not understand that, you are trying to write a program that will do something you do not understand, and your chances of 
success are reduced to sheer dumb luck. 

Second, and also basic, practice incremental development! This is a sizeable program, especially so if it's done properly. 
My solution, including comments, runs something over 1500 lines of code. It takes quite a bit of work before you have 
enough working code to test on full input files, but unit testing is extremely valuable. 

Third, record your design decisions in some way; a simple text file is often useful for tracking your deliberations, the 
alternatives you considered, and the conclusions you reached. That information is invaluable as your implementation 
becomes more complete, and hence more complex, and you are attempting to extend it to incorporate additional features. 

Write useful comments in your code, as you go. Leave notes to yourself about things that still need to be done, or that you 
are currently handling in a clumsy manner. 

A preprocessing phase is helpful; for example, it gives you a chance to filter out comments, trim whitespace, and gather 
various pieces of information. Do not try to do everything in one pass. Compilers and assemblers frequently produce a 
number of intermediate files and/or in-memory structures, recording the results of different phases of execution. 

Enumerated types are extremely useful for representing various kinds of information, especially about type attributes of 
structured variables. For example: 

enum Breed {BEAGLE, CHOW, SHIH_TSU, GOLDEN_RETRIEVER, MIXED}; 


struct _Dog { 


char* Name; 


enum Breed Type; 


unsigned int Weight; 
}; 
typedef struct _Dog Dog; 


Think carefully about what information would be useful when analyzing and translating the assembly code. Much of this is 
actually not part of the source code, but rather part of the specification of the assembly and machine languages. Consider 
using static tables of structures to organize language information; by static, I mean a table that's directly initialized when it's 
declared, has static storage duration, and is private to the file in which it’s created. For example: 

#define TABLE_SZ 20 


static Dog Table[TABLE_SZ] = { 


{"Amber", GOLDEN_RETRIEVER, 110}, // initializes first struct element, 
{"Dammit", BEAGLE, 27}, // etc. 
{"Sassy", MIXED, 30}, 
. . . 
}; 

Version 2.0. 


CS 2506 Computer Organization II C Programming 3: Simple MIPS Assembler 

Obviously, the sample code shown above does not play a role in my solution. On the other hand, I used this approach to 
organize quite a bit of information about instruction formats and encodings. Also, be careful about making assumptions 
about the instruction formats… 

Consult the manual MIPS32 Architecture Volume 2, linked from the Resources page. It has lots of details on machine 
language and assembly instruction formats. I found it invaluable, especially in some cases where an instruction doesn't 
quite fit the simple description of MIPS assembly conventions in the course notes (e.g., slland syscall). 

Feel free to make reasonable assumptions about limits on things like the number of variables, number of labels, number of 
assembly statements, etc. It's not good to guess too low about these things, but making sensible guesses let you avoid 
(some) dynamic allocations. 

Write lots of "utility" functions because they simplify things tremendously; e.g., string trimmers, mappers, etc. 

Data structures play a role because there's a substantial amount of information that must be collected, represented and 
organized. However, I used nothing fancier than arrays. 

Data types, like the structure shown above, play a major role in a good solution. I wrote a significant number of them. 

Explore string.hcarefully. Useful functions include strncpy(), strncmp(), memcpy()and strtok(). There 
are lots of useful functions in the C Standard Library, not just in string.h. One key to becoming proficient and 
productive in C, as in most programming languages, is to take full advantage of the library that comes with that language. 

When testing, you should create some small input files. That makes it easy to isolate the various things your assembler 
must deal with. Note that the assembler is not doing any validation of the logic of the assembly code, so you don’t have to 
worry about producing assembly test code that will actually do anything sensible. For example, you might use a short 
sequence of R-type instructions: 

.text 
add $t0, $t1, $t2 
sub $t3, $t1, $t0 
xor $s7, $t4, $v0 

Version 2.0. 


CS 2506 Computer Organization II 
C Programming 3: Simple MIPS Assembler 

What should I turn in, and how? 

Create a tar file containing: 

• All the .cand .hfiles which are necessary in order to build your assembler. 
• A GNU makefile named “makefile”. The command “make assembler”should build an executable 
  named “assemble”. The makefile may include additional targets as you see fit. 
• A readme.txtfile if there’s anything you want to tell us regarding your implementation. For example, if there 
  are certain things that would cause your assembler to fail (e.g., it doesn’t handle lainstructions), telling us that 
  may result in a more satisfactory evaluation of your assembler. 
• A pledge.txtfile containing the pledge statement from the course website. 
• Nothing else. Do not include object files or an executable. We will compile your source code. 
  Submit this tar file to the Curator, by the deadline specified on the course website. 

Test Environment 

Your assembler will be tested on the rlogin cluster, running 64-bit CentOS and gcc 4.4.4. There are many of you, and few 
of us. Therefore, we will not test your assembler on any other environment. So, be sure that you compile and test it there 
before you submit it. Be warned in particular, if you use OS-X, that the version of gcc available there has been modified 
for Apple-specific reasons, and that past students have encountered significant differences between that version and the one 
running on Linux systems. 

Maximizing Your Results 

Ideally you will produce a fully complete and correct solution. If not, there are some things you can do that will improve 
your score: 

• Make sure your assembler does not crash on any valid input, even if it cannot produce the correct results. If you 
ensure that your assembler processes all the posted test files, it is extremely unlikely it will encounter anything in 
our test data that would cause it to crash. On the other hand, if your assembler does crash on any of the posted test 
files, it will certainly do so during our testing. We will not invest time or effort in diagnosing the cause of such a 
crash during our testing. It’s your responsibility to make sure we don’t encounter such crashes. 

• If there is a MIPS32 instruction or data declaration that your solution cannot handle, document that in the 
readme.txtfile you will include in your submission. 

• If there is a MIPS32 instruction or data declaration that your solution cannot handle, make sure that it still 
produces the correct number of lines of output, since we will automate much of the checking we do. In particular, 
if your assembler encounters a MIPS32 instruction it cannot handle, write a sequence of 32 asterisk characters 
(‘*’) in place of the correct machine representation (or multiple lines for some pseudo-instructions). Doing this 
will not give you credit for correctly translating that instruction, but this will make it more likely that we correctly 
evaluate the following parts of your translation. 
Credits 

The original formulation of this project was by Dr Srinidhi Vadarajan, who was then a member of the Dept of Computer 
Science at Virginia Tech. His sources of inspiration for this project are lost in the mists of time. 

The current modification was produced by William D McQuain, as a member of the Dept of Computer Science at Virginia 
Tech. Any errors, ambiguities and omissions should be attributed to him. 

Version 2.0. 


