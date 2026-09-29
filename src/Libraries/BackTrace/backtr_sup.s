@ backtr_sup.s
@ Machine code support function for the backtrace() function
@ (c) Paul Field 1995


        .global  BackTrace_SupportCurrentFrame
BackTrace_SupportCurrentFrame:
        STR      fp, [a1, #0x18]
        STR      sp, [a1, #0x1c]
        STR      sl, [a1, #0x24]!
        MOV     pc, lr

