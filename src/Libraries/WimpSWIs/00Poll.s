@
@       Title                  : Wimp Poll
@       System                 : Wimp Library
@       Version                : 1.10
@       Copyright              : (C) John Winters
@       Date                   : 22nd September, 1991
@       Author                 : John H. Winters
@
@       Modifications          : 1.10 Jason Williams
@                                Added support for passing r3 (non-0 pollword)
@                                (new feature of the RISC OS 3 WIMP)
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
        

        .global Wimp_Poll
Wimp_Poll:
        MOV     a3, #0

        .global Wimp_Poll3
Wimp_Poll3:

        ORR     a1, a1, #1<<24 @ Always save FP
        ADD     a2, a2, #4
        MOV     a4, a3
        SWI     SWI_Wimp_Poll + XOS_Bit
        STRVC   a1, [a2, #-4]
        MOVVC   a1, #0
        MOV     pc, lr

        .global Wimp_PollIdle
Wimp_PollIdle:
        MOV     a4, #0

        .global Wimp_PollIdle3
Wimp_PollIdle3:
        ORR     a1, a1, #1<<24 @ Always save FP
        ADD     a2, a2, #4
        SWI     SWI_Wimp_PollIdle + XOS_Bit
        STRVC   a1, [a2, #-4]
        MOVVC   a1, #0
        MOV     pc, lr

