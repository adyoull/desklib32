;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    KernelSWIs.s.GSTrans
;   Author:  Copyright © 1995 Sergio Monesi
;   Version: 1.00 (13 Jul 1995)
;   Purpose: Veneer for OS_GSTrans

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

        PREAMBLE

; os_error *OS_GSTrans(char *string, char *buffer, int size, char **end);

        STARTCODE OS_GSTrans

        MOV     ip, lr
        SWI     SWI_OS_GSTrans + XOS_Bit
        MOVVS   pc, ip
        TEQ     r3, #0
        STRNE   r0, [r3]
        MOV     r0, #0
        MOV     pc, ip


        END
