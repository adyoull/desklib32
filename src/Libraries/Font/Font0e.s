; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;os_error *Font_CharBBox(FontHandle font, char c, int flags, FontInfo *info);

        PREAMBLE
        STARTCODE Font_CharBBox
;
        STMFD   sp!, {r4,r5,lr}
        MOV     r5,r3
        SWI     SWI_Font_CharBBox + XOS_Bit
        STR     r1,[r5,#0]
        STR     r2,[r5,#4]
        STR     r3,[r5,#8]
        STR     r4,[r5,#12]
        MOVVC   r0,#0
        LDMFD   sp!, {r4,r5,pc}
;
        END
