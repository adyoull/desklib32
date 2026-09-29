@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Filing.s.MakePath
@   Author  Copyright © 1994, 1995 Sergio Monesi
@   Version 1.01 (6 Jun 1995)
@   Purpose Constructs the full pathname from the directory and leafname
@   Mods    6 June 1995 - changed procedure name to a more
@                          DeskLib-compliant style

        .include     "RegDefs.h"
        .include     "Macros.h"

@ char *Filing_MakePath(char *newpath, const char *dirname, const char *leafname);

        
        .global Filing_MakePath
Filing_MakePath:
@
        MOVS    ip, r0
        TEQNE   r1, #0
        TEQNE   r2, #0
        MOVEQ   r0, #0
        MOVEQ   pc, lr

loop1:
        LDRB    r3, [r1], #1
        TEQ     r3, #0
        STRNEB  r3, [ip], #1
        BNE     loop1

        MOV     r3, #46            @ = '.'
        STRB    r3, [ip], #1

loop2:
        LDRB    r3, [r2], #1
        TEQ     r3, #0
        STRB    r3, [ip], #1
        BNE     loop2

        MOV     pc,lr
@
