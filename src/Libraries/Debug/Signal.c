/*
 * This file is part of DeskLib, the C library for RISC OS.
 * Please see accompanying documentation for terms of use.
 *
 *       http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module:  Debugs
 * File:    OtherLibs/Signal.c
 * Author:  Paul Field and Cy Booker, hacked around by Julian Smith
 * Purpose: Provides a set of Debug_ functions which deal with signals nicely
 *
 * Version History
 * 01/07/2007: Version 0.00
 * 22/08/2007: Added debug_siginited = TRUE to Debug_InitialiseSignal
 *
 */

#include <signal.h>

#define _DeskLib_Debug_BUILD

#include "DeskLib:Debug.h"

#include "DeskLib:Event.h"	/* for event_taskname	*/
#include "DeskLib:Error.h"



typedef struct	{
	debug_signalhandlerfn	fn;
	void			*reference;
	}
	debug_sighandlerblock;

static debug_sighandlerblock debug_usersighandler = { NULL, NULL};

static BOOL debug_siginited = FALSE;
	/* So we can check we are inited when Debug_ClaimSignal is called */

static const char *debug_signalnames[] =
	{
	"zero signal",
	"abnormal termination",
	"arithmetic exception",
	"illegal instruction",
	"interrupt",
	"memory violation",
	"terminate",
	"stack overflow",
	"user1 signal",
	"user2 signal",
	"OS error signal"
	};

#define debug_maxsignal 10



static void	Debug__SignalHandler( int sig)
{
if ( debug_usersighandler.fn)
	debug_usersighandler.fn( sig, debug_usersighandler.reference);

Error_Report(
	0,
	"'%s' has suffered a fatal internal error, signal %i (%s) and must exit immediately",
	event_taskname,
	sig,
	(sig>=1 && sig<=debug_maxsignal) ? debug_signalnames[ sig] : "Illegal signal"
	);

/* Pass things on to the default handler     */
/* (gives a useful message and a stack dump) */
raise( sig);
}



void	Debug_InitialiseSignal( void)
{
signal( SIGABRT, &Debug__SignalHandler);
signal( SIGFPE,  &Debug__SignalHandler);
signal( SIGILL,  &Debug__SignalHandler);
signal( SIGSEGV, &Debug__SignalHandler);
signal( SIGTERM, &Debug__SignalHandler);
/* signal( SIGSTAK, &Debug__SignalHandler); */
debug_siginited = TRUE;
}



void	Debug_ClaimSignal( debug_signalhandlerfn fn, void *reference)
{
if ( !debug_siginited)	 Debug_InitialiseSignal();
debug_usersighandler.fn		= fn;
debug_usersighandler.reference	= reference;
}



void	Debug_ReleaseSignal( void)
{
debug_usersighandler.fn	= NULL;
}
