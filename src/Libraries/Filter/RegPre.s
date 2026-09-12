; Copyright 1993 Shaun Blackmore

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

;        void Filter_RegisterPreFilter(char *FilterName, FilterHandler handler, int R12, int TaskHandle);

        PREAMBLE
        STARTCODE Filter_RegisterPreFilter
;
        STMFD   sp!, {lr}
        SWI     SWI_Filter_RegisterPreFilter
        LDMFD   sp!, {pc}
;
        END
