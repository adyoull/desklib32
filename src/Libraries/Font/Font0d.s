; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;os_error *Font_FindCaret(FontString *string);

;       r0=pointer to definition block

;       [r0,#0]  = char *s string
;       [r0,#4]  = x
;       [r0,#8]  = y
;       [r0,#12] = number of printable chars
;       [r0,#16] = index

        PREAMBLE
        STARTCODE Font_FindCaret
;
        STMFD   sp!, {r4,r5,r6,lr}
        MOV     r6,r0
        LDMIA   r6,{r1,r2,r3,r4,r5}
        SWI     SWI_Font_FindCaret + XOS_Bit
        STMIA   r6,{r1,r2,r3,r4,r5}
        MOVVC   r0,#0
        LDMFD   sp!, {r4,r5,r6,pc}
;
        END
