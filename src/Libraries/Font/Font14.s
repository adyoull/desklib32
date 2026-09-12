; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;os_error *Font_ReadThresholds(FontThresholds *t);


        PREAMBLE
        STARTCODE Font_ReadThresholds
;
        STMFD   sp!, {lr}
        MOV     r1,r0
        SWI     SWI_Font_ReadThresholds + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
        END
