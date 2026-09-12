;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Hourglass.s.Start
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (24 Jul 1993)
;   Purpose: Start the hourglass after a delay

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; void Hourglass_Start(int startdelay);

        PREAMBLE
        STARTCODE Hourglass_Start
;
        STMFD   sp!, {lr}
        SWI     SWI_Hourglass_Start + XOS_Bit
        LDMFD   sp!, {pc}
;
        END
