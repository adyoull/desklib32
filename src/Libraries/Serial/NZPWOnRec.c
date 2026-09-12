/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Serial.c.NZPWOnRec
    Author:  Copyright © 1993 Jason Howat
    Version: 1.00 (21 Nov 1993)
    Purpose: Provide a handler to modify a pollword on reception of a
             character on the serial port.
*/

#include <stdlib.h>
#include <string.h>

#include "DeskLib:Core.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Error.h"
#include "DeskLib:Serial.h"


static unsigned *serial__RMAblock = NULL;
static void *serial__pollword = NULL;
static const unsigned serial__insvcode[] = {0xe92d4002,
                                            0xe3c11102,
                                            0xe3510001,
                                            0x059f1004,
                                            0x058c1000,
                                            0xe8fd8002};

void Serial__NZPWORRemove(void);


void Serial_NonZeroPollWordOnReceive(int pollword_value,
                                          void *pollword)
{
  Error_CheckFatal(SWI(4,3, SWI_OS_Module + XOS_Bit, 6, 0, 0, 32,
                                      /* TO */ NULL, NULL, &serial__RMAblock));

  if(pollword_value == 0)
    pollword_value = 1;

  memcpy(serial__RMAblock, serial__insvcode, 24);
  serial__RMAblock[6] = pollword_value;

  Error_CheckFatal(SWI(3,0, SWI_OS_Claim + SWI_XOS_Bit, 0x14, serial__RMAblock,
                                                              pollword));
  serial__pollword = pollword;

  atexit(Serial__NZPWORRemove);
}

void Serial__NZPWORRemove(void)
{
  Error_CheckFatal(SWI(3,0, SWI_OS_Release + SWI_XOS_Bit,
                                     0x14, serial__RMAblock, serial__pollword));
  Error_CheckFatal(SWI(3,0, SWI_OS_Module + SWI_XOS_Bit, 7, 0,
                                                             serial__RMAblock));
}
