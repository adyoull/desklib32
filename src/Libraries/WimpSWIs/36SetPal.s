@
@       Title                  : Wimp Set Palette
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Sets the palette.
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
        
        .global Wimp_SetPalette
Wimp_SetPalette:
@
@        STMFD   sp!, {lr}
        MOV     ip, lr

        MOV     a2, a1
        SWI     SWI_Wimp_SetPalette + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip
@        LDMFD   sp!, {pc}
@
