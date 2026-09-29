@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_SetScaleFactor(int xscale, int yscale);

        
        .global Font_SetScaleFactor
Font_SetScaleFactor:
@
        STMFD   sp!, {lr}
        MOV     r2,r1
        MOV     r1,r0
        SWI     SWI_Font_SetScaleFactor + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
@
