; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;os_error *Font_SetScaleFactor(int xscale, int yscale);

        PREAMBLE
        STARTCODE Font_SetScaleFactor
;
        STMFD   sp!, {lr}
        MOV     r2,r1
        MOV     r1,r0
        SWI     SWI_Font_SetScaleFactor + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
;
        END
