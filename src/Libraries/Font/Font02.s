; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *Font_LoseFont(FontHandle font);

;       r0=font;


        PREAMBLE
        STARTCODE Font_LoseFont
;
        STMFD   sp!, {lr}
        SWI     SWI_Font_LoseFont + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {pc}
;
        END
