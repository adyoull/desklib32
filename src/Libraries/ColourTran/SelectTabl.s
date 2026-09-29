@-------------------------------------------------------------------------------
@ ColourTrans_SelectTable(int src_mode, int *src_pal,
@                         int dst_mode, int *dst_pal,
@                         char *table);
@ Copyright 1993 Shaun Blackmore


        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global ColourTrans_SelectTable
ColourTrans_SelectTable:
@
        MOV     ip,sp
        STMFD   sp!,{r4,lr}
        LDR     r4,[ip,#0]
        SWI     SWI_ColourTrans_SelectTable + XOS_Bit
        MOVVC   r0, #0
        LDMFD   sp!,{r4,pc}
@
