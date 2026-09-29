;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Hourglass.s.On
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (24 Jul 1993)
;   Purpose: Start the Hourglass

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; void Hourglass_On(void);

        PREAMBLE
        STARTCODE Hourglass_On
;
        STMFD   sp!, {lr}
        SWI     SWI_Hourglass_On + XOS_Bit
        LDMFD   sp!, {pc}
;
        END
