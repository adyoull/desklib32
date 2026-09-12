;-------------------------------------------------------------------------------
; GFX_SetColour(flags, colour)
; by Peter Naulls 2004
;
        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE GFX_SetColour
;
        SWI     XSWI_OS_SetColour
        MOV     pc, lr
;
        END
