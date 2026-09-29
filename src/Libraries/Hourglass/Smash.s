;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Hourglass.s.Smash
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (24 Jul 1993)
;   Purpose: Stop the hourglass (NOW!)

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; void Hourglass_Smash(void);

        PREAMBLE
        STARTCODE Hourglass_Smash
;
        STMFD   sp!, {lr}
        SWI     SWI_Hourglass_Smash + XOS_Bit
        LDMFD   sp!, {pc}
;
        END
