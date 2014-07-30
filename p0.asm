.text
main:
        la   $s0, A              # load variables A and B into registers
        lw   $s0, 0($s0)
        la   $s1, B
        lw   $s1, 0($s1)

        add  $s2, $s0, $s1       # calculate sum
        la   $s3, Sum            # assign result to Sum
        sw   $s2, 0($s3)

        sub  $s2, $s0, $s1       # calculate difference
        la   $s3, Diff           # assign result to Diff
        sw   $s2, 0($s3)
exit:
        li   $v0, 10             # system call to terminate program
        syscall                  # make call

.data
A:      .word   32
B:      .word   16
Sum:    .word    0
Diff:   .word    0

