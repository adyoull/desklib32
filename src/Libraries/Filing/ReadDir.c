/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Filing.ReadDir.c
    Author:  Copyright © 1994, 1995 Sergio Monesi
    Version: 1.01 (6 Jun 1995)
    Purpose: Reads directory content easily
    Mods:    6 June 1995 - changed procedure names to a more
                           DeskLib-compliant style
             07 Jun 1995 - JS - moved definition of __outofmemory to inside
                                function when compiling a DLL - otherwise
                                cc complains about 'RISC OS (or other reentrant
                                module has static init. to data 'x$dataseg''.

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "DeskLib:Core.h"
#include "DeskLib:Filing.h"
#include "DeskLib:Error.h"

#include "Errors.h"

/* Note: this functions use malloc to allocate memory. If you want to use
         a different way (mem, flex, heap, etc.) change the lines below
         but be careful about the way in which parameters are passed. */

#define MALLOC(size) malloc((size))
#define FREE(ptr)    free((ptr))


error_STATIC( __outofmemory, filing_ERROR_OutOfMemory, "Not enough memory");


os_error *Filing_OpenDir(const char *dirname, filing_dirdata *dirdata,
                         int bufsize, filing_readdirtype type)
{
 os_error *er;
 filing_objtype objtype;

 dirdata->buf=NULL;
 dirdata->dirname=NULL;   /* to avoid problems with free in Filing_CloseDir */

 if (er=Filing_ReadCatalogue(dirname,&objtype,NULL,NULL,NULL,NULL,NULL),er!=NULL)
   return er;
 if (objtype==1) {
   error_global.errnum=filing_ERROR_DirectoryIsFile;
   sprintf(error_global.errmess,"Directory '%s' is actually a file!",dirname);
   return &error_global;
 }
 else if (objtype==0) {
   error_global.errnum=filing_ERROR_DirectoryDoesntExist;
   sprintf(error_global.errmess,"Directory '%s' doesn't exist!",dirname);
   return &error_global;
 }

 dirdata->dirname=MALLOC(strlen(dirname)+1);
 if (dirdata->dirname==NULL)
   return __outofmemory;
 strcpy(dirdata->dirname,dirname);

 dirdata->buf=MALLOC(bufsize);
 if (dirdata->buf==NULL)
   return __outofmemory;

 dirdata->size=bufsize;
 dirdata->type=type;
 dirdata->offset=0;
 dirdata->read=0;
 dirdata->match=NULL;  /* this is currently always set to NULL so that all */
                       /* the files in the directory are read (NULL="*")   */

 /* adesso bisogna iniziare a leggere la directory... */
 /* ...no, forse e` inutile... per adesso non lo faccio... */

/* switch(type) {
   case readdirtype_DIRENTRY:
    break;

 }*/

 return NULL;
}

void *Filing_ReadDir(filing_dirdata *dirdata)
{
 os_error *er;

 if (dirdata->read==0) {    /* no more objects read in the buffer */
   if (dirdata->offset==-1) {    /* no more objects to read */
     return NULL;
   }

   dirdata->read=255;   /* objects to read */

   switch(dirdata->type) {
     case readdirtype_DIRENTRY:
      er=Filing_ReadDirEntry(dirdata->dirname,(filing_direntry *)dirdata->buf,
                             &dirdata->read,&dirdata->offset,
                             dirdata->size,dirdata->match);
      if (er!=NULL) {
        return NULL;
      }
      if (dirdata->read==0) {
        dirdata->offset=-1;
        return NULL;
      }
      dirdata->read--;
      dirdata->act.direntry=(filing_direntry *)dirdata->buf;
      break;
     case readdirtype_FULLDIRENTRY:
      er=Filing_ReadFullDirEntry(dirdata->dirname,(filing_fulldirentry *)dirdata->buf,
                                 &dirdata->read,&dirdata->offset,
                                 dirdata->size,dirdata->match);
      if (er!=NULL) {
        return NULL;
      }
      if (dirdata->read==0) {
        dirdata->offset=-1;
        return NULL;
      }
      dirdata->read--;
      dirdata->act.fulldirentry=(filing_fulldirentry *)dirdata->buf;
      break;
     case readdirtype_NAMEONLY:
      er=Filing_ReadDirNames(dirdata->dirname,(char *)dirdata->buf,
                             &dirdata->read,&dirdata->offset,
                             dirdata->size,dirdata->match);
      if (er!=NULL) {
        return NULL;
      }
      if (dirdata->read==0) {
        dirdata->offset=-1;
        return NULL;
      }
      dirdata->read--;
      dirdata->act.name=(char *)dirdata->buf;
      break;
   }

 }
 else {     /* some object is ready in buffer */
   switch(dirdata->type) {
     case readdirtype_DIRENTRY:
      dirdata->read--;
      dirdata->act.direntry=(filing_direntry *)(((int)(&(dirdata->act.direntry->name))+
                                               strlen(dirdata->act.direntry->name)+4)&~3);
      break;
     case readdirtype_FULLDIRENTRY:
      dirdata->read--;
      dirdata->act.fulldirentry=(filing_fulldirentry *)(((int)(&(dirdata->act.fulldirentry->name))+
                                                       strlen(dirdata->act.fulldirentry->name)+4)&~3);
      break;
     case readdirtype_NAMEONLY:
      dirdata->read--;
/*      dirdata->act.name=(char *)(((int)(&(dirdata->act.name))+strlen(dirdata->act.name)+4)&~3);*/
      dirdata->act.name=(char *)(dirdata->act.name+strlen(dirdata->act.name)+1);
      break;
   }

 }

 return (void *)dirdata->act.act;
}

BOOL Filing_CloseDir(filing_dirdata *dirdata)
{
 BOOL ret=TRUE;

 if (dirdata->buf!=NULL) {
   FREE(dirdata->buf);
 }
 else {
   ret=FALSE;
 }

 if (dirdata->dirname!=NULL) {
   FREE(dirdata->dirname);
 }
 else {
   ret=FALSE;
 }

 return ret;
}
