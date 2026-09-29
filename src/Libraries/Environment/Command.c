/*
 * This file is part of DeskLib, the C library for RISC OS.
 * Please see accompanying documentation for terms of use.
 *
 *       http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module:  Environment
 * File:    Command.c
 * Author:  Adam Richardson
 * Purpose: Return OS version info.
 *
 * Version History
 * 18/02/2008: Creation
 *
 */

#include "DeskLib:Environment.h"
#include "DeskLib:KernelSWIs.h"
#include <stdio.h>
#include <stdarg.h>

os_error *Environment_Command(const char *format, ...)
{
  char buffer[1024];
  va_list argptr;
  os_error *swierr;

  va_start(argptr, format);
  vsnprintf(buffer, sizeof(buffer), format, argptr);
  va_end(argptr);

  /* If the output string runs over the end of the buffer, it will be
     truncated but not null-terminated, so have to account for that: */
  buffer[sizeof(buffer)-1] = '\0';

  swierr = Environment__OS_CLI(buffer);

  return swierr;
}
