/*
 * File: Url.Encode.c
 * Author: Andrew Sidwell, 23rd December 2002
 *
 * Abstract: Encode a string so it's ready for inclusion into an URL.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */
#include "DeskLib:Core.h"
#include "DeskLib:Url.h"

/* -------------------------------------------- takkaria, 2002-09-09 ---
 * Encode a set of characters ready for inclusion into an URL.
 *
 * The method we use for this is simple.  We employ a two-pass loop so
 * we first check how much space we need, and then if we have enough,
 * the second pass is carried out, which is where the work is done.
 * --------------------------------------------------------------------- */
BOOL Url_Encode(const char *toencode, char *encoded_buf, int *bufsize)
{
  const char *hex = "0123456789abcdef";
  char *encoded = encoded_buf;
  int okay = TRUE;
  const char *encoding;
  int counter;
  int pass = 0;

  /* Do two passes */
  for (pass = 0; pass < 2; pass++)
  {
    /* Reset these */
    counter = 0;
    encoding = toencode;

    /* Cycle through the string */
    while (*encoding)
    {
      /* Reset */
      okay = TRUE;

      /* Do ranges */
      if ((*encoding <=  31) ||
         ((*encoding >= 128) && (*encoding <= 255)) ||
         ((*encoding >=  59) && (*encoding <=  64)) ||
         ((*encoding >=  34) && (*encoding <=  38)) ||
         ((*encoding >=  91) && (*encoding <=  96)) ||
         ((*encoding >= 123) && (*encoding <= 126)))
      {
        okay = FALSE;
      }
      else
      {
        /* Do individual values */
        switch (*encoding)
        {
          case 32:
          case 43:
          case 44:
          case 47:
          {
            okay = FALSE;
          }
        }
      }

      /* Check */
      if (!okay)
      {
        /* If we are encoding to somewhere... */
        if (pass == 1)
        {
          *(encoded++) = '%';
          *(encoded++) = hex[(*encoding / 16) % 16];
          *(encoded++) = hex[*encoding % 16];
        }

        /* Increase the needed size */
        counter += 3;
      }
      /* Otherwise, just advance our pointer */
      else
      {
        /* If we are encoding to somewhere, just set the next char */
        if (pass == 1) *(encoded++) = *encoding;

        /* Increase the needed size */
        counter++;
      }

      /* Advance the read point */
      encoding++;
    }

    /* We need one extra, for the terminator */
    counter++;

    /* Has the size been requested? */
    if (*bufsize == 0)
    {
      /* Set the size needed */
      *bufsize = counter;

      /* We've not filled in the URL (obviously) */
      return (FALSE);
    }
    else if (*bufsize < counter)
    {
      /* We've not got enough space */
      *bufsize = counter;

      /* Indicate this */
      return (FALSE);
    }
  }

  /* Terminate */
  *encoded = '\0';

  /* Done */
  return (TRUE);
}
