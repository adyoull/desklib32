#include <stdlib.h>
#include <stdio.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:PDriver.h"
#include "DeskLib:Error.h"
#include "DeskLib:File.h"
#include "DeskLib:Event.h"

#include "DeskLib:Print.h"



typedef enum	{
	print__progress_2,
	print__progress_4_5,
	print__progress_5,
	print__progress_6,
	print__progress_7
	}
	print__progress;
	/* These correspond to the numbered paragraphs in PRMs 3-259.	*/


typedef struct	{

	print_block	ppublic;

	unsigned int	message_ref;	/* id of the last event_SENDWANTACK we sent.	*/
	print__progress	progress;
	print_printfn	printfn;
	print_savefn	savefn;
	print_resultfn	resultfn;
	}
	print__block;


static void	Print__ClaimMessages( print__block *print);
static void	Print__ReleaseMessages( print__block *print);

static void	Print__Finish(
	print__block	*print,
	BOOL			releasemsgs,
	print_result	result
	);



static void	Print__PrintIt( print__block *print)
	/* This is called when the Wimp messaging between us	*/
	/* and !Printers has finished.				*/
{
os_error	*error;

Print__ReleaseMessages( print);

error =  PDriver_Info( &print->ppublic.printerinfo);
if (error)	{
	Print__Finish( print, FALSE, (print_result) error);
	return;
	}

print->ppublic.job = File_Open( "Printer:", file_WRITE);
if ( !print->ppublic.job)	{
	Print__Finish( print, FALSE, print_result_CANTOPENPRINTER);
	return;
	}

error = PDriver_SelectJob( print->ppublic.job, print->ppublic.jobtitle, &print->ppublic.oldjob);
if (error)	{
	Print__Finish( print, FALSE, (print_result) error);
	return;
	}

print->printfn( &print->ppublic);

PDriver_EndJob( print->ppublic.job);
File_Close( print->ppublic.job);

Print__Finish( print, FALSE, print_result_OK);

return;
}





/*
#ifndef message_PRINTERROR
#define message_PRINTERROR 0x80144
#endif

#ifndef message_PRINTSAVE
#define message_PRINTSAVE 0x80142
#endif
*/


#define Message_IsBounce( event, oldref)			\
	( 							\
		(event)->type==event_USERMESSAGEACK		\
		&& 						\
		(event)->data.message.header.myref==(oldref)	\
	) 							\
	? 							\
	TRUE : FALSE;						\
	/* This returns TRUE if the event is our message being	*/
	/* returned to us by the wimp.				*/

#define Message_IsReply( event, oldref)						\
	( (event)->data.message.header.yourref == (oldref)) ? TRUE : FALSE	\




