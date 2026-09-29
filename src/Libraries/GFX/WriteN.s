@-------------------------------------------------------------------------------
@ OS_WriteN
@
        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global GFX_WriteN
GFX_WriteN:
@
        SWI     XSWI_OS_WriteN
        MOV     pc, lr
@
