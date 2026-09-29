;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Hourglass.s.Off
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (24 Jul 1993)
;   Purpose: Off

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; void Hourglass_Off(void);

        PREAMBLE
        STARTCODE Hourglass_Off
;
        STMFD   sp!, {lr}
        SWI     SWI_Hourglass_Off + XOS_Bit
        LDMFD   sp!, {pc}
;
        END
