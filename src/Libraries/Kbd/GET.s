; Author:  Copyright 1993 Jason Williams
; Version: 1.00 (24 Jul 1993)

; int Kbd_GET(void)
; (OS_ReadC veneer)
; returns the ASCII code of the next pressed (buffered) key (as in BASIC's GET)


        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h


        PREAMBLE
        STARTCODE Kbd_GET

        STMFD  sp!, {lr}
        SWI    SWI_OS_ReadC + XOS_Bit
        LDMFD  sp!, {pc}

        END
