.entry K
STR:    .string  "abcdef"
MAIN:    mov   w, r1
lea w, r4
LOOP:    jmp  &END

;here I cant even reach "nsh" because of knowing the numofops.
;so the prog won't advance the pointer, therefor -NoProblemo.

prn w

;instruction error
subliminal #1, r1

;unrecogized command
.externalio t

.extern w
inc   r0

;unrecogized instruction
sysProgLab r0

LENGTH:  .data  6
mov  r3,STR

;unrecogized instruction
blabla

bne LOOP

;unrecogized instruction
paamAharona

END:    stop

;unrecogized command
.entry LENGTH

.extenalii sss

K:    .data  2,2,222,2
