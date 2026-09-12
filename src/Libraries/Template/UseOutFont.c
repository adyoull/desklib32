/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.UseOutFont.c
    Author:  Copyright © 1993 Jason Williams
    Version: 1.00 (29 Jun 1993)
    Purpose: Support for use of outline fonts in templates
*/


#include "TempDefs.h"

#include "DeskLib:Font.h"


static void Template_ExitHandler(void)
/*  Called on exit to clean up the fonts we're using */
{
  if ((int) template_fontarray > 0)
    Font_LoseAllFonts(template_fontarray);
}


extern void Template_UseOutlineFonts(void)
/*  Allocates store for the font usage array - must be called before calls
 *  to Template_LoadFile or your outline fonts won't be used.
 *  Note that if it fails, it does so quietly - your windows will come up
 *  without outline fonts.
 */
{
  if ((int) template_fontarray > 0) return;

  template_fontarray = malloc(sizeof(font_array));
  if (template_fontarray == NULL)
    template_fontarray = (font_array *) -1;     /* Failed! Ensure we are safe*/
  else
  {
    int i;

    for (i = 0; i < 256; i++)                   /* Initialise counts to zero */
      template_fontarray->fonts[i] = 0;

    atexit(Template_ExitHandler);               /* Set up exit handler       */
  }
}
