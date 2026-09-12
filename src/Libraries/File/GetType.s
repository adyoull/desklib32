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
;   File:    File.s.GetType
;   Author:  Ben Summers, but only because Jason Williams
;            forgot to write the damn thing
;   Version: 1.02 (25 Sep 2002)
;   Purpose: SWI veneer for file operations - get the type of a file

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h
;
        PREAMBLE
        STARTCODE File_GetType
;
; extern int File_GetType(const char *filename);
;
        STMFD   sp!, {v1, v2, lr}

        MOV     a2, a1
        MOV     a1, #17

        SWI     SWI_OS_File + XOS_Bit
        BVS     err
        TST     a1,#1      ; Success for file or image file
        BEQ     err

        MOV     a1,a3,LSL#12
        MOV     a1,a1,LSR#12+8

        LDMFD   sp!, {v1, v2, pc}
err
        MOV     a1,#-1
        LDMFD   sp!, {v1, v2, pc}
;
        END
