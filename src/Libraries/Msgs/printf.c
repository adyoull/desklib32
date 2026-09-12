/*
 * File: Msgs.printf.c
 * Author: Jason Williams, 8th April 1992
 *
 * Abstract: MessageTrans-like message handling functions. (If you want
 * MessageTrans, use the SWI interface, if you want high-level message
 * handling, use this code...)
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

#include <stdarg.h>
#include <stdio.h>

#undef vsprintf

#include "DeskLib:Msgs.h"

extern BOOL Msgs_printf(char *result, const char *formattag, ...)
{
  va_list ap;
  char temp[1024];

  result[0] = '\0';

  if (Msgs_Lookup(formattag, temp, 1020))
  {
    va_start(ap, formattag);
    vsprintf(result, temp, ap);
    va_end(ap);

    return (TRUE);
  }

  return (FALSE);
}
