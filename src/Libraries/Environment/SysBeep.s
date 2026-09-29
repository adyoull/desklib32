;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Environment/SysBeep.s
;   Author:  Copyright © 1993 Jason Williams
;   Version: 1.00 (28 Mar 1992)
;            1.01 (25 Oct 2007) Moved to Environment module
;   Purpose: Does a VDU 7 in order to sound a system beep

  GET     RegDefs.h
  GET     SwiNos.h
  GET     Macros.h


  PREAMBLE
  STARTCODE Environment_SysBeep

    STMFD   sp!, {lr}
    SWI     OS_WriteI + 7 + XOS_Bit
    LDMFD   sp!, {pc}

  END
