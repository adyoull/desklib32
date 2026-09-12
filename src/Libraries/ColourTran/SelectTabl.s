;-------------------------------------------------------------------------------
; ColourTrans_SelectTable(int src_mode, int *src_pal,
;                         int dst_mode, int *dst_pal,
;                         char *table);
; Copyright 1993 Shaun Blackmore


        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE ColourTrans_SelectTable
;
        MOV     ip,sp
        STMFD   sp!,{r4,lr}
        LDR     r4,[ip,#0]
        SWI     SWI_ColourTrans_SelectTable + XOS_Bit
        MOVVC   r0, #0
        LDMFD   sp!,{r4,pc}
;
        END
