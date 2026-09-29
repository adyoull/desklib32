@
@       Title                  : Wimp Load Template.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Load a template from a file.
@
@
@       Modification history.
@
@       Version                : 1.1 (Reflect in header IDENT)
@       Date                   : 30th November, 1991
@       Author                 : John H. Winters
@       Changes                : Can't use R0 as a temporary pointer since
@                                the SWI corrupts it (contrary to Acorn's
@                                documentation).  Use R7 instead.
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
        
        .global Wimp_LoadTemplate
Wimp_LoadTemplate:
@
        STMFD   sp!, {v1, v2, v3, v4, lr}
        LDMIA   a1!, {a2, a3, a4, v1, v2, v3}
        MOV     v4, a1
        SWI     SWI_Wimp_LoadTemplate + XOS_Bit
        STMDB   v4!, {a2, a3, a4, v1, v2, v3}
        MOVVC   a1, #0
        LDMFD   sp!, {v1, v2, v3, v4, pc}
@
