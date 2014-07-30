# p10.asm
#
#  procedure call, full-bore testing
#  array in data segment
#
.text
main:
       # initializes array List[]
       la    $t0, List              # $t0 points to current elem in List
       li    $s0, 42                # set array values manually
       sw    $s0, 0($t0)
       addi  $t0, $t0, 4
       li    $s0, 102
       sw    $s0, 0($t0)
       addi  $t0, $t0, 4
       li    $s0, 0
       sw    $s0, 0($t0)
       addi  $t0, $t0, 4
       li    $s0, -56
       sw    $s0, 0($t0)
       addi  $t0, $t0, 4
       li    $s0, 47
       sw    $s0, 0($t0)
       addi  $t0, $t0, 4
       li    $s0, 29876
       sw    $s0, 0($t0)
       addi  $t0, $t0, 4
       li    $s0, 1024
       sw    $s0, 0($t0)
       addi  $t0, $t0, 4
       li    $s0, -20001
       sw    $s0, 0($t0)

       la    $a0, List
       la    $a1, N
       lw    $a1, 0($a1)
       jal   bubble_sort

       addi  $v0, $zero, 10         # termination syscall
       syscall

########################################################### bubble_sort
bubble_sort:
       addi  $t3, $a1, -1             # initialize outer loop limit

outer:                                  # outer bubble-sort loop
       slt $at, $t3, $zero
       bne $at, $zero, outer_end
       li    $t0, 0                   # initialize inner loop counter
       add   $t2, $a0, $zero      # put address of first elem in temp register
inner:                                  # inner bubble-sort loop
       slt   $at, $t3, $t0
       bne   $at, $zero, inner_end

       lw    $t7, 0($t2)              # get value at current index
       lw    $t8, 4($t2)              # get value at "next" index

       ble   $t7, $t8, no_swap
       slt  $at, $t8, $t7
       beq  $at, $zero, no_swap

       sw    $t8, 0($t2)
       sw    $t7, 4($t2)
no_swap:   
       addi  $t0, $t0, 1              # increment inner loop counter
       addi  $t2, $t2, 4
       j     inner
inner_end:
       addi  $t3, $t3, -1             # decrement outer loop limit
       j     outer
outer_end:
       jr    $ra

.data
List: .word   56, 43, 33, 20, 59, 73, 9, 41
N:    .word   8
