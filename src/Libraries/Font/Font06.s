@ Author Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@os_error *Font_Paint(char *, int options, int x, int y);

@   r0=char*s
@   r1=options
@   r2=x
@   r3=y

        
        .global Font_Paint
Font_Paint:
@
        STMFD   sp!, {r4,lr}
        MOV     r4,r3
        MOV     r3,r2
        MOV     r2,r1
        MOV     r1,r0
        SWI     SWI_Font_Paint + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {r4,pc}
@
