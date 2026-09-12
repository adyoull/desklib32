/*
 * File: Clear.Init.c
 * Author: Jason Howat, 16th December 1993
 *
 * Abstract: Initialise the Clear library.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

#include "DeskLib:Clear.h"

#include "ClearDefs.h"

const char *clear__creator;
unsigned clear__creatorversion;


void Clear_Init(const char *creator, unsigned version)
{
  clear__creator = creator;
  clear__creatorversion = version;
}
