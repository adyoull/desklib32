; backtr_sup.s
; Machine code support function for the backtrace() function
; (c) Paul Field 1995


        AREA |C$$code|, CODE, READONLY

        EXPORT  BackTrace_SupportCurrentFrame
BackTrace_SupportCurrentFrame
        STR      fp, [a1, #&18]
        STR      sp, [a1, #&1c]
        STR      sl, [a1, #&24]!
        MOV     pc, lr

        END
