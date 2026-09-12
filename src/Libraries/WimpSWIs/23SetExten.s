;
;       Title                  : Wimp Set Extent
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Set the extent of a work area.
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
        STARTCODE Wimp_SetExtent
;
;        STMFD   sp!, {lr}
        MOV     ip, lr

        SWI     SWI_Wimp_SetExtent + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip
;        LDMFD   sp!, {pc}
;
        END
