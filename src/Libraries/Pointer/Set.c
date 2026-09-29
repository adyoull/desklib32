/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Pointer.Set.c
    Author:  Copyright © 2020 Stephen Fryatt
    Version: 1.00 (16 Jun 2020)
    Purpose: Set the pointer position
*/

#include "DeskLib:KernelSWIs.h"
#include "DeskLib:Pointer.h"

typedef char byte;

os_error *Pointer_SetPosition(wimp_point position)
{
 byte         box[5]={3};

 box[1] = (position.x & 0x00ff);
 box[2] = (position.x & 0xff00) >> 8;
 box[3] = (position.y & 0x00ff);
 box[4] = (position.y & 0xff00) >> 8;

 return OS_Word(osword_DEFINEPOINTERANDMOUSE,box);
}
