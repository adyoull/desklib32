;-------------------------------------------------------------------------------
; ColourTrans_SetGCOL(int r0, int r1, int r2);
; Copyright 1993 Shaun Blackmore


        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE ColourTrans_SetGCOL
;
        STMFD   sp!,{r4,lr}
        MOV     r3,r1
        MOV     r4,r2
        SWI     SWI_ColourTrans_SetGCOL + XOS_Bit
        MOVVC   r0, #0
        LDMFD   sp!,{r4,pc}
;
        END
