/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.MakeIndrct.c
    Author:  Copyright © 1994 Lenny
    Version: 0.01 (20 Nov 1994)
    Purpose: Makes the specified menu item an 'indirected text' icon.
    History: 0.01 (20 Nov 94) : Added 'Menu_MakeIndirected()'

*/

/* --- LOAD HEADERS ------------------------------------------------------ */

/* --- DeskLib ----------------------------------------------------------- */
#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"

/* === FUNCTION DEFINITIONS ============================================== */

extern void Menu_MakeIndirected(menu_ptr menu, int entry, char *buffer, int size)
/*
 *  Makes the specified menu item 'indirected text', pointing to 'buffer'.
 *  ie the menu item will contain the text held in 'buffer'.
 *
 *  Similar to 'Menu_MakeWritable()' except this doesn't produce a writable
 *  entry.  It does provide a user defined buffer though, and updating
 *  this is simpler (and quicker) than calling 'Menu_SetText()'.
 *  The validation string is set to null, as this is only really of use
 *  with a writable icon, when the user may need to restrict the characters
 *  entered.
 */
{
  menu_item *item = (menu_item *) (((int) menu) + sizeof(menu_block));

  item = &item[entry];
  item->iconflags.data.indirected = TRUE;

  item->icondata.indirecttext.buffer      = buffer;
  item->icondata.indirecttext.bufflen     = size;
  item->icondata.indirecttext.validstring = (char *) -1;

}

/***************************************************************************/
