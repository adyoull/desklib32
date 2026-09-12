;
;       Title                  : Append Sprite
;       System                 : Sprite Library
;       Version                : 1.0
;       Copyright              : (C) John Tytgat
;       Date                   : 29 Nov 2005
;       Author                 : John Tytgat
;
;       Function               : Appends sprites
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
        STARTCODE Sprite_AppendP
;
        STMFD     sp!, {a1-a4, v1, lr}
        LDMFD     sp!, {a2-a4, v1}
        MOV       a1, #512
        ADD	  a1, a1, #35
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1, #0
        LDMFD     sp!, {v1, pc}
;
	END
