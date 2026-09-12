;
;       Title                  : Read palette size
;       System                 : Sprite Library
;       Version                : 1.0
;       Copyright              : (C) John Tytgat
;       Date                   : 29 Nov 2005
;       Author                 : John Tytgat
;
;       Function               : Reads size of sprite's palette
;
;
;       Modification history.
;
;       Version                : (Reflect in header IDENT)
;       Date                   :
;       Author                 :
;       Changes                :
;
;
;============================================================================
;
;  Include files.
;
;============================================================================
;
        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h
;
;============================================================================
;
;  Code.
;
;============================================================================
;
        PREAMBLE
        STARTCODE Sprite_ReadPaletteSizeP
;
        STMFD     sp!, {a1-a2, v1-v2, lr}
        LDMFD     sp!, {a2-a3}
        MOV       a1, #512
        ADD       a1, a1, #37
        MVN       a4, #0
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1, a4
        MOVVS     a1, #0
        LDMFD     sp!, {v1-v2, pc}
;
	END
