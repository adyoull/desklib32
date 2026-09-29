;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Tinct.Plot.s
;   Author:  Copyright © 2005 Peter Naulls
;   Version: 1.00 (27 May 2005)

       GET     RegDefs.h
       GET     SwiNos.h
       GET     Macros.h


       PREAMBLE
       STARTCODE Tinct_ConvertSprite

       MOV     r2, r0
       MOV     r3, r1
       SWI     SWI_Tinct_ConvertSprite + XOS_Bit
       MOV     pc, lr

       END
