# p3.asm
#
#  Basic R-type and I-type instructions only.
#  No pseudos.
#  Simple data segment.
#
.text

main:
        addi  $t0, $zero, 5000
        addi  $t1, $zero, 10000
        add   $t2, $t0, $t1

        addi  $v0, $zero, 10
        syscall

.data
A:       .word   987654321
