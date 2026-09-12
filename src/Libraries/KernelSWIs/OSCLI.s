;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    KernelSWIs.s.OSCLI
;   Author:  Copyright © 1995 Sergio Monesi
;   Version: 1.00 (15 Jul 1995)
;   Purpose: Veneer for OS_CLI

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

        PREAMBLE

; os_error *OS_CLI(char *cmd);

        STARTCODE OS_CLI

        MOV     ip, lr
        SWI     SWI_OS_CLI + XOS_Bit
        MOVVC   r0, #0
        MOV     pc, ip


        END
