; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;os_error *Font_SetThresholds(FontThresholds *t);


        PREAMBLE
        STARTCODE Font_SetThresholds
;
        STMFD   sp!, {lr}
        MOV     r1,r0
        SWI     SWI_Font_SetThresholds + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
        END
