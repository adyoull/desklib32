/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.GetFlags.c
    Author:  Ben Summers
    Version: 1.00 (21 Oct 94)
    Purpose: Makes a Font menu
*/

#include <string.h>
#include <stdio.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"
#include "DeskLib:SWI.h"




  /* minimum version to use font manager font menus * 100 */
#define MIN_NEW_FONT_VERSION 307

#define NEW     0
#define OLD     1
#define UNKNOWN 2

#define XFont_CacheAddr 0x40080 + XOS_Bit
#define XFont_ListFonts 0x40091 + XOS_Bit

static int version = UNKNOWN;

static menu_ptr font_menu = 0;
static int entries;
static int last_ticked = -1;

static BOOL new_font_manager(void)
{
  if(version == UNKNOWN) {
    int v;
    if(SWI(0, 1, XFont_CacheAddr, &v) != 0 || v < MIN_NEW_FONT_VERSION) {
      version = OLD;
    } else {
      version = NEW;
    }
  }

  return version == NEW;
}

menu_ptr Menu_FontMenu(BOOL sysfont, char *tick)
{
  if(new_font_manager()) {
    return Menu_FontMenu3(sysfont, tick);
  } else {
    int l;

    if(font_menu == 0) {    /* make the font menu */
      BOOL created = FALSE;
      int count = 0;
      char name[64];

      entries = 0;

      do {
        if(SWI(4, 3, XFont_ListFonts, 0, (int)name, count, -1, 0, 0, &count) != 0)
          return 0;

        if(created == FALSE) {
          char text[128];
          if(sysfont) {
            sprintf(text, "System font,%s", name);
            entries++;
          }

          if((font_menu = Menu_New("Fonts", sysfont?text:name)) == 0)
            return 0;

          created = TRUE;
        } else {
          font_menu = Menu_Extend(font_menu, name);
        }

        entries++;

      } while(count != -1);
    }

    if(last_ticked != -1) {
      Menu_SetFlags(font_menu, last_ticked, FALSE, FALSE);
      last_ticked = -1;
    }

    if(tick != Menu_FontMenu_NOTICK) {
      if(tick == Menu_FontMenu_TICKSYSFONT) {
        last_ticked = 0;
      } else {
        for(l = 0; l < entries; l++) {
          if(strcmp(Menu_GetText(font_menu, l), tick) == 0) {
            last_ticked = l;
            break;
          }
        }
      }

      if(last_ticked != -1) {
        Menu_SetFlags(font_menu, last_ticked, TRUE, FALSE);
        last_ticked = -1;
      }
    }

    return menu_fontmenu = font_menu;

  }

  return 0;
}

extern char *Menu_FontMenuDecode(int *selection)
{
  if(new_font_manager()) {
    return Menu_FontMenuDecode3(selection);
  } else {
    return Menu_GetText(font_menu, *selection);
  }
}
