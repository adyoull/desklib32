@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_StringBBox(char *string, FontInfo *info);


        
        .global Font_StringBBox
Font_StringBBox:
@
        STMFD   sp!, {r4,r5,lr}
        MOV     r5,r1
        MOV     r1,r0
        SWI     SWI_Font_StringBBox + XOS_Bit
        STMIA   r5,{r1,r2,r3,r4}
        MOVVC   r0,#0
        LDMFD   sp!, {r4,r5,pc}
