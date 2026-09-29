@
@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    File.s.Delete
@   Author  Copyright © 1993 Jason Williams
@   Version 1.01 (13 May 1994)
@   Purpose SWI veneer for file operations - delete a file

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"
@
        
        .global File_Delete
File_Delete:
@
@ extern os_error *File_Delete(char *filename);
@
        STMFD   sp!, {v1, v2, lr}

        MOV     a2, a1
        MOV     a1, #6

        SWI     SWI_OS_File + XOS_Bit

        MOVVC   a1, #0

        LDMFD   sp!, {v1, v2, pc}
@
