/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Pointer.Get.c
    Author:  Copyright © 2020 Stephen Fryatt
    Version: 1.00 (18 Jul 2020)
    Purpose: Get the pointer position
*/

#include "DeskLib:KernelSWIs.h"
#include "DeskLib:Pointer.h"

typedef char byte;

os_error *Pointer_GetPosition(wimp_point *position)
{
 byte         box[5]={6};
 os_error     *error = NULL;

 if (position == NULL)
  return NULL;

 error = OS_Word(osword_DEFINEPOINTERANDMOUSE,box);
 if (error != NULL)
  return error;

 position->x = box[1] + (box[2] << 8);
 position->y = box[3] + (box[4] << 8);

 return NULL;
}
