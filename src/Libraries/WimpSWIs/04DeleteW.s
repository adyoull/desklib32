@
@       Title                  : Wimp Delete Wind
@       System                 : Wimp Library
@       Version                : 1.01
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Deletes a window.
@
@
@       Modification history.
@
@       Date                   : 03 Apr 1993
@       Author                 : Shaun Blackmore
@
@
@============================================================================
@
@  Include files.
@
@============================================================================
@
        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"
@
@============================================================================
@
@  Code.
@
@============================================================================
@
        
        .global Wimp_DeleteWindow
Wimp_DeleteWindow:
@
        STMFD   sp!, {a1, lr}
        MOV     a2, sp
        SWI     SWI_Wimp_DeleteWindow + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {a2, pc}
@
