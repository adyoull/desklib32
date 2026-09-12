		EXPORT	BackTrace_PCMask

		AREA	|C$$code|, CODE, READONLY
BackTrace_PCMask
		CMP	a3, a3
		RSBS	a2, pc, pc		; Z=1 if 32-bit mode, else Z=0
		MOVEQ	a1, #&FFFFFFFF		; 32-bit: set the mask value
		MOVNE	a1, #~&FC000003		; 26-bit: set the mask value
		MOV	pc, lr

		END
