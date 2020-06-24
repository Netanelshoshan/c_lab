;asfafa
;asfasf
.entry LIST
.extern W

MAIN:	add  ass	r3,LIST
LOOP:	prn		#48
lea		W,r6

;invalid instruction
incremento		r6 

;invalid instruction
move		r3,K

;invalid instruction
subliminal		r1,r4
bne		END
cmp		K,#-6

;invalid commands
.external ext
.ent ent
.stringi "g"

bne		END
dec		W
.entry MAIN
jmp		&LOOP

;wrong addressing method
add L3, &LOOP 

add		L3,L3
;invalid instruction
END:	stupid
STR:	.string "abcd"
LIST:	.data	6,-9
.data	-100
K:		.data	31
.extern L3
