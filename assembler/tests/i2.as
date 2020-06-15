.entry K
STR:    .string  "abcdef"
MAIN:    mov   w, r1
lea w, r4
LOOP:    jmp  &END
prn w
sub #1, r1
.extern w
inc   r0
LENGTH:  .data  6
mov  r3,STR
bne LOOP
END:    stop
.entry LENGTH
K:    .data  2
