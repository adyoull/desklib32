;
;       Title                  : Wimp Poll
;       System                 : Wimp Library
;       Version                : 1.10
;       Copyright              : (C) John Winters
;       Date                   : 22nd September, 1991
;       Author                 : John H. Winters
;
;       Modifications          : 1.10 Jason Williams
;                                Added support for passing r3 (non-0 pollword)
;                                (new feature of the RISC OS 3 WIMP)
;
;
;============================================================================
;
;  Include files.
;
;============================================================================
;
        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h
;
;============================================================================
;
;  Code.
;
;============================================================================
;
        PREAMBLE

        STARTCODE Wimp_Poll
        MOV     a3, #0

        STARTCODE Wimp_Poll3

        ORR     a1, a1, #1<<24 ; Always save FP
        ADD     a2, a2, #4
        MOV     a4, a3
        SWI     SWI_Wimp_Poll + XOS_Bit
        STRVC   a1, [a2, #-4]
        MOVVC   a1, #0
        MOV     pc, lr

        STARTCODE Wimp_PollIdle
        MOV     a4, #0

        STARTCODE Wimp_PollIdle3
        ORR     a1, a1, #1<<24 ; Always save FP
        ADD     a2, a2, #4
        SWI     SWI_Wimp_PollIdle + XOS_Bit
        STRVC   a1, [a2, #-4]
        MOVVC   a1, #0
        MOV     pc, lr

    END
