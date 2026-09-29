@-------------------------------------------------------------------------------
@ GFX_SetColour(flags, colour)
@ by Peter Naulls 2004
@
        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global GFX_SetColour
GFX_SetColour:
@
        SWI     XSWI_OS_SetColour
        MOV     pc, lr
@
