@ Copyright 1993 Shaun Blackmore, Sergio Monesi

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@        void Filter_RegisterPostFilter(char *FilterName, FilterHandler handler, int R12, int TaskHandle, int EventMask);

        
        .global Filter_RegisterPostFilter
Filter_RegisterPostFilter:
@
        MOV     ip,sp
        STMFD   sp!, {r4,lr}
        LDR     r4,[ip]
        SWI     SWI_Filter_RegisterPostFilter
        LDMFD   sp!, {r4,pc}
@
