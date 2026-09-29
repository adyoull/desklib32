@-------------------------------------------------------------------------------
@ Get graphics clip rectangle 
@
        .include     "Macros.h"
        .include     "RegDefs.h"
        .include     "SwiNos.h"
@
        
        .global GFX_SetClip
GFX_SetClip:
@
        MOV     a2, a1

        MOV     a1, #24
        SWI     XSWI_OS_WriteC

        LDRB    a1, [a2, #0]
        SWI     XSWI_OS_WriteC
        LDRB    a1, [a2, #1]
        SWI     XSWI_OS_WriteC
        LDRB    a1, [a2, #4]
        SWI     XSWI_OS_WriteC
        LDRB    a1, [a2, #5]
        SWI     XSWI_OS_WriteC
        LDRB    a1, [a2, #8]
        SWI     XSWI_OS_WriteC
        LDRB    a1, [a2, #9]
        SWI     XSWI_OS_WriteC
        LDRB    a1, [a2, #12]
        SWI     XSWI_OS_WriteC
        LDRB    a1, [a2, #13]
        SWI     XSWI_OS_WriteC

        MOV     pc, lr
@
