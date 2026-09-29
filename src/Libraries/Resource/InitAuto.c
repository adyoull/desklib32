/*
 * This file is part of DeskLib, the C library for RISC OS.
 * Please see accompanying documentation for terms of use.
 *
 *       http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module:  Resource
 * File:    InitAuto.c
 * Author:  Adam Richardson
 * Purpose: Simpler (for the user) initialisation of resource_pathname
 *
 * Version History
 * 01/09/2007: Creation
 *
 */


#include <string.h>
#include <stdio.h>
#include "DeskLib:KernelSWIs.h"
#include "DeskLib:Resource.h"
#include "DeskLib:Str.h"
#include "DeskLib:File.h"
#include "DeskLib:Event.h"       /* For event_taskname declaration */

BOOL resource__autoinited = FALSE;

BOOL Resource_InitialiseAuto(void)
{
  char buffer[100];
  BOOL returnvalue = FALSE;

  /* Check for a name in event_taskname, otherwise give up and return FALSE */
  if ((strcmp(event_taskname, "")) && (!resource__autoinited))
  {
    /* Check for AppName$Dir */
    snprintf(buffer, sizeof(buffer), "%s$Dir", event_taskname);
    if (OS_ReadVarVal(buffer, 0, 0))
    {
      returnvalue = TRUE;
      snprintf(resource_pathname, sizeof(resource_pathname), "<%s$Dir>.", event_taskname);
    }

    /* Check for AppName$Dir.Resources */
    snprintf(buffer, sizeof(buffer), "<%s$Dir>.Resources", event_taskname);
    if (File_IsDirectory(buffer))
    {
      returnvalue = TRUE;
      snprintf(resource_pathname, sizeof(resource_pathname), "<%s$Dir>.Resources.", event_taskname);
    }

    /* Check for AppName$Path */
    snprintf(buffer, sizeof(buffer), "%s$Path", event_taskname);
    if (OS_ReadVarVal(buffer, 0, 0))
    {
      returnvalue = TRUE;
      snprintf(resource_pathname, sizeof(resource_pathname), "%s:", event_taskname);
    }

    /* Check for AppNameRes$Path */
    snprintf(buffer, sizeof(buffer), "%sRes$Path", event_taskname);
    if (OS_ReadVarVal(buffer, 0, 0))
    {
      returnvalue = TRUE;
      snprintf(resource_pathname, sizeof(resource_pathname), "%sRes:", event_taskname);
    }

    resource__autoinited = TRUE;
  }

  return returnvalue;
}
