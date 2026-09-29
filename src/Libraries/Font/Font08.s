@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_ConvertToOS(int x, int y, int *xout, int *yout)



        
        .global Font_ConvertToOS
Font_ConvertToOS:
@
        STMFD   sp!, {r4,lr}
        MOV     r4,r3
        MOV     r3,r2
        MOV     r2,r1
        MOV     r1,r0
        SWI     SWI_Font_ConverttoOS + XOS_Bit
        STR     r1,[r3,#0]
        STR     r2,[r4,#0]
        MOVVC   r0,#0
        LDMFD   sp!, {r4,pc}
@