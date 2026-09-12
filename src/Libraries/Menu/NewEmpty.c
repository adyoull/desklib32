/*
 * File: Menu.NewEmpty.c
 * Author: Antony Sidwell, 1st January 2003
 *
 * Abstract: Creates an "empty" menu of a given size.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

#include <string.h>
#include <stdlib.h>

#include "DeskLib:Menu.h"
#include "DeskLib:Wimp.h"

/* -------------------------------------------------- ajps, 01/01/03 ---
 * This creates an empty menu with the given title and number of entries.
 * This is useful if you need to dynamically create a menu - you can
 * create an empty menu, and then fill in the text with Menu_SetText
 * rather than having to build a descriptor string.
 *
 * If it cannot allocate memory for the menu, it returns NULL.
 * --------------------------------------------------------------------- */
menu_ptr Menu_NewEmpty(const char *title, int numitems)
{
  unsigned int titlelen = strlen(title);
  menu_ptr menu;
  menu_item *items;
  int n;

  if (numitems < 1) return NULL;

  menu = malloc(sizeof(menu_block) + (numitems * sizeof(menu_item)));
  if (menu == NULL) return NULL;

  strncpy(menu->title, title, wimp_MAXNAME);

  /*
   * All menu items are empty, so width is determined by the title.
   * We add a little extra for submenu arrows, etc.
   */
  menu->width = (strlen(title) + 1) * 16;

  /* Standard Wimp menu settings */
  menu->titlefore = 7;
  menu->titleback = 2;
  menu->workfore  = 7;
  menu->workback  = 0;
  menu->height    = 44;
  menu->gap       = 0;

  /* The array of menu items directly follows the menu_block */
  items = (menu_item *) &(menu[1]);

  for (n = 0; n < numitems; n++)
  {
    items[n].menuflags.value = 0;
    items[n].submenu.value   = NULL;
    items[n].iconflags.value = icon_TEXT | icon_FILLED;
    items[n].iconflags.data.foreground = 7;
    items[n].icondata.text[0] = '\0';
  }

  items[numitems - 1].menuflags.data.last = TRUE;

  /*
   * If the title length is > 12 characters, make it indirected.
   * We do this here as we can still return a working menu even if
   * the memory for the title text cannot be allocated.
   */
  if (titlelen > wimp_MAXNAME)
  {
    char *titletext = malloc(titlelen + 1);
    icon_data *titleicon;

    if (titletext == NULL) return menu;

    /* We now treat the menu title as a text icon block */
    titleicon = (icon_data *)(void *)&menu->title;
    titleicon->indirecttext.buffer = titletext;
    titleicon->indirecttext.validstring = (char *) -1;
    titleicon->indirecttext.bufflen = titlelen + 1;

    /* Copy the text */
    strcpy(titletext, title);

    /* Set flag in the first item to indicate the indirected title */
    items[0].menuflags.data.indtitle = 1;
  }

  return menu;
}
