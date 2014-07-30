# p1.asm
#
# Some basic, mostly R-type, instructions.  Simple data segment.  Loop.

.text
main:
        la   $s0, A              # load variables A and B into registers
        lw   $s0, 0($s0)
        la   $s1, B
        lw   $s1, 0($s1)

        add  $t0, $zero, $zero   # set counter to 0
do:
        addi $t0, $t0, 1         # count iterations
        addi $s1, $s1, 2         # increase $s1
        sub  $s2, $s1, $s0       # take difference
        bltz $s2, do             # continue if $s1 is smaller

        la   $t1, Count
        sw   $t0, 0($t1)         # save counter to memory

exit:
        li   $v0, 10             # system call to terminate program
        syscall                  # make call

.data
A:      .word   32
B:      .word   16
Count:  .word    0

