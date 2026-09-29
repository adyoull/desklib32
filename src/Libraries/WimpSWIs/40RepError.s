@
@       Title                  : Wimp Report Error.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Reports an error.
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
        
        .global Wimp_ReportError
Wimp_ReportError:
@
@        STMFD   sp!, {lr}
        MOV     ip, lr

        SWI     SWI_Wimp_ReportError + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip
@        LDMFD   sp!, {pc}
@
