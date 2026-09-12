/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.ClearAll.c
    Author:  Copyright © 1992, 1993, 1994 Jason Williams
             Thanks to John Winters for supplying the code that I hacked
             changed, hacked, rewrote, and then wrote again from scratch!
    Version: 1.20 (24 Apr 1994)
    Purpose: Loading, cacheing, and retrieval of window templates
*/

#include "TempDefs.h"
#include "DeskLib:Font.h"


extern void Template_ClearAll(void)
/* Wipes all templates currently known to the system.
 * Deallocates ALL memory used by these templates
 */
{
  template_record *t, *n;

  t = (template_record *) template_list.next;

  while (t != NULL)
  {
    /*  Free memory used by the template
     *  NOTE that Template_LoadFile allocs 2 meory blocks per template
     *  while cloned templates are fragmented into lots of smaller blocks.
     *  Thus, we must not use Template_Free to deallocate the memory!
     *  (Only LoadFile'd templates can alppear in our template list)
     */
    free(t->windowdef);
    free(t->indirectdata);

    n = (template_record *) t->header.next;
    LinkList_Unlink(&template_list, &(t->header));
    free(t);
    t = n;
  }

  LinkList_Init(&template_list);         /* just to be on the safe side! */

  if ((int) template_fontarray > 0)
    Font_LoseAllFonts(template_fontarray);
}
