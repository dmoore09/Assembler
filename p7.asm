# p7.asm
#
#  simple program, R-type, I-type, basic pseudos
.text

main:
        la    $s1, N
        lw    $s1, 0($s1)

        addi  $t0, $zero, 1
        addi  $t1, $zero, 1
        addi  $s0, $zero, 2
loop:
        add   $t2, $t0, $t1
        add   $t0, $t1, $zero
        add   $t1, $t2, $zero

        addi  $s0, $s0, 1
        bne   $s0, $s1, loop

        la    $s1, fiboN
        sw    $t2, 0($s1)
        addi  $v0, $zero, 10
        syscall

.data
fiboN:   .word   0
N:       .word   10
