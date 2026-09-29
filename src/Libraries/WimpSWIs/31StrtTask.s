@
@       Title                  : Wimp Start Task
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Starts a task.
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
        
        .global Wimp_StartTask
Wimp_StartTask:
@
@        STMFD   sp!, {lr}

        MOV     ip, lr
        SWI     SWI_Wimp_StartTask + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip
@        LDMFD   sp!, {pc}
@
