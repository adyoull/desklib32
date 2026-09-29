/*
 * This file is part of DeskLib, the C library for RISC OS.
 * Please see accompanying documentation for terms of use.
 *
 *       http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module:  Environment
 * File:    OSVersion.c
 * Author:  Adam Richardson
 * Purpose: Return OS version info.
 *
 * Version History
 * 31/10/2007: Creation
 *
 */

#include "DeskLib:Environment.h"
#include "DeskLib:KernelSWIs.h"
#include <stdio.h>

int Environment_OSVersion(void)
{
  int version, dummy;

  OS_Byte(osbyte_READOSIDENTIFIER, 0, 255, &version, &dummy);

  switch (version)
  {
    case 0xA0:
      version = 100;
      break;
    case 0xA1:
      version = 200;
      break;
    case 0xA2:
      version = 200;
      break;
    case 0xA3:
      version = 300;
      break;
    case 0xA4:
      version = 300;
      break;
    case 0xA5:
      version = 300;
      break;
    case 0xA6:
      version = 300;
      break;
    case 0xA7:
      version = 300;
      break;
    case 0xA8:
      version = 400;
      break;
    case 0xA9:
      version = 430;
      break;
    case 0xAA:
      version = 500;
      break;
    default:
      version = 0;
      break;
  }

  return version;
}