static BOOL	Print__MessageHandler( event_pollblock *event, void *reference)
{
print__block	*print = (print__block *) reference;
BOOL			reply;
BOOL			bounced;

reply	= Message_IsReply( event, print->message_ref);
bounced	= Message_IsBounce( event, print->message_ref);


if ( print->progress == print__progress_2)	{

	if ( bounced)		{
		/* PrintSave message bounced...	*/
		if ( print->printfn)	Print__PrintIt( print);
		else	Print__Finish( print, TRUE, print_result_NEEDPRINTERMANAGER);

		return TRUE;
		}

	else if ( reply && event->data.message.header.action == message_PRINTERROR)	{
		Print__Finish( print, TRUE, print_result_PRINTERROR);
		return TRUE;
		}

	else if ( reply && event->data.message.header.action == message_PRINTFILE)	{
		print->progress = print__progress_4_5;
		return TRUE;
		}

	else if ( reply && event->data.message.header.action == message_DATASAVEACK)	{
		print->progress = print__progress_4_5;
		}
	}


if ( print->progress == print__progress_4_5)	{

	if ( /*reply &&*/ event->data.message.header.action == message_PRINTTYPEODD)	{
		message_block	message = event->data.message;

		if ( print->printfn)	{
			/* We can print the file ourselves...	*/
			message.header.action	= message_PRINTTYPEKNOWN;
			message.header.yourref	= event->data.message.header.myref;
			message.header.size	= 256;
			Wimp_SendMessage(
				event_SEND,
				&message, event->data.message.header.sender,
				0
				);
				/* This is the last message of the protool, so we don't	*/
				/* want it returned if not replied to.			*/
			Print__PrintIt( print);
			return TRUE;
			}

		else	{
			/* We can't print directly, so we ignore message_PRINTTYPEODD	*/
			/*  - printers will send DATASAVEACK next	*/
			}

		return TRUE;
		}

	else if ( reply && event->data.message.header.action == message_DATASAVEACK)	{
		/* Save the file, for queing etc., to the filename specified. Then send	*/
		/* message_DATALOAD.							*/
		/* wait for file to reach top of thr printer queue,			*/
		/* whereapon printers will send us a message_PRINTTYPEODD.		*/
		/* Note that Print_ doesn't deal with a broadcast PRINTTYPEODD.		*/

		print->progress = print__progress_6;
		if ( print->savefn)	{
			message_block	sreply;
			BOOL		error;

			error = print->savefn(
				&print->ppublic, &event->data.message.data.datasaveack
				);
			if (error)	{
				/* Couldn't save file.	*/
				/*Error_Report( 0, "Couldn't save the data to be printed");*/
				/* Leave savefn to report the error.	*/
				Print__Finish( print, TRUE, print_result_SAVEFAILED);
				return TRUE;
				}

			sreply = event->data.message;
			sreply.header.action	= message_DATALOAD;
			sreply.header.yourref	= event->data.message.header.myref;
			Wimp_SendMessage(
				event_SENDWANTACK,
				&sreply,
				event->data.message.header.sender,
				0
				);
			print->message_ref = sreply.header.myref;

			print->progress = print__progress_7;
			return TRUE;
			}

		else	{
			Error_Report( 0,
				"Printer busy, and we are unable to save "
				"data to be printed to printer queue"
				);
			Print__Finish( print, TRUE, print_result_CANTSAVE);
			}

		return TRUE;
		}
	}

if ( print->progress == print__progress_7)	{
	if ( bounced)	{
		Error_Report( 0, "Print bounced, progress_7");
		Print__Finish( print, TRUE, print_result_FAILED);
		return TRUE;
		}

	if ( reply && event->data.message.header.action == message_DATALOADACK)	{
		Print__Finish( print, TRUE, print_result_QUEUED);
		return TRUE;
		}
	}


if (reply)	{
	Error_ReportInternal( 0,
		"Unrecognised print reply %i, progress %i",
		event->data.message.header.action,
		print->progress
		);
	Print__Finish( print, TRUE, print_result_FAILED);
	return TRUE;
	}

return FALSE;
}




BOOL	Print_StartPrint(
		print_printfn	printfn,
		print_savefn	savefn,
		print_resultfn	resultfn,
		void			*reference,
		int			filetype,
		int			estsize,
		const char	*leafname,
		const char	*jobtitle
		)
{
message_block	message;
print__block	*print = (print__block *) malloc( sizeof( print__block));

if ( !print)	{
	Error_ReportInternal( 0, "Can't malloc print__block...");
	return ERROR;
	}


print->printfn	= printfn;
print->savefn	= savefn;
print->resultfn	= resultfn;
print->ppublic.reference	= reference;
print->ppublic.jobtitle	= jobtitle;

message.header.action		= message_PRINTSAVE;
message.header.size		= 256;
message.data.print.filler[4]	= estsize;
message.data.print.filetype	= filetype;
message.header.yourref		= 0;		/* not a reply*/
sprintf( message.data.print.filename, leafname);
Wimp_SendMessage( event_SENDWANTACK, &message, 0, 0);
print->message_ref = message.header.myref;

Print__ClaimMessages( print);

print->progress = print__progress_2;

return NOERROR;
}






static void	Print__ClaimMessages( print__block *print)
{
Event_Claim( event_USERMESSAGE, event_ANY, event_ANY, Print__MessageHandler, print);
Event_Claim( event_USERMESSAGERECORDED, event_ANY, event_ANY, Print__MessageHandler, print);
Event_Claim( event_ACK, event_ANY, event_ANY, Print__MessageHandler, print);
}

static void	Print__ReleaseMessages( print__block *print)
{
Event_Release( event_USERMESSAGE, event_ANY, event_ANY, Print__MessageHandler, print);
Event_Release( event_USERMESSAGERECORDED, event_ANY, event_ANY, Print__MessageHandler, print);
Event_Release( event_ACK, event_ANY, event_ANY, Print__MessageHandler, print);
}



static void	Print__Finish(
	print__block	*print,
	BOOL			releasemsgs,
	print_result	result
	)
{
if ( print->resultfn)	print->resultfn( &print->ppublic, result);
if ( releasemsgs)	Print__ReleaseMessages( print);
free( print);
}

