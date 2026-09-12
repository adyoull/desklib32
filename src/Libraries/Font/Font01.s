; Author: Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *Font_FindFont(FontHandle *font, char *name, int xsize, int ysize, int xres, int yres);

;       r0=*font;
;       r1=name
;       r2=xsize
;       r3=ysize
;       r4,[ip,#0]=xres
;       r5,[ip,#4]=yres

        PREAMBLE
        STARTCODE Font_FindFont
;
        MOV     ip,sp
        STMFD   sp!, {r4,r5,r6,lr}
        MOV     r6,r0
        LDR     r4,[ip,#0]
        LDR     r5,[ip,#4]
        SWI     SWI_Font_FindFont + XOS_Bit
        STRVC   r0,[r6,#0]
        MOVVC   r0,#0
        LDMFD   sp!, {r4,r5,r6,pc}
;
        END
