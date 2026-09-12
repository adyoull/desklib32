;-------------------------------------------------------------------------------
; OS_WriteC - VDU(ch)
;
        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE GFX_VDU
;
        SWI     XSWI_OS_WriteC
        MOV     pc, lr
;
        END
