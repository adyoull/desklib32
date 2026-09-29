@-------------------------------------------------------------------------------
@ GFX_Wait (void)
@ by Jason Williams 1993
@
        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global GFX_Wait
GFX_Wait:
@
        MOV     a1, #19
        SWI     XSWI_OS_Byte
        MOV     pc, lr
@
