/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Kbd.GetMods.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (04 Mar 1994)
    Purpose: Get the state of all modifier keys in one go.
*/

#include "DeskLib:Kbd.h"


extern kbd_modifiers Kbd_GetModifiers(BOOL detailed)
{
  kbd_modifiers mods = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  mods.alt   = Kbd_KeyDown(inkey_ALT);
  mods.ctrl  = Kbd_KeyDown(inkey_CTRL);
  mods.shift = Kbd_KeyDown(inkey_SHIFT);

  if (detailed)
  {
    mods.left_alt    = Kbd_KeyDown(inkey_LALT);
    mods.left_ctrl   = Kbd_KeyDown(inkey_LCTRL);
    mods.left_shift  = Kbd_KeyDown(inkey_LSHIFT);

    mods.right_alt   = Kbd_KeyDown(inkey_RALT);
    mods.right_ctrl  = Kbd_KeyDown(inkey_RCTRL);
    mods.right_shift = Kbd_KeyDown(inkey_RSHIFT);
  }

  return mods;
}

