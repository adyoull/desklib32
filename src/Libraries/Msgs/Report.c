/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Msgs.Report.c
    Author:  Copyright © 1993 Philip Colmer
    Version: 1.01 (13 Jul 1993)
             1.02 (05 Sep 2007) Changed vsprintf to vsnprintf
    Purpose: To ease reporting errors which are based on messages
*/

#include <stdarg.h>
#include <stdio.h>

#include "DeskLib:Error.h"
#include "DeskLib:Msgs.h"


extern void Msgs_Report(int errornum, const char *tag, ...)
{
  va_list va;
  char buffer[256];
  char errmess[256];

  if (!Msgs_Lookup(tag, buffer, 252))
    Error_ReportInternal(0, "Cannot find message for error '%s'", tag);
  else
  {
    va_start(va, tag);
    vsnprintf(errmess, sizeof(errmess), buffer, va);
    va_end(va);
    errmess[sizeof(errmess)-1] = '\0';

    Error_Report(errornum, errmess);
  }
}


extern void Msgs_ReportFatal(int errornum, const char *tag, ...)
{
  va_list va;
  char buffer[256];

  if (!Msgs_Lookup(tag, buffer, 252))
    Error_ReportFatalInternal(0,
                          "Cannot find message for fatal error '%s'", tag);
  else
  {
    va_start(va, tag);
    Error_ReportFatal(errornum, buffer, va);
    va_end(va);
  }
}
