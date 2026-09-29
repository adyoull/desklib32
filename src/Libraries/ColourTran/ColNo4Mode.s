@-------------------------------------------------------------------------------
@ int ColourTrans_ReturnColourNumberForMode(int pal_entry, int mode, int pal);
@ Copyright 1993 Jason Howat


        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global ColourTrans_ReturnColourNumberForMode
ColourTrans_ReturnColourNumberForMode:
@
        STMFD   sp!,{lr}
        SWI     SWI_ColourTrans_ReturnColourNumberForMode + XOS_Bit
        MOVVS   r0, #-1
        LDMFD   sp!,{pc}
@
