;
;       Title                  : Read pixel
;       System                 : Sprite Library
;       Version                : 1.1
;       Copyright              : (C) Ainsley Pereira
;       Date                   : Sun 27th February 94
;       Author                 : Ainsley M. Pereira
;
;       Function               : Read pixel from a sprite
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
        STARTCODE Sprite_ReadPixel
;
        STMFD     sp!, {a4, v1-v3, lr}
	LDMIA	  a3, {a4, v1}		; Reads x,y coord
        MOV       a3, a2
        MOV       a2, a1
        MOV       a1, #256
        ADD	  a1, a1, #41
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1, #0
	LDRVC     ip, [sp], #4		; Write colour
        STRVC     v2, [ip]
	LDRVC     ip, [sp, #4*4] 	; Write tint
	STRVC     v3, [ip]
        LDMFD     sp!, {v1-v3, pc}
;
	END
