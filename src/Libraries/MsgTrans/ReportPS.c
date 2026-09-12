/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    MsgTrans.ReportPS.c
    Author:  Copyright © 1994 Tony Houghton
    Version: 1.00 (05 Nov 1994)
    Purpose: Match a token in a Messages file and report it as an error

*/

#include "DeskLib:Error.h"
#include "DeskLib:MsgTrans.h"

os_error *MsgTrans_ReportPS(msgtrans_filedesc *filedesc, char *token,
                            BOOL fatal,
                            const char *p0,
                            const char *p1,
                            const char *p2,
                            const char *p3)
{
  os_error *swierr;
  char buffer[252];

  swierr = MsgTrans_LookupPS(filedesc, token, buffer, 252,
                             p0, p1, p2, p3);
  if (swierr)
    return (swierr);

  if (fatal)
    Error_ReportFatal(0,buffer);
  else
    Error_Report(0,buffer);

  return NULL;
}
