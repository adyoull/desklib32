/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    MsgTrans.LoseFile.c
    Author:  Copyright © 1994 Tony Houghton
    Version: 1.02 (28 Dec 2002)
    Purpose: Close a Messages file and free its memory
*/

#include <stdlib.h>

#include "DeskLib:LinkList.h"
#include "DeskLib:MsgTrans.h"

#include "MTDefs.h"


os_error *MsgTrans_LoseFile(msgtrans_filedesc *filedesc)
{
  msgtrans_file *block;
  os_error *swierr = MessageTrans_CloseFile(filedesc);

  for (block = LinkList_NextItem(&msgtrans__list);block;block = LinkList_NextItem(block))
    if (&block->filedesc == filedesc)
    {
      free(block->data);
      LinkList_Unlink(&msgtrans__list,&block->list);
      free(block);
      break;
    }
  return (swierr);
}

