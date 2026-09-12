/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.Delete.c
    Author:  Copyright © 1992, 1993, 1994 Jason Williams
             Thanks to John Winters for supplying the code that I hacked
             changed, hacked, rewrote, and then wrote again from scratch!
    Version: 1.11 (24 Apr 1994)
    Purpose: Loading, cacheing, and retrieval of window templates
*/

#include "TempDefs.h"


extern void Template_Delete(const char *name)
/* Attempts to delete the named template, and deallocate its memory */
{
  template_record *t;

  t = Template__FindTemplate(name);
  if (t == NULL)
    return;

  free(&(t->windowdef));
  free(t->indirectdata);

  LinkList_Unlink(&template_list, &(t->header));

  free(t);
}
