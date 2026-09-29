@ Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@        void Filter_DeRegisterPreFilter(char *FilterName, FilterHandler handler, int R12, int TaskHandle);

        
        .global Filter_DeRegisterPreFilter
Filter_DeRegisterPreFilter:
@
        STMFD   sp!, {lr}
        SWI     SWI_Filter_DeRegisterPreFilter
        LDMFD   sp!, {pc}
@
