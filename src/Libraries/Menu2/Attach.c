/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu2.Attach.c
    Author:  Copyright © 1995 Julian Smith
    Version: 1.00 (22 Jun 1995)
    Purpose: Attaching menus to icons/windows.
*/


#include "DeskLib:Event.h"
#include "DeskLib:Menu2.h"



static BOOL	Menu2__OpenFromClick(
	event_pollblock	*event,
	menu2_handle	menu,
	unsigned int	button
	)
{
if ( event->data.mouse.button.value != button)	return FALSE;

Menu2_Open(
	menu,
	event->data.mouse.pos.x,
	(event->data.mouse.window < 0) ? -1 : event->data.mouse.pos.y
	);
return TRUE;
}



static BOOL	Menu2__SelectClickHandler( event_pollblock *event, void *reference)
{
return Menu2__OpenFromClick( event, (menu2_handle) reference, button_SELECT);
}


static BOOL	Menu2__AdjustClickHandler( event_pollblock *event, void *reference)
{
return Menu2__OpenFromClick( event, (menu2_handle) reference, button_ADJUST);
}



static BOOL	Menu2__MenuClickHandler( event_pollblock *event, void *reference)
{
return Menu2__OpenFromClick( event, (menu2_handle) reference, button_MENU);
}



static void	Menu2__AttachOrReleaseMenu(
	window_handle		window,
	icon_handle		icon,
	menu2_handle		menu,
	int			button,
	event_claimorreleasefn	fn
	)
{
if ( button & button_MENU)
	fn( event_CLICK, window, icon, Menu2__MenuClickHandler, (void *) menu );

if ( button & button_SELECT)
	fn( event_CLICK, window, icon, Menu2__SelectClickHandler, (void *) menu);

if ( button & button_ADJUST)
	fn( event_CLICK, window, icon, Menu2__AdjustClickHandler, (void *) menu);
}




void	Menu2_AttachMenu(
	window_handle	window,
	icon_handle	icon,
	menu2_handle	menu,
	int		button
	)
{
Menu2__AttachOrReleaseMenu( window, icon, menu, button, Event_Claim);
}



void	Menu2_DetachMenu(
	window_handle	window,
	icon_handle	icon,
	menu2_handle	menu,
	int		button
	)
{
Menu2__AttachOrReleaseMenu( window, icon, menu, button, Event_Release);
}
