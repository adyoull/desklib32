
        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ int TaskWindow_Check(void);

        
        .global TaskWindow_Check
TaskWindow_Check:
@
        STMFD   sp!, {lr}
        MOV     r0, #0
        SWI     SWI_TaskWindow_TaskInfo + XOS_Bit
        LDMFD   sp!, {pc}
@
