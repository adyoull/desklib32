/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Debug.h
    Author:  Copyright © 1995 Julian Smith, Cy Booker, Paul Field.
    Version: 1.02 (17 Apr 2005)
    Purpose: Defines interface to a set of functions useful for
             debugging.
    Mods:    25 Aug 1995 - JS - Added Debug_Assert.

*/

/* Abstract */
/*
  This file has prototypes for a standard set of simple debugging functions.

  Several different ways of outputting debug messages are provided. Call
  Debug_Initialise to specify which method to use (otherwise the library
  defaults to outputting to stderr).

  This header file ensures that all calls to Debug_xxx functions are
  macro-ed out by the preprocessor unless DeskLib_DEBUG is predefined.
  So don't worry about debug calls taking up space in your finished
  project, they will disappear when you build a normal version of your
  project.

  Thread Safety: Debug_Initialise is not thread-safe (though, in general
  you should only need to call it once at the start of your app).
  Debug_Printf may or may not be thread-safe, depending on the value
  passed to Debug_Initialise.
*/

#ifndef __dl_debug_h
#define __dl_debug_h

#ifndef __dl_core_h
#include "DeskLib:Core.h"
#endif

#ifndef __dl_error_h
#include "DeskLib:Error.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  debug_UNINITIALISED,
  debug_REPORTER,
  debug_PIPETYPE,
  debug_STDERR,
  debug_UNIQUEFILE,
  debug_UNIQUEPIPE
} debug_type;
/*
   This enum is used to specify which method of outputting debug
   information you'd like the debug functions to use. Pass one of
   the values to Debug_Initialise.

   REPORTER will output to Martin Avison's Reporter if it's
   present when the debug library is initialised (otherwise it
   will return TRUE and the debug type will remain uninitialised).
   Debug_Printf is thread-safe if this option is successful.

   PIPETYPE writes to a file in Pipe: and displays it in a
   taskwindow on screen (so can only be initialised after
   the program has called Event_Initialise).

   STDERR writes to the stderr output (usually specified by
   redirection in your !Run file).

   UNIQUEFILE writes to a unique file inside Scrap

   UNIQUEPIPE is similar to PIPETYPE but creates a unique file in
   Pipe: and does not open it in a taskwindow.

   UNINITIALISED is only for use internally.
*/

typedef void (*debug_signalhandlerfn)( int sig, void *reference);
/*
  This is a function type which is called when a signal happens.
*/

