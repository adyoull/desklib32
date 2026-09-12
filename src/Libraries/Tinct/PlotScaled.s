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
       STARTCODE Tinct_PlotScaled
   
       MOV     ip, sp
       STMFD   sp!, {r4-r7,lr}
       MOV     r7, r3
       MOV     r4, r2
       MOV     r3, r1
       MOV     r2, r0
       LDMIA   ip, {r5, r6}
       SWI     SWI_Tinct_PlotScaled + XOS_Bit
       MOVVC   r0, #0
       LDMFD   sp!, {r4-r7,pc}

       END
