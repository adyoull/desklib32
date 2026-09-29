	.global	BackTrace_GetSL

BackTrace_GetSL:
	MOV	a1, sl
	MOV	pc, lr @ 32 bit

