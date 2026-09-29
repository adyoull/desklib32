@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_ReadScaleFactor(int *xscale, int *yscale);

        
        .global Font_ReadScaleFactor
Font_ReadScaleFactor:
@
        STMFD   sp!, {r4,lr}
        MOV     r3,r0
        MOV     r4,r1
        SWI     SWI_Font_ReadScaleFactor + XOS_Bit
        STR     r1,[r3,#0]
        STR     r2,[r4,#0]
        MOVVC   r0,#0
        LDMFD   sp!, {r4,pc}
@
