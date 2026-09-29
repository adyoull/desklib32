@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_SetFont(FontHandle font);



        
        .global Font_SetFont
Font_SetFont:
@
        STMFD   sp!, {lr}
        SWI     SWI_Font_SetFont + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
@
