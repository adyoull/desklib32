/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.TempDefs.h
    Author:  Copyright © 1992 Jason Williams
             Thanks to John Winters for supplying the code that I hacked
             changed, hacked, rewrote, and then wrote again from scratch!
             Modified 19 May 1995 Julian Smith to avoid duplicated
             Window.TempDefs header file.

    Version: 1.12 (19 May 1995)
    Purpose: Definitions and stuff for internal template routines

    NOTE:    If you make changes to this header you must also fix the
             file Window.h.TempDefs, which is a duplicate of this file, and
             is used by Window.c.ModeChange.
             - Don't need to now...
*/

#ifndef __dl_tempdefs_h
#define __dl_tempdefs_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __dl_linklist_h
#include "DeskLib:LinkList.h"
#endif

#ifndef __dl_wimpswis_h
#include "DeskLib:WimpSWIs.h"
#endif

#ifndef __dl_error_h
#include "DeskLib:Error.h"
#endif

#ifndef __resource_h
#include "DeskLib:Resource.h"
#endif

#ifndef __dl_template_h
#include "DeskLib:Template.h"
#endif

#ifndef __dl_strlencr_h
#include "DeskLib:StringCR.h"
#endif

#ifndef __dl_sprite_h
#include "DeskLib:Sprite.h"
#endif

#define ERRBASE 1
#define ERR1 ERRBASE+0
#define ERRMESS1 "Not enough memory to copy template"
#define ERR2 ERRBASE+1
#define ERRMESS2 "Template '%s' not found"
#define ERR3 ERRBASE+2
#define ERRMESS3 "Not enough memory to load templates"
#define ERR4 ERRBASE+3
#define ERRMESS4 "Template file corrupt"
#define ERR5 ERRBASE+4
#define ERRMESS5 "Error reading template file"
#define ERR6 ERRBASE+5
#define ERRMESS6 "Can't find template body"
#define ERR7 ERRBASE+6
#define ERRMESS7 "Unable to find template file"
#define ERR8 ERRBASE+7
#define ERRMESS8 "Template file is empty"

#define HEADER_SIZE 16
#define INDEX_SIZE  24


/* The next few are now declared in Template.h 19 May 1995 Julian Smith */
/*
typedef struct
{
  linklist_header header;
  char            identifier[wimp_MAXNAME + 1];
  window_block    *windowdef;
  int             dataoffset;
  int             templatesize;    |* size of window+icons+indirect data *|
  int             indirectsize;    |* size of expanded indirect data     *|
  char            *indirectdata;
} template_record;


extern linklist_header template_list;
extern font_array      *template_fontarray;
*/


extern sprite_area __template_spritearea;

extern template_record *Template__FindTemplate(const char *name);

#endif
