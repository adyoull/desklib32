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
#include <stdio.h>
#include <string.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:LinkList.h"
#include "DeskLib:MsgTrans.h"
#include "DeskLib:File.h"
#include "DeskLib:Resource.h"
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
  char namebuff[wimp_MAXPATH];

  block = malloc(sizeof(msgtrans_file));
  if (!block)
  {
    MsgTrans_Lookup(0,"NoStore",error.errmess,252);
    return &error;
  }

  strncpy(namebuff, filename, sizeof(namebuff) - 1);
  namebuff[sizeof(namebuff) - 1] = '\0'; /* Ensure correct termination */

  /* Check if file exists */
  if (!File_Exists(namebuff))
  {
    /* File not found, try prefixing with resources location */
    snprintf(namebuff, sizeof(namebuff), "%s%s", resource_pathname, filename);

    if (!File_Exists(namebuff))
    {
      /* Still can't find file */
      snprintf(error.errmess, sizeof(error.errmess), "Messages file not found. Tried '%s' and '%s'", filename, namebuff);
      error.errnum = 1;
      return &error;
    }
  }

  swierr = MessageTrans_FileInfo(namebuff, &flags, &size);
  if (swierr)
    return (swierr);

  block->data = malloc(size);
  if (!block->data)
  {
    MsgTrans_Lookup(0,"NoStore",error.errmess,252);
    free(block);
    return &error;
  }

  swierr = MessageTrans_OpenFile(&block->filedesc, namebuff, block->data);
  if (swierr)
  {
    MsgTrans_Lookup(0,"NoStore",error.errmess,252);
    free(block->data);
    free(block);
  }

  LinkList_AddToTail(&msgtrans__list,&block->list);

  /* Only return filedesc if function was successful */
  if (!swierr) *filedesc = &block->filedesc;

  return (swierr);
}
