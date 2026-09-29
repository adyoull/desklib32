;-------------------------------------------------------------------------------
; OS_WriteN
;
        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE GFX_WriteN
;
        SWI     XSWI_OS_WriteN
        MOV     pc, lr
;
        END
