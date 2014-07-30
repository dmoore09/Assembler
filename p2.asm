# p2.asm
#
# Some basic, mostly R-type, instructions.  
# More complex data segment.  
# Loop.

.text
main:
        la    $s0, Sz             # get dimension of array
        lw    $s0, 0($s0)
        blez  $s0, exit           # exit if no array

        la    $s1, List           # get pointer to array

        sll   $s0, $s0, 2         # calculate size of array
        add   $s2, $s0, $s1       # calculate address past array
        addi  $s2, $s2, -4        # calculate address of last cell
do:
        sw    $s1, 0($s1)         # write address of cell into cell
        addi  $s1, $s1, 4         # step pointer to next cell
        ble   $s1, $s2, do        # continue through last cell

exit:
        li    $v0, 10             # system call to terminate program
        syscall                   # make call

.data
List:   .word    -1:16
Sz:     .word    16

