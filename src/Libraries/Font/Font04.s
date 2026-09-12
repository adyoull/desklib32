; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;os_error *Font_ReadInfo(FontHandle font, FontInfo *info);

;       r0=font
;       r1=pointer to definition block

;       [r5,#0]  = min x
;       [r5,#4]  = min y
;       [r5,#8]  = max x
;       [r5,#12] = max y

        PREAMBLE
        STARTCODE Font_ReadInfo
;
        STMFD   sp!, {r4,r5,lr}
        MOV     r5,r1
        SWI     SWI_Font_ReadInfo + XOS_Bit
        STMIA   r5,{r1,r2,r3,r4}
        MOVVC   r0,#0
        LDMFD   sp!, {r4,r5,pc}
;
        END
