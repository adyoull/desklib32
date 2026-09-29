@
@       Title                  : Wimp Read Palette
@       System                 : Wimp Library
@       Version                : 1.2
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Reads the palette.
@
@
@       Modification history.
@
@       Version                : 1.1 (Reflect in header IDENT)
@       Date                   : 24th October, 1991
@       Author                 : John H. Winters
@       Changes                : Corrected to return.
@
@       Version                : 1.2 (Reflect in header IDENT)
@       Date                   : 25th January, 1992
@       Author                 : John H. Winters
@       Changes                : Correction corrected.
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
        
        .global Wimp_ReadPalette
Wimp_ReadPalette:
@
@        STMFD   sp!, {lr}
        MOV     ip, lr

        MOV     a2, a1
        SWI     SWI_Wimp_ReadPalette + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip
@        LDMFD   sp!, {pc}
@
