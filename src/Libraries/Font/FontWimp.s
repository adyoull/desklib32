; Author: Copyright 2003 Peter Naulls

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; extern os_error *Font_GetWimpFont(font_handle *handle);

        PREAMBLE
        STARTCODE Font_GetWimpFont
;
        MOV     r2, r0
        MOV     r0, #8
        SWI     SWI_Wimp_ReadSysInfo + XOS_Bit
        STR     r0, [r2]
        MOVVC   a1,#0
        MOV     pc, lr
;
        END
