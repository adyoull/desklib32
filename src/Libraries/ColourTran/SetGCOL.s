@-------------------------------------------------------------------------------
@ ColourTrans_SetGCOL(int r0, int r1, int r2);
@ Copyright 1993 Shaun Blackmore


        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global ColourTrans_SetGCOL
ColourTrans_SetGCOL:
@
        STMFD   sp!,{r4,lr}
        MOV     r3,r1
        MOV     r4,r2
        SWI     SWI_ColourTrans_SetGCOL + XOS_Bit
        MOVVC   r0, #0
        LDMFD   sp!,{r4,pc}
@
