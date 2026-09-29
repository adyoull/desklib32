@ Copyright 1993 Shaun Blackmore

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@        void Filter_RegisterPreFilter(char *FilterName, FilterHandler handler, int R12, int TaskHandle);

        
        .global Filter_RegisterPreFilter
Filter_RegisterPreFilter:
@
        STMFD   sp!, {lr}
        SWI     SWI_Filter_RegisterPreFilter
        LDMFD   sp!, {pc}
@
