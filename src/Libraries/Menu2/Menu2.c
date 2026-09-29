/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu2.Menu2.c
    Author:  Copyright © 1995 Julian Smith
    Version: 1.00 (21 Jun 1995)
    Purpose: Easy menu handling
*/


#include <stdlib.h>

#include "DeskLib:Icon.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Event.h"
#include "DeskLib:Menu.h"
#include "DeskLib:Error.h"
#include "DeskLib:Debug.h"
#include "DeskLib:Menu2.h"



typedef struct	{
	const char		*title;
	const char		*spec;
	menu2_makefn	makefn;
	menu2_flagsfn	flagsfn;
	menu2_subfn	subfn;
	menu2_selectfn	selectfn;
	menu_ptr	menu;
	void		*reference;
	}
	menu2_block;



#define menu2_MAXNEST	10
static menu2_block	*menu2__openmenus[ menu2_MAXNEST+1];
static int		menu2__numopen = 0;







static void	Menu2__ClaimRelease( event_claimorreleasefn fn, void *reference);

#define Menu2__Claim( reference)			\
	Menu2__ClaimRelease( Event_Claim, reference)

#define Menu2__Release( reference)			\
	Menu2__ClaimRelease( Event_Release, reference)








static void	Menu2__FreeMenu( menu2_block *m)
/* Frees all data associated with menu2_block m	*/
/* Doesn't free submenus...			*/
{
menu_item	*item = Menu_FirstItem( m->menu);
for( ; ; item++)	{
	Icon_DisposeIndData( &item->icondata, item->iconflags);
	if ( item->menuflags.data.last)	break;
	}
free( m->menu);
m->menu = NULL;
}


static void	Menu2__Free( int i)
/* Free menus starting from nesting i	*/
{
int	ii;
Debug_Printf( "Menu2__Free, i=%i, menu2__numopen=%i\n", i, menu2__numopen);

for ( ii=i; ii<menu2__numopen; ii++)	{

	Debug_Printf( error_PLACE "menu2__openmenus[ ii(=%i)]->makefn = %p\n",
		ii, menu2__openmenus[ ii]->makefn
		);

	if ( !menu2__openmenus[ ii]->makefn)	{
		Debug_Printf( error_PLACE "Freeing menu number %i\n", ii);
		Menu2__FreeMenu( menu2__openmenus[ ii]);
		}
	else	{
		Debug_Printf( "Haven't free-d menu - it had a special creator function\n");
		}
	/* We don't free menus we haven't created	*/
	}

menu2__numopen = i;
}


menu2_handle	Menu2_Create(
	const char			*title,
	const char			*spec,
	menu2_makefn		makefn,		/* If !=NULL, called to make the menu	*/
	menu2_flagsfn		initfn, 	/* Called every time menu is opened	*/
	menu2_subfn		subfn,		/* Called when submenu is needed	*/
	menu2_selectfn	selectfn, 		/* Called when selection is made	*/
	void			*reference
	)
{
menu2_block	*m = (menu2_block *) malloc( sizeof( menu2_block));
if (!m)	return 0;

m->title	= title;
m->spec		= spec;
m->makefn	= makefn;
m->flagsfn	= initfn;
m->subfn	= subfn;
m->selectfn	= selectfn;
m->reference	= reference;
m->menu		= NULL;

Debug_Printf( "Menu2_Create, title=%s, subfn=%p\n", title, subfn); 

return (menu2_handle) m;
}





static void	Menu2__EnsureMenu( menu2_block *m)
{
if ( m->menu == NULL)	{
	if ( m->makefn)	{
		Debug_Printf( "Menu2__EnsureMenu calling makefn at %p\n", m->makefn);
		m->menu = m->makefn( m->reference);
		}
	else	{
		m->menu = Menu_New( m->title, m->spec);
		};
	}

if ( m->flagsfn)	{
	Debug_Printf( "Menu2__EnsureMenu calling flagsfn at %p\n", m->flagsfn);
	m->flagsfn( m->menu, m->reference);
	}
}




void	Menu2_Open( menu2_handle handle, int x, int y)
{
menu2_block	*m = (menu2_block *) handle;

Debug_Printf( "Menu2_Open called\n");

Menu2__Free( 0);	/* Close all menus	*/
Menu2__EnsureMenu( m);
Menu_Show( m->menu, x, y);
menu2__openmenus[0] = m;
menu2__numopen = 1;
Menu2__Claim( NULL);
}




