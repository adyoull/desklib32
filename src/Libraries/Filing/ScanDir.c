/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Filing.ScanDir.c
    Author:  Copyright © 1995 Sergio Monesi
    Version: 1.01 (27 Aug 1995)
    Purpose: Scans a directory tree calling the specified functions.
*/

#include <stdio.h>
#include <string.h>
#include "DeskLib:Core.h"
#include "DeskLib:Filing.h"

#define BUF__SIZE 2048      /* size of the buffer where the directory entries are read every time */

typedef struct {
  Filing_ScanDir_StartDir *startdir;
  Filing_ScanDir_FoundFile *foundfile;
  Filing_ScanDir_EndDir *enddir;
} scandir_funcs;

static os_error *Filing__ScanDirRec(const char *dirname, filing_fulldirentry *dirdata, scandir_funcs *funcs)
{
 int readnum=0, offset=0;
 char direntries[BUF__SIZE];
 os_error *er;
 filing_fulldirentry *actptr = NULL;

 if (funcs->startdir!=NULL) {
   if (er=funcs->startdir(dirname,dirdata),er!=NULL)
     return er;
 }

 do {
   if (readnum==0) {
     if (offset==-1) {
       /* no more objects to read */
       break;
     }
     readnum=255;
     er = Filing_ReadFullDirEntry(dirname, (filing_fulldirentry *)(void *)direntries,
                                  &readnum,&offset,BUF__SIZE,NULL);
     if (er != NULL) {
       return er;
     }
     if (readnum==0) {
       break;
     }
     actptr=(filing_fulldirentry *)(void *)direntries;
   }
   else {
     actptr=(filing_fulldirentry *)(((int)(&actptr->name)+strlen(actptr->name)+4)&~3);
   }
   readnum--;

   if (funcs->foundfile!=NULL) {
     if (er=funcs->foundfile(dirname,actptr),er!=NULL) {
       return er;
     }
   }

   if (actptr->objtype==2) {
     char newname[256];
     Filing_MakePath(newname,dirname,actptr->name);
     if (er=Filing__ScanDirRec(newname,actptr,funcs),er!=NULL) {
       return er;
     }
   }
 } while(TRUE);

 if (funcs->enddir!=NULL) {
   if (er=funcs->enddir(dirname,dirdata),er!=NULL)
     return er;
 }

 return NULL;
}

os_error *Filing_ScanDir(const char *dirname,
                         Filing_ScanDir_StartDir *startdirproc,
                         Filing_ScanDir_FoundFile *foundfileproc,
                         Filing_ScanDir_EndDir *enddirproc)
{
 os_error *er;
 scandir_funcs funcs;

 funcs.startdir=startdirproc;
 funcs.foundfile=foundfileproc;
 funcs.enddir=enddirproc;

 if (dirname[strlen(dirname)-1]!='$') {
   filing_fulldirentry dirdata;
   if (er=Filing_SingleFullDirEntry(dirname,&dirdata,sizeof(dirdata)),er!=NULL)
     return er;
   return Filing__ScanDirRec(dirname,&dirdata,&funcs);
 }
 else {
   /* it could be possible to 'forge' the filing_fulldirentry for the root directory... */
   return Filing__ScanDirRec(dirname,NULL,&funcs);
 }
}
