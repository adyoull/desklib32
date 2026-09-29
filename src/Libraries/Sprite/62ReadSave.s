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
@   File    Sprite.s.62ReadSave
@   Author  Copyright © 1993,1994 Jason Williams, Jason Howat
@   Version 1.01 (10 Jun 1994)
@   Purpose Read the size of a sprite save area (veneer to SpriteOp 62)

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"
@
        
        .global Sprite_ReadSaveAreaSize
Sprite_ReadSaveAreaSize:
@
@ extern os_error *Sprite_ReadSaveAreaSize(areainfo*, sprname*, size*);
@
        STMFD   sp!, {v1,lr}

        MOV     v1, a3             @ r3 = &size

        MOV     a3, a2             @ r2 = &sprname
        MOV     a2, a1             @ r1 = 0xareainfo

        MOV     a1, #256           @ r0 = 256 + 62
        ADD     a1, a1, #62

        SWI     OS_SpriteOp + XOS_Bit

        STRVC   a4, [v1]           @ store resulting size
        MOVVC   a1, #0             @ return NULL (no error)

        LDMFD   sp!, {v1,pc}
@
