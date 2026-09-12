/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Resource.Choices.c
    Author:  Copyright © 2001 Lenny
    Version: 0.01 (25 Feb 2001)
    Purpose: Support for Choices files.  It will use the RiscPC boot Choices
             variables if these are defined, else it will fall back to using
             the application's directory.
    Note:    Based on the function given by Justin Fletcher in his Choices
             doc, though with additional functionality for grouping multiple
             choices files.
    NB.      Resource_Initialise() should be called prior to calling either
             of these routines.
    History: 0.01 (25 Feb 2001) : Added   'Resource_ChoicesInit()'
                                : Added   'Resource_ChoicesPath()'

*/

/* --- LOAD HEADERS ------------------------------------------------------ */

/* --- DeskLib ----------------------------------------------------------- */
#include "DeskLib:Error.h"          /* Centralised error reporting         */
#include "DeskLib:KernelSWIs.h"     /* General low level SWI veneers       */
#include "DeskLib:Resource.h"       /* Handles finding resource files      */
#include "DeskLib:File.h"           /* Filing system functions             */

/* --- CLib -------------------------------------------------------------- */
#include <stdio.h>      /*  General I/O routines                           */
#include <string.h>     /*  String manipulation routines                   */

/* === FUNCTION DEFINITIONS ============================================== */

/*  Global variables.
 */

static BOOL resource_choices_init = FALSE;
static BOOL resource_choices_multiple = FALSE;
static char resource_choices_groupdir[32] = "\0";
static char resource_choices_appname[32] = "\0";


extern void Resource_ChoicesInit(const char *groupdir, const char *appname, BOOL multiple)
/*
 *  Initialises the Choices system (sets up global variables).
 *
 *  This MUST be called prior to using the following function -
 *  Resource_ChoicesPath().
 *
 *  'groupdir' : != NULL (or zero length string) to use a parent directory
 *               (eg 'WWW' or 'MidiWays').
 *  'appname'  : Name of the owning application.
 *  'multiple' : == TRUE allows for multiple choices files (each having a
 *               different leafname), placed in a subdirectory, named <appname>
 *               on new boot systems, or in '<appname$Dir>.Choices.' on older
 *               systems.
 *               You may use multiple==TRUE even if you're using only a single
 *               choices file, if you want that file to be placed within a
 *               subdirectory.
 *
 *  Date : 0.01 : 25 Feb 2001
 */
{

  if (groupdir == NULL)  resource_choices_groupdir[0] = 0;
  else if (groupdir[0] == 0)  resource_choices_groupdir[0] = 0;
  else  strcpy(resource_choices_groupdir, groupdir);

  if ((appname == NULL) || (appname[0] == 0)) {
    Error_ReportFatal(1, "Choices Init error : the application name must be provided");
  } else {
    strcpy(resource_choices_appname, appname);
  }

  resource_choices_multiple = multiple;

  resource_choices_init = TRUE;  /* Flag that this function has been called */

}

/* ----------------------------------------------------------------------- */

