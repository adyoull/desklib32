/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.LoadFile.c
    Author:  Copyright © 1992 Jason Williams
             Thanks to John Winters for supplying the code that I hacked
             changed, hacked, rewrote, and then wrote again from scratch!
    Version: 1.12 (02 Mar 1994)
    Purpose: Loading, cacheing, and retrieval of window templates
             Now correctly loads templates with outline fonts in them
*/


#include "TempDefs.h"


/* Not intended for user consumption */
linklist_header template_list         = {NULL, NULL};
font_array      *template_fontarray   = (font_array *) -1;


static void ReadHeader(const char *filename)
/* Find out how many templates, names, and sizes */
{
  FILE *filehandle;
  int  buffer[10];  /* 40 byte file buffer */
  int  i, numtemplates;
  char *s;
  template_record *temprec;

  filehandle = fopen(filename, "rb");
  if (filehandle == NULL)
    Error_ReportFatalInternal(ERR7, ERRMESS7);

  if (fread(buffer, HEADER_SIZE, 1, filehandle) != 1)
  {
    fclose(filehandle);
    Error_ReportFatalInternal(ERR4, ERRMESS4);
  }

  numtemplates = 0;
  while(TRUE)
  {
    if (fread(&buffer[0], INDEX_SIZE, 1, filehandle) != 1)
    {
      fclose(filehandle);
      Error_ReportFatalInternal(ERR5, ERRMESS5);
    }

    if (buffer[0] == 0) /* template list terminator */
      break;

    numtemplates++;
    temprec = (template_record *) malloc(sizeof(template_record));

    i = 0;
    s = (char *) &buffer[3];
    buffer[6] = 0;  /* Ensure terminators after 12-bytes of name */
    while (TRUE)
    {
      if (s[i] < 32)
      {
        temprec->identifier[i] = '\0';
        break;
      }
      temprec->identifier[i] = s[i];

      i++;
    }

    temprec->dataoffset   = buffer[0];
    temprec->templatesize = buffer[1];     /* size needed to load template */
    temprec->windowdef    = NULL;
    temprec->indirectdata = NULL;
    LinkList_Init(&(temprec->header));
    LinkList_AddToTail(&template_list, &(temprec->header));
  }

  fclose(filehandle);
}



extern void Template_LoadFile(const char *leafname)
{
  template_record *tptr;
  char            filename[60];
  char            tempname[20];
  template_block  tblock;
  char            tempdata[5192];        /* temp. buffer for indirected data */

  strcpy(filename, resource_pathname);
  strcat(filename, leafname);

  /* Remember the end of the template list - this will be NULL for the
   * first call to Template_LoadFile, and non-NULL for subsequent calls.
   */
  tptr = (template_record *) template_list.previous;

  ReadHeader(filename);     /* Find out how many templates, names, and sizes */
  Wimp_OpenTemplate(filename);

  /* If tptr is NULL, then this is the first time we have loaded any templates,
   * so get the head of the list to find the first blank record supplied by
   * ReadHeader().  If tptr is not NULL, then we are appending templates, so
   * get the next record after tptr, as this will be the first blank record
   * just added by ReadHeader().
   */
  if (tptr == NULL)
    tptr = (template_record *) template_list.next;
  else
    tptr = (template_record *) tptr->header.next;

  while (tptr != NULL)
  {
    tptr->windowdef = (window_block *) malloc(tptr->templatesize);
    if (tptr->windowdef == NULL)
    {
      Wimp_CloseTemplate();
      Error_ReportFatalInternal(ERR3, ERRMESS3);
    }

/*  Now, read template once to determine the indirected data size needed.
 *  I tried many different methods to do this, but thrashing in all the icons
 *  and counting their indirect text buffer sizes, and then thrashing in
 *  all of the validation strings and counting their lengths was slow, nasty,
 *  and didn't seem very reliable.
 *  This way also produces a much smaller code lump to include in DeskLib
 *  It works very nicely, so long as your indirected data doesn't expand
 *  to more than 5kB (unlikely unless you are being very antisocial towards
 *  Mr. Wimp)
 */
    tblock.buffer   = tptr->windowdef;
    tblock.workfree = tempdata;
    tblock.workend  = tblock.workfree + 5188;

    /* The following line has been changed. If the font array was not passed
     * in here as well, the fonts were not handled at all...
     */
    tblock.font     = template_fontarray;   /* was  (font_array *) -1 */
    strcpy(tempname, tptr->identifier);
    tblock.name     = tempname;
    tblock.index    = 0;
    Wimp_LoadTemplate(&tblock);

    tptr->indirectsize = (tblock.workfree - tempdata) + 4;
    tptr->indirectdata = (char *) malloc(tptr->indirectsize);
    if (tptr->indirectdata == NULL)
    {
      Wimp_CloseTemplate();
      Error_ReportFatalInternal(ERR3, ERRMESS3);
    }

/*  Now, do a Template_Load to actually load in the template. Should be nice
 *  and quick, as the previous load should have cached the data in a buffer
 *  so more disc reading is unlikely. (Though don't quote me on that! ;-)
 */
    tblock.buffer   = tptr->windowdef;
    tblock.workfree = tptr->indirectdata;
    tblock.workend  = tblock.workfree + tptr->indirectsize;
    tblock.font     = template_fontarray;
    strcpy(tempname, tptr->identifier);
    tblock.name     = tempname;
    tblock.index    = 0;
    Wimp_LoadTemplate(&tblock);

    tptr = (template_record *) tptr->header.next;
  }

  Wimp_CloseTemplate();
}
