;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Str.strcpycr.s
;   Author:  Copyright © 1993 Ainsley Pereira
;   Version: 1.01 (05 Mar 1994)
;   Purpose: Copies a CR-terminated string - note that this will change the
;            terminator of the result to a NUL (0).


        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

        PREAMBLE
        STARTCODE strcpycr

        STMFD   sp!, {v1,lr}
        MOV     v1,#0
strcpycr_00
        LDRB    ip,[a2,v1]
        CMP     ip,#31
        STRHIB  ip,[a1,v1]
        ADDGT   v1,v1,#1
        BGT     strcpycr_00
        MOV     ip,#0
        STRB    ip,[a1,v1]
        LDMFD   sp!, {v1,pc}
;
        END
