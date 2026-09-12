;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    KernelSWIs.s.SWINumStr
;   Author:  Copyright © 1994, 1995 Sergio Monesi
;   Version: 1.01 (13 Jul 1995)
;   Purpose: Veneers for OS_SWINumber(To|From)String

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

        PREAMBLE

; os_error *OS_SWINumberToString(int number, char *buffer, int size);

        STARTCODE OS_SWINumberToString

        MOV     ip, lr
        SWI     SWI_OS_SWINumberToString + XOS_Bit
        MOVVC   r0, #0
        MOV     pc, ip


; os_error *OS_SWINumberFromString(char *string, int *number);

        STARTCODE OS_SWINumberFromString

        MOV     ip, lr
        MOV     r2, r1
        MOV     r1, r0
        SWI     SWI_OS_SWINumberFromString + XOS_Bit
        MOVVS   pc, ip
        TEQ     r2, #0
        STRNE   r0, [r2]
        MOV     r0, #0
        MOV     pc, ip


        END
