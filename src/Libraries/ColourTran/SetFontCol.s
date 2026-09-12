;-------------------------------------------------------------------------------
; ColourTrans_SetFontColours
; Copyright 2003 Peter Naulls


        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE ColourTrans_SetFontColours
;
        SWI     SWI_ColourTrans_SetFontColours + XOS_Bit
        MOVVC   r0, #0
        MOV     pc, lr
;
        END
