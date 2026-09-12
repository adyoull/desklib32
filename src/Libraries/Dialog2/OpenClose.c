/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog2.OpenClose.c
    Author:  Copyright © 1994 Julian Smith
    Version: 1.03 (13 Mar 1995)
    Purpose: Dialogue box handling
*/


#include <stdlib.h>

#include "DeskLib:Window.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Event.h"
#include "DeskLib:Error.h"
#include "DeskLib:Handler.h"
#include "DeskLib:Sound.h"
#include "DeskLib:Template.h"
#include "DeskLib:Dialog2.h"

#include "Defs.h"


dialog2_block	*dialog2_menublock = NULL;


static void	Dialog2__ClaimOrReleaseStandardHandlers(
	event_claimorreleasefn	fn,
	dialog2_block		*dialog2
	);
	/* The actual function is later on...	*/





BOOL	Dialog2__CloseDialogInternal(
	dialog2_block	*dialog2,
	BOOL		closemenu,	/* If TRUE, close the current menu if dialog is a menu	*/
	BOOL		deletewindow	/* If TRUE, delete the window if dialog2 allows.	*/
	)
{
if ( !dialog2)	return ERROR;
if ( dialog2->flags.data.type == dialog2_type_CLOSED)	return ERROR;

if ( dialog2->window)	{

	Dialog2__ClaimOrReleaseStandardHandlers( Event_Release, dialog2);

	if (	   dialog2->flags.data.type == dialog2_type_MENU
		|| dialog2->flags.data.type == dialog2_type_MENULEAF
		)
		{
		if (closemenu)	Wimp_CreateMenu( (menu_block *) -1, 0, 0);
		}
	else	Wimp_CloseWindow( dialog2->window);

	if ( !dialog2->flags.data.keepwindow)	{
		if ( deletewindow)	{
			Window_Delete( dialog2->window);
			dialog2->window = NULL;
			}
		else	Event_ReleaseWindow( dialog2->window);
		}
	}

dialog2->flags.data.type = dialog2_type_CLOSED;

if ( dialog2->flags.data.notifyclose && dialog2->openfn)
	dialog2->openfn( dialog2);

if ( dialog2_menublock == dialog2)	dialog2_menublock = NULL;
	/* So we know whether a menu-dialog2 is open.	*/

return NOERROR;
}




BOOL	Dialog2_CloseDialog( dialog2_block *dialog2)
{
return Dialog2__CloseDialogInternal(
	dialog2,
	TRUE,	/* close menu if dialog2 is part of a menu;		*/
	TRUE	/* delete window if !dialog2->flags.data.keepwindow	*/
	);
}




/* Handler for message_MENUSDELETED - sent if the menu closes.	*/
static BOOL Dialog2__MenusDeletedHandler(event_pollblock *event, void *reference)
{
UNUSED( reference);

if ( event->data.message.header.action == message_MENUSDELETED && dialog2_menublock)	{

	if	(
		dialog2_menublock->flags.data.type == dialog2_type_MENU
		&&
		(window_handle) event->data.message.data.words[0] != dialog2_menublock->window
		)
		return FALSE;
		/* The menu being deleted isn't our dialog2 box - eg the user	*/
		/* has opened the dialog2_menublock from another menu for which	*/
		/* we are now getting a message_MENUSDELETED, *after* we've 	*/
		/* opened our dialog2 as a menu-window.				*/

	Dialog2_CloseDialog( dialog2_menublock);
	}
return FALSE;
}






/* Handler for click on OK or Cancel buttons.	*/
static BOOL Dialog2__OKCancelHandler( event_pollblock *event, void *reference)
{
dialog2_block *dialog2 = (dialog2_block *) reference;

if ( dialog2->flags.data.type == dialog2_type_CLOSED)	{
	Error_ReportFatalInternal( 1, error_PLACE "Click for closed dialog2");
	}

if ( dialog2->okfn && event->data.mouse.icon == dialog2->flags.data.okbutton)
	dialog2->okfn( dialog2);


if	(
	event->data.mouse.button.data.select
	&&
		(
		   event->data.mouse.icon == dialog2->flags.data.okbutton
		|| event->data.mouse.icon == dialog2->flags.data.cancelbutton
		)
	)
	{
	Dialog2_CloseDialog( dialog2);
	}

return FALSE;
}


/* Handler for event_CLOSE.	*/
static BOOL Dialog2__CloseHandler( event_pollblock *event, void *reference)
{
UNUSED( event);
Dialog2_CloseDialog( (dialog2_block *) reference);
return FALSE;
}





