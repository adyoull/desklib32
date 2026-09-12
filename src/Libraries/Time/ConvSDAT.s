;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Misc.ConvSDAT.s
;   Author:  Copyright © 1994 Jason Howat
;   Version: 1.00 (12 May 1994)
;   Purpose: Convert given time to text using standard format string.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

        PREAMBLE
        STARTCODE Time_ConvertStandardDateAndTime
;
        STMFD   sp!, {lr}
        SWI     SWI_OS_ConvertStandardDateAndTime + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
