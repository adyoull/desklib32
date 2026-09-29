; Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;        void Filter_DeRegisterPreFilter(char *FilterName, FilterHandler handler, int R12, int TaskHandle);

        PREAMBLE
        STARTCODE Filter_DeRegisterPreFilter
;
        STMFD   sp!, {lr}
        SWI     SWI_Filter_DeRegisterPreFilter
        LDMFD   sp!, {pc}
;
        END
