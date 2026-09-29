@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_ReadThresholds(FontThresholds *t);


        
        .global Font_ReadThresholds
Font_ReadThresholds:
@
        STMFD   sp!, {lr}
        MOV     r1,r0
        SWI     SWI_Font_ReadThresholds + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
