//header file that holds declarations of all static data like table of opcodes, structures for different instruction types

//table to hold all register numbers
typedef struct registertable {
int zero[5];
int v0[5];
int v1[5];
int a0[5];
int a1[5];
int a2[5];
int a3[5];
int t0[5];
int t1[5];
int t2[5];
int t3[5];
int t4[5];
int t5[5];
int t6[5];
int t7[5];
int s0[5];
int s1[5];
int s2[5];
int s3[5];
int s4[5];
int s5[5];
int s6[5];
int s7[5];
int t8[5];
int t9[5];
int gp[5];
int sp[5];
int fp[5];
int ra[5];
int at[5];
} RegistersT;

//table of all supported opcode/funct bits
typedef struct opCode {
int lw[6];
int sw[6];
int la[6];
int li[6];
int add[6];
int sub[6];
int addi[6];
int addiu[6];
int orin[6];
int andin[6];
int ori[6];
int andi[6];
int slt[6];
int slti[6];
int sll[6];
int srl[6];
int sra[6];
int nop[6];
int beq[6];
int bne[6];
int blez[6];
int j[6];
int jr[6];
int jal[6];
int bltz[6];
} opCode;

//struc to represent rtype instructions
typedef struct rType{
        int rs[5];
        int rt[5];
        int rd[5];
        int shamt[5];
        int funct[6];
} rType;

//struc to represent ltype instructions        
typedef struct lType{
        int op[6];
        int rs[5];
        int rt[5];
        int Imm[16];
} lType;

//struc to represent jtype instructions        
typedef struct jType{
        int op[6];
	int Imm[26];
} jType;
