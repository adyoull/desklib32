/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    MsgTrans.MTDefs.h
    Author:  Copyright © 1994 Tony Houghton
    Version: 1.00 (27 Sep 1994)
    Purpose: Definitions for internal use by some MsgTrans functions
*/

#include "DeskLib:LinkList.h"
#include "DeskLib:MsgTrans.h"

typedef struct {
  linklist_header list;
  char *data;
  msgtrans_filedesc filedesc;
} msgtrans_file;

extern linklist_header msgtrans__list;
