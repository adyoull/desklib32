@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Tinct.Plot.s
@   Author  Copyright © 2005 Peter Naulls
@   Version 1.00 (27 May 2005)

       .include     "RegDefs.h"
       .include     "SwiNos.h"
       .include     "Macros.h"


       
       .global Tinct_AvailableFeatures
Tinct_AvailableFeatures:

       SWI     SWI_Tinct_AvailableFeatures + XOS_Bit
       MOV     pc, lr

