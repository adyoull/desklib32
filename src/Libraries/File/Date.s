;
;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    File.s.Date
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (31 Jul 1993)
;   Purpose: SWI veneer for file operations - read datestamp

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h
;
        PREAMBLE
        STARTCODE File_Date
;
; extern void File_Date(char *filename, unsigned char *fivebyteblock);
;
        STMFD   sp!, {a2,v1,v2,lr}

        MOV     a2, a1
        MOV     a1, #17

        SWI     SWI_OS_File + XOS_Bit

        LDMFD   sp!, {a2,v1,v2,lr}

        STR     a4, [a2],#4
        STRB    a3, [a2]

        MOV     pc, lr
;
        END
