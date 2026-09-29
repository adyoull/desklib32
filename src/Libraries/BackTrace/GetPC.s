	EXPORT	BackTrace_GetPC

	AREA	|C$$code|, CODE, READONLY
BackTrace_GetPC
	MOV	a1, lr
	MOV	pc, lr

	END