#if defined(DeskLib_DEBUG) || defined(_DeskLib_Debug_BUILD)

        BOOL Debug_Initialise(debug_type type);
	/*
	  Sets up the debug library. Call this before you use the
	  Debug_Print or Debug_Printf functions. "type" specifies how
	  the debug functions will output the debug information.

	  Calling this function is not mandatory (unless using threads),
	  as it will be automatically called if you don't do so. In
	  this case, it is initialised to the STDERR output.

	  It returns TRUE if an error occurred.
	*/

	void Debug_InitialiseSignal(void);
	/*
	  Sets up a default handler for signals which calls Error_Report.
	  To set up your own handler, use Debug_ClaimSignal (which calls
	  Debug_InitialiseSignal for you).
	*/

	void Debug_ClaimSignal(debug_signalhandlerfn fn, void *reference);
	/*
	  Makes 'fn' be called when a signal occurs.
	*/

	void Debug_ReleaseSignal(void);
	/*
	  Stops the fn, from a previous call to Debug_ClaimSignal, being called.
	  The default handler is re-instated.
	*/

	int Debug_Printf(const char *format, ...);
	/*
	  Sends the text to the debug output, in same way as printf.
	*/

	void Debug_Print(const char *text);
	/*
	  Sends a string to the debug output.
	*/


	#define Debug_Error_CheckFatal( errfn) \
		do	{					\
			os_error	*debug__e = errfn;	\
				if ( debug__e)	{		\
				Error_ReportFatal( 		\
					debug__e->errnum, 	\
					error_PLACE "%s", 	\
					debug__e->errmess	\
					);			\
				}				\
			}					\
			while (0)
	/*
	  As Error_ReportFatal, but also displays the file and line number if
	  DeskLib_DEBUG is predefined.
	*/

	#define Debug_Assert( expression)					\
		(								\
			(expression) 						\
				? 						\
				(void) 0	 				\
				: 						\
				Debug__Assert( #expression, __FILE__, __LINE__)	\
		)
	/*
	  This is similar to the standard ANSI C 'assert' macro. If
	  'expression' evaluates to FALSE, Debug_Printf is used
	  (indirectly) to output a diagnostic message.

	  Like all the Debug_ commands, Debug_Assert is removed before compilation
	  unless DeskLib_DEBUG is defined.
	*/

#else

	/*
          These effectively remove any references to Debug_ functions from
          source code.
        */

        /* This bit is ignored to avoid duplicate entries being written */
        /* haddoc ignore on */

	#define Debug_Initialise(choice)

	#define Debug_InitialiseSignal()

	#define Debug_ReleaseSignal()

	#define Debug_Print(text)

	#define Debug_ClaimSignal(fn, reference)

	#define Debug_Assert(expression)

	#define Debug_Error_CheckFatal(errfn) Error_CheckFatal(errfn);

	#define Debug_Printf Debug_Dummyf

        /* haddoc ignore off */

#endif






/* haddoc ignore on */
/* This bit is ignored as the programmer doesn't need to see it */

int Debug__Dummyf(const char *format, ...);
/*
  This is a dummy function which takes an arbitary number of
  arguments. It is called instead of Debug_Printf when compilation
  is for a non-debug version - it isn't possible to macro-out a call to
  a function such as Debug_Printf which takes an arbitary number of
  arguments.
*/

#define Debug_Dummyf  if (1) (void)0; else Debug__Dummyf
/*
  A macro which is used to remove calls to a multi-argument function
  (Debug_Printf) which returns an integer. This isn't a bomb-proof macro,
  but is the best I can come up with. Debug__Dummyf is a dummy function
  which does nothing.

  NB The simpler alternative macro '#define Debug_Dummyf if (0)
  Debug__Dummyf' isn't used because it affects the meaning of 'else' in
  (for example):

  if (flag) Debug_Dummyf( "flag=%i\n", flag);
  else flag=3;
*/

/* haddoc ignore off */

extern int debug_level;
/*
  This is initially 0. Setting it to values greater than zero will turn on
  any subsequent DebugX_Printf statements.

  Note that this will only happen with code compiled with DeskLib_DEBUG
  predefined.

  Note that this selection of DebugX_Printf is 'live' - ie you can change
  debug_level before calling different functions to only have diagnostics
  from certain parts of your code.

  At present, no DeskLib libraries have DebugX or Debug calls (and even if
  they did, you would have to build a new DeskLib with DeskLib_DEBUG
  predefined to use them).
*/


/*
  The DebugX_Printf macros can be used to control how much
  diagnostic information is produced.
*/

#define Debug1_Printf (debug_level<1) ? 0 : Debug_Printf
/* Only outputs diagnostics if debug_level is >= 1	*/

#define Debug2_Printf (debug_level<2) ? 0 : Debug_Printf
/* Only outputs diagnostics if debug_level is >= 2	*/

#define Debug3_Printf (debug_level<3) ? 0 : Debug_Printf
/* Only outputs diagnostics if debug_level is >= 3	*/

#define Debug4_Printf (debug_level<4) ? 0 : Debug_Printf
/* Only outputs diagnostics if debug_level is >= 4	*/

#define Debug5_Printf (debug_level<5) ? 0 : Debug_Printf
/* Only outputs diagnostics if debug_level is >= 5	*/





#ifdef __cplusplus
}
#endif


#endif
