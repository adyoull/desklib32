; Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; void DragASprite_Start(int flags, void *spritearea, char *name, wimp_box *bound, wimp_box *parent);

        PREAMBLE
        STARTCODE DragASprite_Start
;
        MOV     ip,sp
        STMFD   sp!, {r4,lr}
        LDR     r4,[ip,#0]
        SWI     SWI_DragASprite_Start + XOS_Bit
        MOVVC   r0,#0
        LDMFD   sp!, {r4,pc}
;
        END
