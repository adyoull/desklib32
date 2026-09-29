@ Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ void DragASprite_Stop(void);

        
        .global DragASprite_Stop
DragASprite_Stop:
@
        STMFD   sp!, {lr}
        SWI     SWI_DragASprite_Stop + XOS_Bit
        MOVVC   r0, #0
        LDMFD   sp!, {pc}
@
