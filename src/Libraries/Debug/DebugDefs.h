/*
 * This file is part of DeskLib, the C library for RISC OS.
 * Please see accompanying documentation for terms of use.
 *
 *       http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module:  Debug
 * File:    DebugDefs.h
 * Author:  Adam Richardson
 * Purpose: Function prototypes for separate debugging methods
 *
 * Version History
 * 18/08/2007: Creation
 *
 */

#ifndef __Debug_Defs_h
#define __Debug_Defs_h

#include <stdio.h>
#include "DeskLib:Core.h"

void Debug__Assert(const char *expression, char *sourcefile, int line);

BOOL Debug_InitialisePipetype(void);
void Debug_PrintPipetype(const char *text);

BOOL Debug_InitialiseReporter(void);
void Debug_PrintReporter(const char *text);

void Debug_PrintStderr(const char *text);

BOOL Debug_InitialiseUniqueFile(void);
void Debug_PrintUniqueFile(const char *text);

BOOL Debug_InitialiseUniquePipe(void);
static FILE *Debug__OpenPipeFile(void);
void Debug_PrintUniquePipe(const char *text);

#endif
