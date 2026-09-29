@
@       Title                  : Wimp Extend
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) Peter Naulls
@       Date                   : 21st November 2002
@       Author                 : Peter Naulls
@
@       Function               : Enumerate windows
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
        
        .global Wimp_Enumerate
Wimp_Enumerate:
@
        MOV     a3, a2
        LDR     a2, [a3]

        SWI     SWI_Wimp_Extend + XOS_Bit
        MOVVC   a1, #0
        STRVC   a2, [a3]

        MOV     pc, lr
@
