/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon/Validation.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.01 (12 Dec 1992)
             1.02 (25 Oct 2007) Moved from Validation to Icon module
             1.10 (15 Mar 2025) Added Icon_FindValidationStringCommand to
                                perform more in-depth parsing of validation
                                strings, and reimplemented the existing
                                Icon_ScanValidationString through this to retain
                                existing behaviour for backwards compatibility.
                                For more background and details, see
                                https://www.riscosopen.org/forum/forums/11/topics/20292
    Purpose: Scanning of validation strings for various fields
*/

#include "DeskLib:Core.h"
#include "DeskLib:Icon.h"

extern int Icon_FindValidationStringCommand(const char *string, char tag, int *lenp)
{
  int index;

  if (string == NULL || (int)string == -1)
    return Icon_NoValidationCommand;

  tag = tag & 255;
  if (tag >= 'a' && tag <= 'z')
    tag -= 32;

  for (index=0; string[index] >= 32; index++)
  {
    char c = string[index];

    if (c >= 'a' && c <= 'z')
      c -= 32;

    if (c == tag || tag == '\0')
    {
      /* We have found the tag */
      int end = index;
      if (lenp == NULL)
        return index;

      /* They wanted to know the length as well, so hunt for the end */
      for (end = index + 1; string[end] >= 32 && string[end] != ';'; end++)
      {
        c = string[end];
        if (c == '\\')
        {
          c = string[end + 1];
          if (c <= 32)
          {
            /* Malformed validation string (should not end with an escape) */
            /* TODO - Shouldn't this set *lenp to something safe? */
            break;
          }
          end++;
        }
      }
      *lenp = end - index;
      return index;
    }

    /* No tag found, look for separator */

    for (index++; string[index] >= 32 && string[index] != ';'; index++)
    {
      c = string[index];
      if (c == '\\')
      {
        c = string[index + 1];
        if (c <= 32)
        {
          /* Malformed validation string (should not end with an escape) */
          return Icon_NoValidationCommand;
        }
        index++;
      }
    }
  }

  /* Tag has not been found */
  return Icon_NoValidationCommand;
}

extern int Icon_ScanValidationString(const char *string, char tag)
{
  int length = 0;
  int start = Icon_FindValidationStringCommand(string, tag, &length);

  /* Not found, so return zero. */

  if (start == Icon_NoValidationCommand)
    return 0;

  /* No parameter, and at the end of the string, so return zero. */

  if (length <= 1 && string[start + 1] < 32)
    return 0;

  /* Return the start of the parameter. */

  return start + 1;
}
