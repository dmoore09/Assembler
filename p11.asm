# p10.asm
#
#  many strings in data segment
#  integers in data segment
#
.text
main:
        la   $a0, S1
        li   $v0,  4
        syscall
        
        la   $a0, S2
        li   $v0,  4
        syscall
        
        la   $a0, S3
        li   $v0,  4
        syscall
        
        la   $a0, S4
        li   $v0,  4
        syscall
        
        la   $a0, S5
        li   $v0,  4
        syscall
        
        li   $v0, 10
        syscall

.data
S1:   .asciiz   "a"
S2:   .asciiz   "b"
I1:   .word     1
S3:   .asciiz   "c"
I2:   .word     2
S4:   .asciiz   "def"
I3:   .word     3
S5:   .asciiz   "ghijk"
I4:   .word     4
