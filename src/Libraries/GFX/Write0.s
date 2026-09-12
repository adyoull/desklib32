;-------------------------------------------------------------------------------
; OS_Write0
;
        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE GFX_Write0
;
        SWI     XSWI_OS_Write0
        MOV     pc, lr
;
        END
