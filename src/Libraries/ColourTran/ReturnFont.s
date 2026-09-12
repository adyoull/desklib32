;-------------------------------------------------------------------------------
; ColourTrans_ReturnFontColours(ct_fontblock);
; Copyright 1993 Shaun Blackmore

        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE ColourTrans_ReturnFontColours
;
        MOV     ip,sp
        STMFD   sp!,{r4,lr}
        MOV     r4,r0
        LDR     r0,[r4,#0]
        LDR     r1,[r4,#4]
        LDR     r2,[r4,#8]
        LDR     r3,[r4,#12]
        SWI     SWI_ColourTrans_ReturnFontColours + XOS_Bit
        STR     r1,[r4,#4]
        STR     r2,[r4,#8]
        STR     r3,[r4,#12]
        MOVVC   r0, #0
        LDMFD   sp!,{r4,pc}
;
        END
