/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Screen.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (23 Mar 1992)
             1.01 (10 Oct 2007) Changed mode variables from macros to enum
                                & renamed dl_ReadModeVar
    Purpose: Screen functions (read size, eig factors, etc.)
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Screen.h"

screen_modeval screen_mode  = { -1 };
wimp_point     screen_size;
wimp_point     screen_eig;
wimp_point     screen_delta;
int            screen_bpp;

/* This is also defined in BarIcon.c as Icon_ReadModeVar */
#define Screen_ReadModeVar(m, v, r) \
  SWI(3, 3, SWI_OS_ReadModeVariable, (m), (v), 0,   0, 0, (r))

extern BOOL Screen_CacheModeInfo(void)
{
  screen_modeval oldmode = screen_mode;

  SWI(1, 3, SWI_OS_Byte, 135, NULL, NULL, &screen_mode);

  if (oldmode.screen_mode == screen_mode.screen_mode)
    return(FALSE);

  Screen_ReadModeVar(-1, modevar_LOG2BPP, &screen_bpp);
  screen_bpp = 1 << screen_bpp;

  Screen_ReadModeVar(-1, modevar_XEIGFACTOR, &screen_eig.x);
  Screen_ReadModeVar(-1, modevar_YEIGFACTOR, &screen_eig.y);

  screen_delta.x = 1 << screen_eig.x;
  screen_delta.y = 1 << screen_eig.y;

  Screen_ReadModeVar(-1, modevar_XWINDOWLIMIT, &screen_size.x);
  screen_size.x = (screen_size.x + 1) << screen_eig.x;

  Screen_ReadModeVar(-1, modevar_YWINDOWLIMIT, &screen_size.y);
  screen_size.y = (screen_size.y + 1) << screen_eig.y;

  return(TRUE);
}
