	EXPORT	BackTrace_GetSL

	AREA	|C$$code|, CODE, READONLY
BackTrace_GetSL
	MOV	a1, sl
	MOV	pc, lr ; 32 bit

	END
