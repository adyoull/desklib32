; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;os_error *Font_SetFontColours(FontHandle font, int back_colour, int fore_colour, int offset);

        PREAMBLE
        STARTCODE Font_SetFontColours
;
        STMFD   sp!, {lr}
        SWI     SWI_Font_SetFontColours + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
        END
