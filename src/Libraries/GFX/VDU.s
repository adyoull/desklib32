@-------------------------------------------------------------------------------
@ OS_WriteC - VDU(ch)
@
        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global GFX_VDU
GFX_VDU:
@
        SWI     XSWI_OS_WriteC
        MOV     pc, lr
@
