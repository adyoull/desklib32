/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.Find.c
    Author:  Copyright © 1992, 1993, 1994 Jason Williams
             Thanks to John Winters for supplying the code that I hacked
             changed, hacked, rewrote, and then wrote again from scratch!
             Sprite area handling added by Tim Browse
    Version: 1.11 (30 Mar 1994)
    Purpose: Loading, cacheing, and retrieval of window templates
*/

#include "DeskLib:GFX.h"

#define __dl_tempfind_c

#include "TempDefs.h"

/* Define the sprite area variable */
sprite_area __template_spritearea = NULL;

extern template_record *Template__FindTemplate(const char *name)
/* not intended for user-consumption! Use Template_Find instead */
{
  template_record *t;

  t = (template_record *) template_list.next;

  while (t != NULL)
  {
    if (!strncmp(name, t->identifier, wimp_MAXNAME))
    {
      if (__template_spritearea != NULL)
      {
        /*
         * Make this window's sprite area point to the required sprite area.
         */
        t->windowdef->spritearea = (void *) __template_spritearea;
      }
      return(t);
    }

    t = (template_record *) t->header.next;
  }

  return(NULL);
}



extern window_block *Template_Find(const char *name)
{
  template_record *t;

  t = Template__FindTemplate(name);

  if (t == NULL)
    return(NULL);

  return(t->windowdef);
}
