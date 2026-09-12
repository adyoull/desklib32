#include <stdlib.h>

#include "DeskLib:Error.h"
#include "DeskLib:Event.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Debug.h"

#include "DeskLib:Font2.h"


typedef struct font2__block	*font2__blockptr;

typedef struct font2__block	{
	font2_block	pub;
	font2__blockptr	next;
	int		usage;
	}
	font2__block;
/*
Users of Font2 only get to see the 'pub' part of this.
*/


static font2__block	*font2__first = NULL;
static BOOL		font2__initialised = FALSE;


/*
font2_blocks are stored in a singly-linked list, which can only get
longer - if a block is no longer used, it is left with usage=0. When a
new block is required though, such unused blocks are reused (see
Font2__GetOrMakeBlankFont2Block.

*/


static font2__block	*Font2__MakeNewFont2Block( void)
{
font2__block	*f;

f = (font2__block *) malloc( sizeof( font2__block));
if ( !f)	return NULL;

f->usage = 0;

/* Add this font2_block to head of our list	*/
f->next		= font2__first;
font2__first	= f;

return f;
}



static font2__block	*Font2__GetOrMakeBlankFont2Block( void)
/*
Returns an unused font2_block. Will malloc one if no
unused ones are available
*/
{
font2__block	*f;

for ( f=font2__first; f; f=f->next)	{
	if ( f->usage==0)	return f;
	}

/* No unused blocks, so we make a new one	*/
return Font2__MakeNewFont2Block();
}



static BOOL	Font2__ModeChangeHandler( event_pollblock *event, void *reference)
{
int		j;
font2__block	*f;

UNUSED( reference);

if ( event->data.message.header.action != message_MODECHANGE)	return FALSE;

/* Should check that the new mode has different eigs	*/

for ( f=font2__first; f; f=f->next)	{

	font_defn	defn;

	if ( f->usage==0)	continue;

	Font_ReadDefn( f->pub.handle, &defn);

	/* Lose all previous occurencies of this font...	*/
	for ( j=0; j<f->usage; j++)	{
		Font_LoseFont( f->pub.handle);
		}

	/* ... and find them again	*/
	for ( j=0; j<f->usage; j++)	{
		/*
		Each of these calls to Font_FindFont will put the
		same font handle into font2__fonts[i].handle
		*/
		Font_FindFont(
			&f->pub.handle,
			defn.name,
			defn.xsize,
			defn.ysize,
			0,	/* Current mode	*/
			0	/* Current mode	*/
			);
		}
	}

return FALSE;
}





static void	Font2_ReleaseFontHandles( void)
{
font2__block	*f;

Debug_Printf( "Font2_ReleaseFontHandles called");

for ( f=font2__first; f; f=f->next)
	for ( ; f->usage; f->usage--)	Font_LoseFont( f->pub.handle);
}


void	Font2_Initialise( void)
{
if ( font2__initialised)	return;
Event_Claim( event_USERMESSAGE, event_ANY, event_ANY, Font2__ModeChangeHandler, NULL);
atexit( Font2_ReleaseFontHandles);
font2__initialised = TRUE;
}





os_error	*Font2_ReclaimFont( font2_block **font2, const char *name, int sizex, int sizey)
{

if ( !font2__initialised)	Font2_Initialise();

if ( *font2)	Font2_ReleaseFont( font2);

return Font2_ClaimFont( font2, name, sizex, sizey);
}




static font2__block	*Font2__FindOrMakeFont2BlockForHandle( font_handle handle)
{
font2__block	*f;

for ( f=font2__first; f; f=f->next)	{
	if ( f->pub.handle==handle)	return f;
	}

/* Make a new font2_block	*/

f = Font2__GetOrMakeBlankFont2Block();
if ( !f)	return NULL;

f->pub.handle	= handle;
Font_ReadInfo( handle, &f->pub.info);

return f;
}





os_error	*Font2_ClaimFont( font2_block **font2, const char *name, int sizex, int sizey)
{
os_error	*e;
font_handle	handle;
font2__block	*f;

if ( !font2__initialised)	Font2_Initialise();

e = Font_FindFont( &handle, name, sizex, sizey, 0, 0);

if ( e)	{
	*font2 = NULL;
	return e;
	}

f = Font2__FindOrMakeFont2BlockForHandle( handle);

if ( !f)	{
	*font2 = NULL;
	return (os_error *) 1;
	}

f->usage++;

*font2 = &f->pub;

return NULL;

}




void	Font2_ReleaseFont( font2_block **font2)
{
font2__block	*f = (font2__block *) (*font2);

if ( *font2 == NULL)	return;

/* In case this font2 hasn't been used yet	*/

Font_LoseFont( (*font2)->handle);
f->usage--;
*font2 = NULL;
}





