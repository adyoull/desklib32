/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    MsgTrans.LoadFile.c
    Author:  Copyright © 1994 Tony Houghton
    Version: 1.00 (27 Sep 1994)
    Purpose: Load a Messages file
*/

#include <stdlib.h>

#include "DeskLib:LinkList.h"
#include "DeskLib:MsgTrans.h"
#include "MTDefs.h"

linklist_header msgtrans__list = {NULL,NULL};

static os_error error;


os_error *MsgTrans_LoadFile(msgtrans_filedesc **filedesc,
                            const char *filename)
{
  int flags;
  int size;
  os_error *swierr;
  msgtrans_file *block;

  block = malloc(sizeof(msgtrans_file));
  if (!block)
  {

    MsgTrans_Lookup(0,"NoStore",error.errmess,252);
    return &error;
  }

  swierr = MessageTrans_FileInfo(filename, &flags, &size);
  if (swierr)
    return (swierr);
  block->data = malloc(size);
  if (!block->data)
  {
    MsgTrans_Lookup(0,"NoStore",error.errmess,252);
    free(block);
    return &error;
  }

  swierr = MessageTrans_OpenFile(&block->filedesc, filename, block->data);
  if (swierr)
  {
    MsgTrans_Lookup(0,"NoStore",error.errmess,252);
    free(block->data);
    free(block);
  }

  LinkList_AddToTail(&msgtrans__list,&block->list);

  *filedesc = &block->filedesc;

  return (swierr);
}