static int	Menu2__SelectionDepth( int *selections)
/* Returns the number of menus in the selection	*/
{
int	i;
for ( i=0; selections[i]!=-1; i++)	;
Debug_Printf( "Menu2__SelectionDepth, returning %i\n", i);
return i;
}




static void	Menu2__OpenSub( event_pollblock *event, menu2_handle handle)
{
menu2_block	*m = (menu2_block *) handle;
int		selectdepth = Menu2__SelectionDepth( &event->data.message.data.words[3]);

Debug_Printf( "Menu2__OpenSub, handle=%i, selectdepth= %i\n", handle, selectdepth);

Menu2__Free( selectdepth);

Menu2__EnsureMenu( m);
Wimp_CreateSubMenu(
	m->menu,
	event->data.message.data.words[1],
	event->data.message.data.words[2]
	);

menu2__openmenus[ menu2__numopen] = m;
menu2__numopen++;
}





static BOOL	Menu2__MessageHandler( event_pollblock *event, void *reference)
{
UNUSED( reference);

Debug_Printf( "Menu2__MessageHandler\n");

if ( event->data.message.header.action == message_MENUWARN)	{

	int		*selections	= &event->data.message.data.words[3];
	int		selectdepth	= Menu2__SelectionDepth( selections);
	menu2_block	*lastmenu	= menu2__openmenus[ selectdepth-1];
	menu2_handle	nextmenu = 0;

	Menu2__Free( selectdepth);
	Debug_Printf( "Menu2__MessageHandler, menu2__numopen=%i\n", menu2__numopen);

	if ( menu2__numopen < selectdepth)	return FALSE;
		/* Some menus are open which we don't know about...	*/

	if ( lastmenu->subfn)	{
		Debug_Printf( "Menu2__MessageHandler, calling subfn %p\n", lastmenu->subfn);
		nextmenu = lastmenu->subfn(
			selections[ selectdepth-1],
			event,
			lastmenu->reference
			);
		Debug_Printf( "Menu2__MessageHandler, nextmenu=%i\n", nextmenu);
		if (nextmenu)	Menu2__OpenSub( event, nextmenu);
		}
	}

else if ( event->data.message.header.action == message_MENUSDELETED)	{
	Menu2__Free( 0);
	Menu2__Release( reference);
	}

return FALSE;
}


static BOOL	Menu2__MenuHandler( event_pollblock *event, void *reference)
{
/*int		selectdepth	= Menu2__SelectionDepth( &event->data.selection[0]);*/
menu2_block	*leafmenu	= menu2__openmenus[ menu2__numopen-1];
mouse_block	mouse;

Wimp_GetPointerInfo( &mouse);

if ( leafmenu->selectfn)	{
	Debug_Printf( "Menu2__MenuHandler calling selectfn %p\n", leafmenu->selectfn);
	leafmenu->selectfn( event->data.selection[ menu2__numopen-1], leafmenu->reference);
	}
	/*
	Note that this is not necessarily the leaf menu, just the
	leaf-est menu2 opened. This is because the last menu2
	could have opened non-menu2 menus itself.
	*/

if ( !mouse.button.data.adjust)	Wimp_GetPointerInfo( &mouse);
	/* Have a second chance to use Adjust (eg if selecfn	*/
	/* brought up an error-box.				*/

if ( mouse.button.data.adjust)	{

	int	i;

	/* Rescan all open menus...	*/
	Debug_Printf( "Menu2__MenuHandler reopening menu\n");

	for ( i=0; i<menu2__numopen; i++)	{
		menu2_block	*m = menu2__openmenus[ i];
		if ( m->flagsfn)	{
			Debug_Printf( "Menu2__MenuHandler calling flagsfn\n");
			/*Debug_Printf( "menublock is %p %p %p %p %p %p %p %p\n",
				m->title, m->spec, m->makefn, m->flagsfn, m->subfn,
				m->selectfn, m->menu, m->reference
				);
			*/
			m->flagsfn( m->menu, m->reference);
			}
		}

	Debug_Printf( "Menu2__MenuHandler calling Menu_ShowLast\n");
	Menu_ShowLast();
	}

else	{
	Menu2__Free( 0);
	Menu2__Release( reference);
	}

return FALSE;
}




static void	Menu2__ClaimRelease( event_claimorreleasefn fn, void *reference)
{
fn( event_USERMESSAGE, event_ANY, event_ANY, Menu2__MessageHandler, reference);
fn( event_USERMESSAGERECORDED, event_ANY, event_ANY, Menu2__MessageHandler, reference);
fn( event_MENU, event_ANY, event_ANY, Menu2__MenuHandler, reference);
}

