;-------------------------------------------------------------------------------
; Get graphics clip rectangle 
;
        GET     Macros.h
        GET     RegDefs.h
        GET     SwiNos.h
;
        PREAMBLE
        STARTCODE GFX_GetClip
;
        MOV     a2, #130
        MOV     a3, #131
        MVN     a4, #0
        STMFD   sp!, {a2-a4}
        MOV     a2, #128
        MOV     a3, #129
        STMFD   sp!, {a2-a3}

        MOV     a2, a1
        MOV     a1, sp

        SWI     XSWI_OS_ReadVduVariables

        ADD     sp, sp, #20
        MOV     pc, lr
;
        END
