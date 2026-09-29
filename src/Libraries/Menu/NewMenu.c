/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.NewMenu.c
    Author:  Copyright © 1993 Shaun Blackmore and Jason Williams,
                       © 1994 Tony Houghton
                       © 1995 Neil Tarrant
    Version: 1.05 (16 May 1995)
    Purpose: Creates a new menu given a textual description string
    Mods:    24 Sep 1994  -  TH  -  Can now cope with indirected menu titles in
                                    >= RISC OS 3.1
             16 May 1995  - Neil Tarrant - malloc(1+titlelen).
*/


#include <stdlib.h>
#include <string.h>

#include "DeskLib:Core.h"
#include "DeskLib:Error.h"
#include "DeskLib:Event.h"
#include "DeskLib:Screen.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"

#include "MenuDefs.h"


/* Description format:
 *      opt   :- " "  no special options (i.e. leading spaces are skipped)
 *               "!"  ticked
 *               "~"  shaded
 *               ">"  has submenu/subwindow
 *               "-"  dotted
 *      name  :- any character except "," and "|"
 *      entry :- {opt}* name
 *      sep   :- ","  to separate normal items
 *               "|"  to separate items and place a dotted line between them
 *      descr :- entry {sep entry}*
 *
 *      EXAMPLE:
 *        ">Info, Create, Quit"
 *        "!Ticked,~Disabled|>Submenu"
 */


extern void Menu__CountItems(const char *description, unsigned int *numitems,
                             unsigned int *menuwidth)
/*  For internal use - counts the number of items in a menu description
 *  string, and also returns the widest menu item (in terms of characters)
 */
{
  const char *s;
  int  width = 0, maxwidth = 0, count = 0;
  BOOL in_option = TRUE;

  for (s = description; *s; s++)
  {
    if (*s == ',' || *s == '|')
    {
     if (width > maxwidth)
       maxwidth = width;
     count++;
     width = 0;
     in_option = TRUE;
    }
    else
    {
      if (in_option)
      {
        if(*s != '!' && *s != '>' && *s != '~' && *s != ' ')
        {
          in_option = FALSE;
          width++;
        }
      }
      else
        width++;
    }
  }

  *numitems = count + 1;                    /* Count last item and its width */

  if (width > maxwidth)
    maxwidth=width;

  *menuwidth = maxwidth + 1;       /* Add room for a gap/sublink arrows, etc */
}


extern BOOL Menu__Create(menu_item *item, const char *description, int numitems)
{
  menu_item *it;
  const char *s, *t;
  int  i, index;
  BOOL foundtext;

  s = description;
  for (i = 0; i < numitems; i++)
  {
    it = &item[i];
    it->menuflags.value = 0;
    it->submenu.value   = 0;
    it->iconflags.value = icon_TEXT | icon_FILLED;
    it->iconflags.data.foreground = 7;

    foundtext = FALSE;
    do
    {
      switch(*s++)
      {
        case '!':
          it->menuflags.data.ticked = TRUE;
          break;

        case '~':
          it->iconflags.data.shaded = TRUE;
          break;

        case '>':
          it->menuflags.data.notifysub = TRUE;  /* Ask for sublink warnings */
          it->submenu.value = 1;                /* This is sublink item '1' */
          break;

        case ' ':  /* No option */
          break;

        default:   /* Any other == start of menu item text, so don't lose it */
          s--;
          foundtext = TRUE;
          break;
      }
    } while (!foundtext);

    t = s;
    for (index = 0; ; index++)
      if (t[index] == 0 || t[index] == '|' || t[index] == ',')
        break;

    if (index <= wimp_MAXNAME)
      t = it->icondata.text;            /* Copy text directly into menu item */
    else
    {
      /* Copy text into indirected menu item */
      it->iconflags.data.indirected = TRUE;
      it->icondata.indirecttext.buffer = malloc(index + 1);
      if (it->icondata.indirecttext.buffer == NULL)
        return(Error_OutOfMemory(FALSE, "menus"));

      it->icondata.indirecttext.bufflen = index + 1;
      it->icondata.indirecttext.validstring = (char *) -1;

      t = it->icondata.indirecttext.buffer;
    }

    {
      char **cst_t = (void *)(int)&t;

      while (*s != 0 && *s != ',' && *s != '|')
        *(*cst_t)++ = *s++;

      if (index != wimp_MAXNAME)    /* Only terminate if not exactly 12 chars */
        *(*cst_t) = 0;

      if (*s++ == '|')                       /* Step over separator...        */
        it->menuflags.data.dotted = TRUE;    /* ...setting 'dotted' if needed */
    }
  }

  return(TRUE);
}



extern menu_ptr Menu_New(const char *title, const char *description)
{
  menu_ptr     menu;
  menu_item    *item;
  unsigned int maxwidth;
  unsigned int numitems;
  unsigned int titlelen;
  unsigned int trunclen;

  /*  Copy the string. If it's less than 12 characters we need it to be
   *  zero-terminated. We can let it go to 12 chars without any terminator
   *  though, so we don't need to bother terminating after the strncpy.
   */

  Menu__CountItems(description, &numitems, &maxwidth);

  titlelen = strlen(title);

  /* Make sure menu is wide enough for truncated title  */
  trunclen = titlelen > wimp_MAXNAME ? wimp_MAXNAME : titlelen;
  if (trunclen > maxwidth)
    maxwidth = trunclen;

  menu = (menu_ptr) malloc(sizeof(menu_block) + (numitems * sizeof(menu_item)));

  if (menu == NULL)
    return((menu_ptr) Error_OutOfMemory(FALSE, "menus"));

  item = (menu_item *) &(menu[1]);

  strncpy(menu->title, title, wimp_MAXNAME);

  menu->titlefore = 7;
  menu->titleback = 2;
  menu->workfore  = 7;
  menu->workback  = 0;
  menu->width     = maxwidth * 16;
  menu->height    = 44;
  menu->gap       = 0;

  if (!Menu__Create(item, description, numitems))
    return((menu_ptr) NULL);
  item[numitems - 1].menuflags.data.last = TRUE;

  /* If title > 12 chars and we've got RISC OS 3 make it indirected */
  if (titlelen > wimp_MAXNAME && event_wimpversion >= 310)
  {
    char *titledata;
    icon_data *titleicon;
    menu_item *firstitem;

    /* Set up data as for indirected text icon */
    if (titledata = malloc(1+titlelen), !titledata)	/* +1 courtesy of Neil Tarrant */
      return (menu);
    strcpy(titledata,title);
    titleicon = (icon_data *)(void *)&menu->title;
    titleicon->indirecttext.buffer = titledata;
    titleicon->indirecttext.validstring = (char *) -1;
    titleicon->indirecttext.bufflen = titlelen;

    /* Set flag in first item to inicate indirected title */
    firstitem = (menu_item *) ((int) menu + sizeof(menu_block));
    firstitem->menuflags.data.indtitle = 1;

    /* Make sure it is wide enough for indirected title  */
    if (titlelen > maxwidth)
      maxwidth = strlen(title);
  }
  return(menu);
}

