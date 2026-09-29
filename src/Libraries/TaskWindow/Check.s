
        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; int TaskWindow_Check(void);

        PREAMBLE
        STARTCODE TaskWindow_Check
;
        STMFD   sp!, {lr}
        MOV     r0, #0
        SWI     SWI_TaskWindow_TaskInfo + XOS_Bit
        LDMFD   sp!, {pc}
;
        END
