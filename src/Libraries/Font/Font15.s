@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_SetThresholds(FontThresholds *t);


        
        .global Font_SetThresholds
Font_SetThresholds:
@
        STMFD   sp!, {lr}
        MOV     r1,r0
        SWI     SWI_Font_SetThresholds + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
