@-------------------------------------------------------------------------------
@ ColourTrans_SetFontColours
@ Copyright 2003 Peter Naulls


        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global ColourTrans_SetFontColours
ColourTrans_SetFontColours:
@
        SWI     SWI_ColourTrans_SetFontColours + XOS_Bit
        MOVVC   r0, #0
        MOV     pc, lr
@
