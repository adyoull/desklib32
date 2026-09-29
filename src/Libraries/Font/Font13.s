@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_SetPalette(int back_log, int fore_log, int offset, int back_phys, int fore_phys);
@
@ r0=back_log
@ r1=fore_log
@ r2=offset
@ r3=back_phys
@ [ip,#0]=fore_phys

        
        .global Font_SetPalette
Font_SetPalette:
@
        MOV     ip,sp
        STMFD   sp!, {r4,r5,lr}
        LDR     r5,[ip,#0]
        MOV     r4,r3
        MOV     r3,r2
        MOV     r2,r1
        MOV     r1,r0
        SWI     SWI_Font_SetPalette + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {r4,r5,pc}
