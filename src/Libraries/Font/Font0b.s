@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_CurrentFont(FontState *state);



        
        .global Font_CurrentFont
Font_CurrentFont:
@
        STMFD   sp!, {r4,lr}
        MOV     r4,r0
        SWI     SWI_Font_CurrentFont + XOS_Bit
        STR     r0,[r4,#0]
        STR     r1,[r4,#4]
        STR     r2,[r4,#8]
        STR     r3,[r4,#12]
        MOVVC   r0,#0
        LDMFD   sp!, {r4,pc}
@
