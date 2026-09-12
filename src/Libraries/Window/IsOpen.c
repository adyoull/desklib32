/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.IsOpen.c
    Purpose: High-level window management functions: Check if window is open
*/


#include "DeskLib:Window.h"

extern BOOL Window_IsOpen(window_handle window)
/* Checks if window is open */
{
  BOOL returnvalue = FALSE;
  window_info winfo;

  Window_GetInfo3(window, &winfo);

  if (winfo.block.flags.data.open) returnvalue = TRUE;

  return returnvalue;
}
