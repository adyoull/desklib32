;-------------------------------------------------------------------------------
; GFX_Plot (int plotcode, int x, int y)
; by Jason Williams 1993
;
        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE GFX_Plot
;
        SWI     XSWI_OS_Plot
        MOV     pc, lr
;
        END
