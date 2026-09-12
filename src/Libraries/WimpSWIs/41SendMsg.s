;
;       Title                  : Wimp Send message
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Sends a Wimp message.
;
;
;       Modification history.
;
;       Version                : (Reflect in header IDENT)
;       Date                   :
;       Author                 :
;       Changes                :
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
        STARTCODE Wimp_SendMessage
;
;        STMFD   sp!, {lr}
        MOV     ip, lr

        SWI     SWI_Wimp_SendMessage + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip
;        LDMFD   sp!, {pc}
;
        END
