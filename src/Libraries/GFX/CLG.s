@-------------------------------------------------------------------------------
@ GFX_CLG(void)
@ by Jason Williams 1993
@
        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global GFX_CLG
GFX_CLG:
@
        SWI     XSWI_OS_WriteI + 16
        MOV     pc, lr
@
