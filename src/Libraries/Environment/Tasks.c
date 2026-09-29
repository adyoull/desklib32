/*
 * This file is part of DeskLib, the C library for RISC OS.
 * Please see accompanying documentation for terms of use.
 *
 *       http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module:  Environment
 * File:    Tasks.c
 * Author:  Adam Richardson
 * Purpose: Find active tasks; check if a task is running; find taskname from handle.
 *
 * Version History
 * 18/08/2007: Creation
 *
 */

#include "DeskLib:Environment.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Error.h"
#include "DeskLib:Str.h"
#include "DeskLib:Wimp.h"        /* For task_handle */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  unsigned int handle;
  char *name;
  unsigned int slotsize;
  unsigned int flags;
} enumeratetasks__block;

BOOL Environment_TaskIsActive(const char *appname)
{
  BOOL returnvalue = FALSE;
  int size;
  int seq = 0;
  enumeratetasks__block data;

  size = sizeof(enumeratetasks__block);

  while ((seq >= 0) && !returnvalue) /* SWI returns -ve seq if no more entries */
  {
    Error_CheckFatal(SWI(3, 1, SWI_TaskManager_EnumerateTasks, seq, &data, size, &seq));
    if (!strcmpcr(appname, data.name)) returnvalue = TRUE; /* skip out of loop if entry found */
  }

  return returnvalue;
}

os_error *Environment_TaskList(char ***ptrlist, int *numtasks)
{
  char **list;
  os_error *error;
  enumeratetasks__block data;
  int i, j, size, seq = 0;
  int totaltasks = -1; /* -1 because in while loop below it'll always end up one too big otherwise */

  size = sizeof(enumeratetasks__block);

  /* First, find number of tasks to list */
  while (seq >= 0) /* SWI returns -ve seq if no more entries */
  {
    error = SWI(3, 1, SWI_TaskManager_EnumerateTasks, seq, &data, size, &seq);
    if (Error_CheckSilent(error))
      return error;

    totaltasks++;
  }

  /* Allocate space for a list of pointers long enough for all tasks */
  list = malloc(totaltasks * sizeof(char *));
  if (!list)
  {
    error_global.errnum = 1;
    snprintf(error_global.errmess, sizeof(error_global.errmess), "Unable to allocate memory in Environment_TaskList");
    return &error_global;
  }

  seq = 0; /* Reset SWI ticker */
  for (i = 0; i < totaltasks; i++)
  {
    Error_CheckFatal(SWI(3, 1, SWI_TaskManager_EnumerateTasks, seq, &data, size, &seq));

    list[i] = malloc(strlencr(data.name) + 1); /* Allocate length of string + terminator */
    if (!list[i])
    {
      error_global.errnum = 1;
      snprintf(error_global.errmess, sizeof(error_global.errmess), "Unable to allocate memory in Environment_TaskList");
      for (j = 0; j < i; j++)
        free(list[i]);

      free(list);

      return &error_global;
    }
    strcpycr(list[i], data.name); /* Don't need strNcpy as correct length allocated dynamically*/
  }

  *ptrlist = list;
  *numtasks = totaltasks;

  return error;
}

void Environment_TaskListFree(char **ptrlist, int numtasks)
{
  int i;

  /* Free the strings pointed to in the pointer list */
  for (i = 0; i < numtasks; i++)
    if (ptrlist[i]) free(ptrlist[i]);

  /* Free the pointer list */
  if (ptrlist) free(ptrlist);
}

os_error *Environment_TaskNameFromHandle(task_handle task, char *buf, int bufsize)
{
  char *result = 0;
  os_error *swierr;

  swierr = SWI(1, 1, SWI_TaskManager_TaskNameFromHandle, task, &result);

  strncpycr(buf, result, bufsize - 1);
  buf[bufsize - 1] = '\0';

  return swierr;
}

os_error *Environment_TaskInfo(const char *appname, task_info *taskinfo)
{
  BOOL foundtask = FALSE, returnvalue = FALSE;
  int size;
  int seq = 0;
  enumeratetasks__block data;
  os_error *error;

  size = sizeof(enumeratetasks__block);

  while ((seq >= 0) && !foundtask) /* SWI returns -ve seq if no more entries */
  {
    error = SWI(3, 1, SWI_TaskManager_EnumerateTasks, seq, &data, size, &seq);
    if (!strcmpcr(appname, data.name)) foundtask = TRUE; /* skip out of loop when entry found */
  }


  if (taskinfo != NULL) /* Just in case value passed is garbage */
  {
    if (foundtask) /* In case user asked for info about a non-existent task */
    {
      taskinfo->handle   = data.handle;
      taskinfo->slotsize = data.slotsize;

      if (data.flags & 1)
        taskinfo->ismodule = TRUE;
      else
        taskinfo->ismodule = FALSE;

      if (data.flags & 2)
        taskinfo->candragbar = TRUE;
      else
        taskinfo->candragbar = FALSE;
    }
    else
    {
      /* Set some non-crazy default values, in case the user tries
         to use them even when the function has returned an error */
      taskinfo->handle = taskinfo->slotsize = 1;
      taskinfo->ismodule = taskinfo->candragbar = FALSE;

      /* Return an error */
      error_global.errnum = 1;
      snprintf(error_global.errmess, sizeof(error_global.errmess), "Task named '%s' not found", appname);
      error = &error_global;
    }
  }
  else
  {
    /* taskinfo parameter not passed correctly */
    error_global.errnum = 1;
    snprintf(error_global.errmess, sizeof(error_global.errmess), "Invalid parameter passed to Environment_TaskInfo function.");
    error = &error_global;
  }

  return error;
}

