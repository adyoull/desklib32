;
;       Title                  : Wimp Command Window
;       System                 : Wimp Library
;       Version                : 1.01
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Opens a command window.
;
;
;       Modification history.
;       Date                   :  03 Apr 1993
;       Author                 :  Shaun Blackmore
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
        STARTCODE Wimp_CommandWindow
;
        MOV     ip, lr

        SWI     SWI_Wimp_CommandWindow + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip
;
        END
