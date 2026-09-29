; Copyright 1993 Shaun Blackmore, Sergio Monesi

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;        void Filter_DeRegisterPostFilter(char *FilterName, FilterHandler handler, int R12, int TaskHandle, int EventMask);

        PREAMBLE
        STARTCODE Filter_DeRegisterPostFilter
;
        MOV     ip,sp
        STMFD   sp!, {r4,lr}
        LDR     r4,[ip]
        SWI     SWI_Filter_DeRegisterPostFilter
        LDMFD   sp!, {r4,pc}
;
        END
