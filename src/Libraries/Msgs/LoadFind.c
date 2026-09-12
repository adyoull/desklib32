/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Msgs.LoadFind.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (08 Apr 1992)
    Purpose: MessageTrans-like message handling functions.
             (If you want MessageTrans, use the SWI interface, if you want
             high-level message handling, use this code...)

             NOTE: Although it may seem as if this chunk of code could be
             split into smaller chunks, I don't see much point, as the
             functions are all inter-dependent - there seems to be no point
             in trying to use Msgs_Load without using the other functions
             defined here (even using Msgs_printf calls Msgs_Lookup)
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "DeskLib:Core.h"
#include "DeskLib:Error.h"
#include "DeskLib:Resource.h"
#include "DeskLib:TextFile.h"
#include "DeskLib:Msgs.h"

#include "MsgsDefs.h"


#undef getc
#undef feof

msgdefptr msgs_grouplist = NULL;





extern BOOL Msgs__MatchToken(const char *tag1, const char *tag2, BOOL wcallowed)
{
  int loop = 0;

  if (!wcallowed)   /* Don't allow wildcard matching when inserting records! */
    return(MatchTokenI(tag1, tag2));

  while (loop < 9 && tag1[loop] != '\0' && tag2[loop] != '\0')
  {
    if (tag1[loop] == '*' || tag2[loop] == '*')
      return(TRUE);

    if (tag1[loop] != tag2[loop])
      return(FALSE);
    loop++;
  }

  if (tag1[loop] == '*' || tag2[loop] == '*')
    return(TRUE);

  if (tag1[loop] != tag2[loop])
    return(FALSE);

  return(TRUE);
}



extern msgdefptr Msgs__Find(msgdefptr *liststart, const char *tag,
                            BOOL create, BOOL wcallowed)
/* Find the given tag in either the group list or a message list (list header
 * passed in as *liststart). If not found, possibly create a new record
 * to hold this tag.
 * Returns ptr to the found/created record, or NULL if not found/created
 */
{
  msgdefptr ptr;

  ptr = *liststart;
  while (ptr != NULL)
  {
    if (Msgs__MatchToken(tag, ptr->tag, wcallowed))
      return(ptr);
    ptr = ptr->next;
  }

  if (!create)                     /* not found & don't want us to create it */
    return(NULL);

  ptr = (msgdefptr) malloc(sizeof(msg_def));             /* create new group */
  if (ptr != NULL)
  {
    ptr->next = *liststart;                   /* link in to head of msg list */
    *liststart = ptr;

    strncpy(ptr->tag, tag, 9);
    ptr->tag[9] = 0;
    ptr->data.taglist = NULL;         /* equivalent to ptr->data.text = NULL */
  }

  return(ptr);
}


static void CopyMessage(char *s1, char *s2, int maxlength);

extern BOOL Msgs_Lookup(const char *tag, char *result, int maxlength)
{
  msgdefptr    ptr;
  char         grouptag[10], msgtag[10];
  register int index, index2;

  result[0] = '\0';
  for(index = 0; index < 10; index++)
  {
    grouptag[index] = 0;
    msgtag[index] = 0;
  }

  index = 0;
  while (index < 9)
  {
    if (tag[index] == '.')
      break;

    grouptag[index] = tag[index];
    index++;
  }

  if (tag[index] != '.')   /* invalid tag  */
    return(FALSE);

  index++;                 /* Skip the '.' */
  index2 = 0;
  while (index2 < 9)
  {
    if (tag[index] == ':' || tag[index] == '\0')
      break;

    msgtag[index2++] = tag[index++];
  }

  /* Find the group the message is in - use 2 passes, once with wildcards
   * disabled, and then a second time with them enabled, so that order of
   * definitiion is less important.
   */
  ptr = Msgs__Find(&msgs_grouplist, grouptag, FALSE, FALSE);
  if (ptr == NULL)  /* not found, so scan again */
    ptr = Msgs__Find(&msgs_grouplist, grouptag, FALSE, TRUE);


  /* Now, do the same  passes on the group's message list to find the actual
   * message.
   */
  if (ptr != NULL)
  {
    msgdefptr ptr2;

    ptr2 = Msgs__Find(&(ptr->data.taglist), msgtag, FALSE, FALSE);
    if (ptr2 == NULL)  /* not found, so scan again */
      ptr = Msgs__Find(&(ptr->data.taglist), msgtag, FALSE, TRUE);
    else
      ptr = ptr2;
  }

  if (ptr == NULL)  /* still not found - have we been supplied with default? */
  {
    if (tag[index] != '\0')
    {
      strncpy(result, &(tag[index + 1]), maxlength);
      result[maxlength] = 0;
      return(TRUE);                                /* return default       */
    }
    return(FALSE);                                 /* Failed completely    */
  }

  CopyMessage(result, ptr->data.text, maxlength);  /* Return found message */
  return(TRUE);
}



