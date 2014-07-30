# p6.asm
#
# Mass test of basic instructions and data segment options
#
.text
main:
        # basic load/store instructions:
        lw    $a1, 0($a0)
        sw    $a2, -12($a0)

        # basic arithmetic/logical instructions:
        add   $a3, $t0, $t1
        sub   $t2, $t3, $t4
        or    $t5, $t6, $t7
        and   $t8, $t9, $t9
        slt   $t0, $t1, $t2
        sll   $t3, $t4, 31
        srl   $t6, $t7, 5
        sra   $v0, $v1, 17
        nop

        # basic immediate instructions:
        addi  $s0, $s1, -32768
        addiu $s2, $s3, 32767
        andi  $s4, $s5, 1
        ori   $s6, $s7, 1023
        slti  $t0, $t1, 4095

        # basic conditional branch instructions
        beq   $s5, $s7, L0
        bne   $t3, $t8, L0
        blez  $t0, L0
        bltz  $t1, L0

        # basic unconditional jump instructions:
        j     L0
        jr    $ra
        jal   L0

L0:
        addi  $v0, $zero, 10
        syscall
.data
One:     .asciiz "MIPS is good for you"
Two:     .word   10
Three:   .word   6:8
Four:    .word   511
