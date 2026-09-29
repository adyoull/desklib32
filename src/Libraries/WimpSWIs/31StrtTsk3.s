@
@       Title                  : Wimp Start Task (RISC OS 3)
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) Sergio Monesi
@       Date                   : 21 Jan 1995
@       Author                 : Sergio Monesi
@
@       Function               : Starts a task and return his task handle.
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
        
        .global Wimp_StartTask3
Wimp_StartTask3:
@
        MOV     ip, lr
        SWI     SWI_Wimp_StartTask + XOS_Bit
        MOVVS   pc, ip
        TEQ     a2, #0
        STRNE   a1, [a2]
        MOV     a1, #0
        MOV     pc, ip
@
