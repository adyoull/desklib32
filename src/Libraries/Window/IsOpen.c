/*
 * This file is part of DeskLib, the C WIMP library for RISC OS.
 *   Please see accompanying documentation for terms of use.
 *
 *         http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module: Window
 * File: IsOpen.c
 * Author: Adam Richardson
 * Purpose: Check if a window (given by its handle) is already open.
 *
 * Version History
 * 1/7/2007: Creation
 *
 */

#include "DeskLib:Window.h"

extern BOOL Window_IsOpen(window_handle window)
{
  BOOL returnvalue = FALSE;
  window_info winfo;

  Window_GetInfo3(window, &winfo);

  if (winfo.block.flags.data.open) returnvalue = TRUE;

  return returnvalue;
}
