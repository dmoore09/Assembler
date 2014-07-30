#  p4.asm
#
#  More complex control, R-type, I-type.
#  Some pseudos.
#  Array in data segment.
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

       # counts elements in List[] that exceed Ceil
       la    $s7, N                 # $s7 == array dimension
       lw    $s7, 0($s7)
       la    $t0, List              # $t0 points to current elem in List
       la    $s6, Ceil              # $s6 == ceiling value
       lw    $s6, 0($s6)

       li    $t1, 0                 # loop counter
       li    $t2, 0                 # number of values > ceiling

L0:    
       lw    $s0, 0($t0)            # load current array element

       ble   $s0, $s6, skip         # skip increment if not > ceiling
       addi  $t2, $t2, 1            # count if > ceiling
skip:

       addi  $t0, $t0, 4            # step to next array element
       addi  $t1, $t1, 1            # count this iteration
       blt   $t1, $s7, L0           # restart loop if necessary

       addi  $v0, $zero, 10         # termination syscall
       syscall

.data
N:    .word   8
List: .word   0:8
Ceil: .word   100