#define keycode_RETURN	13
#define keycode_ESCAPE	0x1B

/* This sends our application an event_CLICK on the ok/cancel button	*/
/* when Return/Escape is pressed in the dialog box. Would be better to 	*/
/* have an Event_FakeEvent function or something...			*/
static BOOL Dialog2__KeyHandler( event_pollblock *event, void *reference)
{
dialog2_block *dialog2 = (dialog2_block *) reference;

if ( dialog2->flags.data.type == dialog2_type_CLOSED)	{
	Error_ReportFatalInternal( 1, error_PLACE "Click for closed dialog2");
	}

if ( event->data.key.code == keycode_RETURN && dialog2->flags.data.okbutton != -1)	{
	event_data	clickeventdata;
	clickeventdata.mouse.button.value	= 0;
	clickeventdata.mouse.button.data.select	= TRUE;
	clickeventdata.mouse.window		= dialog2->window;
	clickeventdata.mouse.icon		= dialog2->flags.data.okbutton;
	Wimp_SendMessage(
		event_CLICK,
		(message_block *) &clickeventdata,
		event_taskhandle, 			/* Us	*/
		dialog2->flags.data.okbutton
		);
	return TRUE;
	}

else if ( event->data.key.code == keycode_ESCAPE && dialog2->flags.data.cancelbutton != -1)	{
	event_data	clickeventdata;
	clickeventdata.mouse.button.value	= 0;
	clickeventdata.mouse.button.data.select	= TRUE;
	clickeventdata.mouse.window		= dialog2->window;
	clickeventdata.mouse.icon		= dialog2->flags.data.cancelbutton;
	Wimp_SendMessage(
		event_CLICK,
		(message_block *) &clickeventdata,
		event_taskhandle, 			/* Us	*/
		dialog2->flags.data.cancelbutton
		);
	return TRUE;
	}

else Wimp_ProcessKey( event->data.key.code);

return FALSE;
}








static void	Dialog2__ClaimOrReleaseStandardHandlers(
	event_claimorreleasefn	fn,
	dialog2_block		*dialog2
	)
	/* This calls Event_Claim/Release for all the 	*/
	/* events in which we are interested in.	*/
	/* Using the same function for claiming and 	*/
	/* releasing events ensures we don't leave any	*/
	/* claims active that we don't use.		*/
{

fn( event_OPEN,  dialog2->window, event_ANY, Handler_OpenWindow,    dialog2);
fn(	event_CLOSE, dialog2->window, event_ANY,
	Dialog2__CloseHandler,
	dialog2
	);

if ( dialog2->flags.data.okbutton != -1 || dialog2->flags.data.cancelbutton != -1)	{
	fn(	event_KEY, dialog2->window, event_ANY,
		Dialog2__KeyHandler,
		dialog2
		);
	fn(	event_CLICK, dialog2->window, event_ANY,
		Dialog2__OKCancelHandler,
		dialog2
		);
	/* Make this icon-unspecific so that the application will get clicks first.	*/
	}

if ( dialog2->flags.data.type == dialog2_type_MENU
||   dialog2->flags.data.type == dialog2_type_MENULEAF
)	{
	fn(	event_USERMESSAGE, event_ANY, event_ANY,
		Dialog2__MenusDeletedHandler,
		NULL
		);
		/* Closes the dialog2 if the menu is closed by Escape or a 	*/
		/* click outside the menu.					*/
		/* NB this won't work with RO2 - the dialog2 won't be closed	*/
		/* until the next dialog2 is opened as a menu.			*/
	}

}









void	Dialog2__CommonOpenCode( dialog2_block *dialog2)
{
Dialog2_EnsureWindowHandle( dialog2);
Dialog2__ClaimOrReleaseStandardHandlers( Event_Claim, dialog2);

if (dialog2->openfn)	dialog2->openfn( dialog2);
	/* Call this after 'Dialog2__ClaimOrReleaseStandardHandlers'
	so that any events claimed inside 'openfn' will
	get sent to the application's code first.
	*/

}



void	Dialog2__CommonOpenMenuCode( dialog2_block *dialog2)
{
if ( dialog2_menublock)	{	/* Need to close the existing menu-dialog box.	*/
	if ( dialog2_menublock == dialog2)
		Dialog2__CloseDialogInternal( dialog2_menublock, FALSE, FALSE);
			/* This dialog is being re-opened, so don't delete the	*/
			/* window as we will only re-open it anyway.		*/

	else	Dialog2__CloseDialogInternal( dialog2_menublock, FALSE, TRUE);
	}

dialog2_menublock = dialog2;

}













