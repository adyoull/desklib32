@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_FindCaretJ(FontString *s, int xoffset, int yoffset);


        
        .global Font_FindCaretJ
Font_FindCaretJ:
@
        STMFD   sp!, {r4,r5,r6,lr}
        MOV     r6,r0
        MOV     r4,r1
        MOV     r5,r2
        LDMIA   r6,{r1,r2,r3}
        SWI     SWI_Font_FindCaretJ + XOS_Bit
        STMIA   r6,{r1,r2,r3,r4,r5}
        MOVVC   r0,#0
        LDMFD   sp!, {r4,r5,r6,pc}
