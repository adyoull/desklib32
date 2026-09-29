/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Screen.h
    Author:  Copyright © 1992 Jasn Williams
    Version: 1.01 (17 Apr 2005)
             1.02 (10 Oct 2007) Replaced macros with enum (originally from kernelSWIs)
    Purpose: Screen functions (read size, eig factors, etc.)
*/

#ifndef __dl_screen_h
#define __dl_screen_h

#ifndef __dl_wimp_h
#include "DeskLib:Wimp.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


/* Abstract */
/*
  This header declares a function to read information about the current
  screen mode, and store it in various global variables.  To ensure that
  the variables are up to date, you should either call Screen_CacheModeInfo
  before you access them, or call it every time the mode changes.

  A handler is provided in the Handler library (Handler_ModeChange) which you
  can use with the EventMsg system to call the function on every mode change.
*/

typedef enum
{
  modevar_MODEFLAGS = 0,
  modevar_RIGHTCOLUMN,
  modevar_BOTTOMROW,
  modevar_NCOLOUR,
  modevar_XEIGFACTOR,
  modevar_YEIGFACTOR,
  modevar_LINELENGTH,
  modevar_SCREENSIZE,
  modevar_YSHIFTFACTOR,
  modevar_LOG2BPP,
  modevar_LOG2BPC,
  modevar_XWINDOWLIMIT,
  modevar_YWINDOWLIMIT
} mode_variable;
/*
  This gives useful names to the various mode variables you can read with
  Screen_ReadModeInfo.
*/

/* haddoc ignore on */
/* These are here for legacy support - superceded by mode_variable enum */
#define SCREEN_VAR_ModeFlags    0
#define SCREEN_VAR_ScrRCol      1
#define SCREEN_VAR_ScrBRow      2
#define SCREEN_VAR_NColour      3
#define SCREEN_VAR_XEigFactor   4
#define SCREEN_VAR_YEigFactor   5
#define SCREEN_VAR_LineLength   6
#define SCREEN_VAR_ScreenSize   7
#define SCREEN_VAR_YShiftFactor 8
#define SCREEN_VAR_Log2BPP      9
#define SCREEN_VAR_Log2BPC     10
#define SCREEN_VAR_XWindLimit  11
#define SCREEN_VAR_YWindLimit  12
/* haddoc ignore off */

/* Mode specifier */
typedef struct {
  unsigned int flags;
  unsigned int xres;
  unsigned int yres;
  unsigned int bpp;
  unsigned int framerate;
  struct {
    unsigned int index;
    unsigned int value;
  } vars[1];
} screen_modespec;


/* Format specifier used for sprites */
typedef struct {
  int istype   : 1;
  int horz_dpi : 13;
  int vert_dpi : 13;
  int type     : 5;
} screen_modesprite;


/* RISC OS modes */
typedef union {
  unsigned int       screen_mode;
  screen_modesprite  sprite_mode;
  screen_modespec   *spec_mode;
} screen_modeval;


extern screen_modeval screen_mode;
/* This holds the current screen mode. */

extern wimp_point screen_size;
/* This holds the current screen size in OS units. */

extern wimp_point screen_eig;     /* Log2(screen_delta) */
/*
  This holds the current screen eigen values,
  OS units = pixels << eigen value
*/

extern wimp_point screen_delta;
/*
  This holds the size of a pixel in OS coordinates for the current mode.
*/

extern int screen_bpp;
/*
  This gives the bits per pixel for the current mode,
  ie. the colour "depth".
*/

/* haddoc ignore on */
/* Old name defined to maintain backwards compatibilty */
#define OS_ReadModeVariable(m,v,r) Screen_ReadModeVariable(m,v,r)
/* haddoc ignore off */
os_error *Screen_ReadModeVariable(int mode, mode_variable variable, int *value);
/*
  This function reads a mode variable for the mode 'mode', or the current
  screen mode if 'mode' is -1.  The value is returned in 'value'.
*/

extern BOOL Screen_CacheModeInfo(void);
/*
  This function reads various pieces of information about the current
  screen mode and sets the variables defined by this file appropriately.
  It should be called before you try to use any of those variables.

  It only needs to be called when the mode changes, so you can call it
  when your program starts and whenever a mode change message is received,
  or else immediately before you use the variables.

  It checks the screenmode, and if it has not changed from the last call,
  then the variables are not updated.  This means it will not be a big
  problem if you call it more often than strictly necessary (every time
  you redraw, for example).

  This function always returns TRUE.
*/


#ifdef __cplusplus
}
#endif


#endif
