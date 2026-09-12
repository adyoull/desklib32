;-------------------------------------------------------------------------------
; GFX_CLG(void)
; by Jason Williams 1993
;
        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE GFX_CLG
;
        SWI     XSWI_OS_WriteI + 16
        MOV     pc, lr
;
        END
