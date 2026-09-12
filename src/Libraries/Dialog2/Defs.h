#ifndef __dl_dialog2_defs_h
#define __dl_dialog2_defs_h

#ifndef __dl_event_h
#include "DeskLib:Event.h"
#endif

#ifndef __dl_dialog2_h
#include "DeskLib:Dialog2.h"
#endif




void	Dialog2__CommonOpenCode( dialog2_block *dialog2);
void	Dialog2__CommonOpenMenuCode( dialog2_block *dialog2);

BOOL	Dialog2__CloseDialogInternal(
	dialog2_block	*dialog2,
	BOOL		closemenu,	/* If TRUE, close the current menu if dialog is a menu	*/
	BOOL		deletewindow	/* If TRUE, delete the window if dialog2 allows.	*/
	);

#endif
