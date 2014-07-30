#  p9.asm
#
#  lots of text segment labels
.text
main:
        la    $t0, X
        lw    $t0, 0($t0)
        la    $t1, Y
        lw    $t1, 0($t1)
L0:       
        beq   $t0, $t1, L2
L1:
        addi  $t1, $t1, 1
        bne   $t0, $t1, L3
L2:
        addi  $t0, $t0, 2
        bne   $t0, $t1, L1
L3:
        addi  $t1, $t1, 4
        bne   $t1, $t1, L2
L4:
        addi  $v0, $zero, 10
        syscall
.data
X:   .word 10
Y:   .word 20