static void CopyMessage(char *dest, char *src, int maxlength)
{
  register int  from = 0, to = 0, i;
  register char c;
  char includetag[24];

  while (from < maxlength - 1)
  {
    c = src[from];
    if (c == 0)
      break;

    if (c == '<')
    {
      if (src[from + 1] == '>')
      {
        from++;                     /* found "<>" - skip to output "<" only */
        dest[to++] = '<';
      }
      else
      {
        from++;    /* Search from < to > to extract the include message tag */
        i = 0;
        while (c != '>')
        {
          c = src[from++];
          includetag[i++] = c;
        }
        includetag[i - 1] = '\0';
        from--;                         /* whoa boy! back up one character! */

        /*  Found include tag, so recurse on it to insert it into final
         *  message at this point
         */

        Msgs_Lookup(includetag, &(dest[to]), maxlength - to);

        /*  And now, we must search ahead from OUR "to" index (the start
         *  of the section inserted) until we find the new end-of-string
         */
        while(dest[to] != '\0')
          to++;
      }
    }
    else
      dest[to++] = c;

    from++;
  }

  dest[to] = '\0';   /* Add string terminator */
}



static BOOL AddMessage(char *grouptag, char *msgtag, char *message)
{
  char *msg;
  msgdefptr ptr;

  msg = (char *) malloc(strlen(message) + 1);
  if (msg == NULL)  return(Error_OutOfMemory(FALSE, "Messages"));
  strcpy(msg, message);

  ptr = Msgs__Find(&msgs_grouplist, grouptag, TRUE, FALSE);
  if (ptr != NULL)
    ptr = Msgs__Find(&(ptr->data.taglist), msgtag, TRUE, FALSE);

  if (ptr != NULL)
  {
    if (ptr->data.text != NULL)
      free(ptr->data.text);                  /* Overwrite previous occurence */

    ptr->data.text = msg;
    return(TRUE);
  }
  return(FALSE);
}



extern BOOL Msgs_LoadFile(const char *leafname)
/*  Merges the given messages file into the current list of messages
 *  Messages with the same group.msg tag will be overwritten by the
 *  new messages coming in from the file.
 *
 *  Expects lines of the form:
 *    group.msg: message text continuing to newline
 *  Leading spaces are ignored
 */
{
  char filename[60];
  char groupname[10], tagname[10];
  char buffer[1024];
  FILE *infile;
  int  loop;
  int  ch;

  strcpy(filename, resource_pathname);
  strcat(filename, leafname);

  infile = fopen(filename, "r");
  if (infile == NULL)
    return(FALSE);

  while (!feof(infile))
  {
    for (loop = 0; loop < 10; loop++)
    {
      groupname[loop] = 0;
      tagname[loop] = 0;
    }

    while (TRUE) /* Skip comments */
    {
      TextFile_SkipBlanks(infile);

      ch = getc(infile);
      ungetc(ch, infile);

      if (ch == '#')
        TextFile_ReadToDelimiter(infile, '\n', buffer, 510);
      else
        break;
    }

    TextFile_ReadToDelimiter(infile, '.', groupname, 10);
    TextFile_ReadToDelimiter(infile, ':', tagname, 10);
    TextFile_ReadToDelimiter(infile, '\n', buffer, 510);

    if (!feof(infile) && !AddMessage(groupname, tagname, buffer))
    {
      fclose(infile);
      return(FALSE);
    }
  }

  fclose(infile);
  return(TRUE);
}
