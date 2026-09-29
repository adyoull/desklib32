@ Author  Copyright 1993 Jason Williams
@ Version 1.00 (14 May 1993)

@ BOOL Kbd_KeyDown(keynum)
@   where keynum is a negative INKEY number, or 0..128
@   e.g. -1 = SHIFT, -2 = CTRL, etc
@ Returns TRUE if the given key is currently depressed
@ (Who would have thought you had a manic depressive keyboard? @-)


        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"


        
        .global Kbd_KeyDown
Kbd_KeyDown:

        STMFD  sp!, {lr}

        MOV    r1, r0
        MOV    r0, #129
        MOV    r2, #255
        SWI    SWI_OS_Byte + XOS_Bit

        CMP    r1, #255
        MOVEQ  r0, #1                   @ return(TRUE);
        MOVNE  r0, #0                   @ return(FALSE);

        LDMFD  sp!, {pc}

