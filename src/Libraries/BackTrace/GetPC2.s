		EXPORT	Desk_BackTrace_GetPC2
		EXPORT	BackTrace_GetPC2	; name declared in BackTrace.h

		AREA	|C$$code|, CODE, READONLY
Desk_BackTrace_GetPC2
BackTrace_GetPC2
		RSBS	a2, pc, pc		; Z=1 if 32-bit mode, else Z=0
		MOVEQ	a1, lr
		BICNE	a1, lr, #&FC000003	; 26-bit: just get the PC
		MOV	pc, lr

		END