extern void Resource_ChoicesPath(char *pathname, const char *leafname, BOOL write)
/*
 *  Returns (in 'pathname') the location of a choices file (to be read or
 *  written).  This function doesn't actually read or write the file.
 *  NB.  Space needs to be allocated to 'pathname' prior to calling this fn.
 *
 *  If the <Choices$Write> variable exists (ie the new boot or the !Choices
 *  pseudo app is present) that is used, else it falls back to using the
 *  application's directory.
 *
 *  'groupdir', 'appname' and 'multiple' should have been defined (with a
 *  call to the Resource_ChoicesInit() function, above).
 *
 *  'leafname' : Name of the choices file.
 *  'write'    : == TRUE if wanting to write choices.
 *               == FALSE if wanting to read choices.
 *
 *  Examples (appname='MyApp' and leafname='!Config' in each case) :
 *
 *                                      With RiscPC boot               No Choices: system
 *                                      -------------------------      ---------------------------
 *    groupdir=NULL, multiple=FALSE  :  Choices:MyApp              OR  <MyApp$Dir>.!Config
 *    groupdir=NULL, multiple=TRUE   :  Choices:MyApp.!Config      OR  <MyApp$Dir>.Choices.!Config
 *    groupdir='WWW', multiple=FALSE :  Choices:WWW.MyApp          OR  <MyApp$Dir>.!Config
 *    groupdir='WWW', multiple=TRUE  :  Choices:WWW.MyApp.!Config  OR  <MyApp$Dir>.Choices.!Config
 *
 *  Date : 0.01 : 25 Feb 2001
 */
{
  char              tmp[256], tmp2[256];
  BOOL              group;
  os_error          *error;

  /* Check that the Resource_ChoicesInit() function has been called .. */
  if (resource_choices_init == FALSE) {
    Error_ReportFatal(1, "Choices system not initialised");
  }

  if (resource_choices_groupdir[0] == 0)  group = FALSE;
  else  group = TRUE;

  if (OS_ReadVarVal("Choices$Write", tmp, 256) == TRUE) {
    /* The Choices: resource (new boot or !Choices pseudo app) exists .. */
    if (write) {
      /* Writing.  Build pathname, and ensure subdir(s) exist .. */
      if (group) {
        if (resource_choices_multiple) {
          sprintf(pathname, "<Choices$Write>.%s.%s.%s",
                  resource_choices_groupdir, resource_choices_appname, leafname);
          sprintf(tmp, "<Choices$Write>.%s", resource_choices_groupdir);  File_CreateDir(tmp);
          sprintf(tmp, "<Choices$Write>.%s.%s", resource_choices_groupdir, resource_choices_appname);
          File_CreateDir(tmp);
        } else {
          /* appname is used as leafname .. */
          sprintf(pathname, "<Choices$Write>.%s.%s", resource_choices_groupdir, resource_choices_appname);
          sprintf(tmp, "<Choices$Write>.%s", resource_choices_groupdir);  File_CreateDir(tmp);
        }
      } else {
        if (resource_choices_multiple) {
          sprintf(pathname, "<Choices$Write>.%s.%s", resource_choices_appname, leafname);
          sprintf(tmp, "<Choices$Write>.%s", resource_choices_appname);  File_CreateDir(tmp);
        } else {
          /* appname is used as leafname .. */
          sprintf(pathname, "<Choices$Write>.%s", resource_choices_appname);
        }
      }
    } else {
      /* Reading.  Build pathname .. */
      if (group) {
        if (resource_choices_multiple) {
          sprintf(pathname, "Choices:%s.%s.%s", resource_choices_groupdir, resource_choices_appname, leafname);
        } else {
          /* appname is used as leafname .. */
          sprintf(pathname, "Choices:%s.%s", resource_choices_groupdir, resource_choices_appname);
        }
      } else {
        if (resource_choices_multiple) {
          sprintf(pathname, "Choices:%s.%s", resource_choices_appname, leafname);
        } else {
          /* appname is used as leafname .. */
          sprintf(pathname, "Choices:%s", resource_choices_appname);
        }
      }
    }
    /* Check for auto-upgrade ..   */
/*    if (multiple)  sprintf(tmp, "<%s$Dir>.Choices.%s", appname, leafname); */ /* Something like this for */
/*    else           sprintf(tmp, "<%s$Dir>.%s", appname, leafname);         */ /* non DeskLib users */
    if (resource_choices_multiple)  sprintf(tmp, "%sChoices.%s", resource_pathname, leafname);
    else                            sprintf(tmp, "%s%s", resource_pathname, leafname);
    if (File_Exists(tmp)) {
      /* The file already exists within app, though Choices$Dir also exists .. */
      if (write) {
        /* As we're writing, this file is out of date, so delete it (but only if possible to save a new one) .. */
        if (File_IsDirectory(pathname) == FALSE) {
          File_Delete(tmp);
        }
      } else {
        /* We're reading, so move file to new location, first ensuring dest dir(s) exist .. */
        if (group) {
          if (resource_choices_multiple) {
            sprintf(tmp2, "<Choices$Write>.%s", resource_choices_groupdir);  File_CreateDir(tmp2);
            sprintf(tmp2, "<Choices$Write>.%s.%s", resource_choices_groupdir, resource_choices_appname);
            File_CreateDir(tmp2);
            sprintf(tmp2, "<Choices$Write>.%s.%s.%s", resource_choices_groupdir, resource_choices_appname, leafname);
          } else {
            /* appname is used as leafname .. */
            sprintf(tmp2, "<Choices$Write>.%s", resource_choices_groupdir);  File_CreateDir(tmp2);
            sprintf(tmp2, "<Choices$Write>.%s.%s", resource_choices_groupdir, resource_choices_appname);
          }
        } else {
          if (resource_choices_multiple) {
            sprintf(tmp2, "<Choices$Write>.%s", resource_choices_appname);  File_CreateDir(tmp2);
            sprintf(tmp2, "<Choices$Write>.%s.%s", resource_choices_appname, leafname);
          } else {
            /* appname is used as leafname .. */
            sprintf(tmp2, "<Choices$Write>.%s", resource_choices_appname);
          }
        }
        /* Move the file (tmp -> tmp2) .. */
        error = File_Copy(tmp, tmp2, (1<<12)+(1<<7)+(1<<1));   /* Flags : 12=Newer, 7=DeleteSrce, 1=Force */
        if (error == NULL) {
          if (File_Exists(tmp))  File_Delete(tmp);  /* It didn't get copied because it's older - delete it */
        } else {
          /*  Should really take action here if the copy failed (delete the old version, at least).
           */
          if (error->errnum == 175) {
            Error_Report(1, "Could not auto-upgrade the choices file, a directory with that name ('%s') already exists.", tmp2);
          } else {
            Error_Report(1, "Could not auto-upgrade the choices file (Error %d - %s)", error->errnum, error->errmess);
          }
        }
      }
    }

  } else {
    /* No Choices: structure - ignore groupdir, use application dir .. */
    if (resource_choices_multiple) {
/*      sprintf(pathname, "<%s$Dir>.Choices.%s", appname, leafname);*/  /* Something like this for non DeskLib users */
      sprintf(pathname, "%sChoices.%s", resource_pathname, leafname);
      if (write) {  /* ensure dir exists .. */
/*        sprintf(tmp, "<%s$Dir>.Choices", appname);  File_CreateDir(tmp); */ /* for non DeskLib users */
        sprintf(tmp, "%sChoices", resource_pathname);  File_CreateDir(tmp);
      }
    } else {
/*      sprintf(pathname, "<%s$Dir>.%s", appname, leafname);*/  /* for non DeskLib users */
      sprintf(pathname, "%s%s", resource_pathname, leafname);
    }
  }

}

/***************************************************************************/
