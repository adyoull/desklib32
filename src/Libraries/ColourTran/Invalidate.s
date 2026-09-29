@-------------------------------------------------------------------------------
@ ColourTrans_InvalidateCache(void);
@ Copyright 1993 Shaun Blackmore

        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global ColourTrans_InvalidateCache
ColourTrans_InvalidateCache:
@
        STMFD   sp!,{lr}
        SWI     SWI_ColourTrans_InvalidateCache + XOS_Bit
        MOVVC   r0, #0
        LDMFD   sp!,{pc}
@
