@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_SetFontColours(FontHandle font, int back_colour, int fore_colour, int offset);

        
        .global Font_SetFontColours
Font_SetFontColours:
@
        STMFD   sp!, {lr}
        SWI     SWI_Font_SetFontColours + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
