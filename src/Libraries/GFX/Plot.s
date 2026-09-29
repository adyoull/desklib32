@-------------------------------------------------------------------------------
@ GFX_Plot (int plotcode, int x, int y)
@ by Jason Williams 1993
@
        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global GFX_Plot
GFX_Plot:
@
        SWI     XSWI_OS_Plot
        MOV     pc, lr
@
