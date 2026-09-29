@
@       Title                  : Wimp Block Copy.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Copies a block.
@
@
@       Modification history.
@
@       Version                : (Reflect in header IDENT)
@       Date                   :
@       Author                 :
@       Changes                :
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
        
        .global Wimp_BlockCopy
Wimp_BlockCopy:
@
        STMFD   sp!, {v1, v2, v3, lr}
        MOV     v2,  a3
        MOV     v3,  a4
        LDMFD   a2,  {a2, a3, a4, v1}
        SWI     SWI_Wimp_BlockCopy + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1, v2, v3, pc}
@
