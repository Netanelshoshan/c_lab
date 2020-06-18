.entry K
STR:    .string  "abcdef"
MAIN:    mov   w, r1
lea w, r4
LOOP:    jmp  &END

;here I cant even reach "nsh" because of knowing the numofops.
;so the prog won't advance the pointer, therefor - No errorno.

prn w  

;instruction error
subliminal #1, r1

;unrecogized command
.externalio t

.extern w
inc   r0

;unrecogized command
invalidInst r0

LENGTH:  .data  6
mov  r3,STR

;unrecogized command
blabla
bne LOOP

;unrecogized command
paamAharona

END:    stop
.entry LENGTH
K:    .data  2
