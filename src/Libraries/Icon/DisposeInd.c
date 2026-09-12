/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icons.DisposeInd.c
    Author:  Copyright © 1993 (Tim Browse)
    Version: 1.00 (27 Mar 1993)
    Purpose: Dispose indirected data space used by an icon. Note that this
             data must have been allocated with malloc() originally.
*/

#include <stdlib.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:Icon.h"


extern void Icon_DisposeIndData(icon_data *data, icon_flags flags)
{
  /* make sure this icon is indirected - exit quietly if not */
  if ((data == NULL) || (!flags.data.indirected))
    return;

  if (!flags.data.text && flags.data.sprite)             /* Sprite only icon */
  {
    if (data->indirectsprite.nameisname)
      free((void *)(int)data->indirectsprite.name);
    return;
  }

  if (flags.data.text || flags.data.sprite)        /* Text/Text&Sprite icon */
  {
    free(data->indirecttext.buffer);

    /* Free validation string, if there is one */
    if ((data->indirecttext.validstring != NULL) &&
        (data->indirecttext.validstring != (char *) 0xffffffff))
      free(data->indirecttext.validstring);
  }
}
