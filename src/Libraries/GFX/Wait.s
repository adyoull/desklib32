;-------------------------------------------------------------------------------
; GFX_Wait (void)
; by Jason Williams 1993
;
        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE GFX_Wait
;
        MOV     a1, #19
        SWI     XSWI_OS_Byte
        MOV     pc, lr
;
        END
