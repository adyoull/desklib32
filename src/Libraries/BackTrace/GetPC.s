	.global	BackTrace_GetPC

BackTrace_GetPC:
	MOV	a1, lr
	MOV	pc, lr

