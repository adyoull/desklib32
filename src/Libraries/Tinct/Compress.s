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
       STARTCODE Tinct_Compress
   
       MOV     ip, sp
       STMFD   sp!, {r7,lr}
       MOV     r7, r3
       MOV     r2, r1
       MOV     r3, r2
       SWI     SWI_Tinct_Compress + XOS_Bit
       LDMVSFD sp!, {r7,pc}
       LDR     r1, [ip]
       CMP     r1, #0
       STRNE   r0, [r1]
       MOV     r0, #0
       LDMFD   sp!, {r7,pc}

       END
