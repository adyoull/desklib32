/*
 * File: ColourMenu.Colourmenu.c
 * Author: Craig Beech, 27th August 1994
 *
 * Abstract: Create a "colour selector" menu.  This is equivalent to the
 * RISC_OSLib colourmenu function.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

#include "DeskLib:Menu.h"
#include "DeskLib:ColourMenu.h"


/****************************************************************************

> menu_ptr ColourMenu_Create(char *name);

  Inputs:   name - title of colour menu.
  Returns:  menu_ptr - colour menu.
  Purpose:  Creates a colour menu similar to the RISC_OSLib function.

****************************************************************************/

extern menu_ptr ColourMenu_Create(const char *name)
{ menu_ptr menu;
  menu_item *item;

  menu = Menu_New(name, "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15");

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[0];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_WHITE;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[1];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_GREY1;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[2];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_GREY2;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[3];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_GREY3;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[4];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_GREY4;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[5];
  item->iconflags.data.foreground = colour_WHITE;
  item->iconflags.data.background = colour_GREY5;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[6];
  item->iconflags.data.foreground = colour_WHITE;
  item->iconflags.data.background = colour_GREY6;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[7];
  item->iconflags.data.foreground = colour_WHITE;
  item->iconflags.data.background = colour_BLACK;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[8];
  item->iconflags.data.foreground = colour_WHITE;
  item->iconflags.data.background = colour_DARK_BLUE;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[9];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_YELLOW;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[10];
  item->iconflags.data.foreground = colour_WHITE;
  item->iconflags.data.background = colour_GREEN;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[11];
  item->iconflags.data.foreground = colour_WHITE;
  item->iconflags.data.background = colour_RED;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[12];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_CREAM;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[13];
  item->iconflags.data.foreground = colour_WHITE;
  item->iconflags.data.background = colour_ARMY_GREEN;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[14];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_ORANGE;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  item = &item[15];
  item->iconflags.data.foreground = colour_BLACK;
  item->iconflags.data.background = colour_LIGHT_BLUE;

  return menu;
}
