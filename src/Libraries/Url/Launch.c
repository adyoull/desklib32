/*
 * File: Url.Launch.c
 * Author: Antony Sidwell, 4th July 2002
 *
 * Abstract: Handle URL launching using the Event & EventMsg bits of DeskLib.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

/*
 * About Url.Launch.c
 * ------------------
 * To launch an URL, we follow a simple series of steps:
 * 1) Broadcast an ANT request
 * 2) If that isn't claimed by anyone, do the Acorn URI_Dispatch
 *    which first broadcasts and then tries to load an app to deal with it
 * 3) If that fails, try and load an app using the ANT loading method.
 * 4) If that fails, call the failure function, which has a hook for giving
 *    user feedback.
 */

#include "DeskLib:Url.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:EventMsg.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Module.h"

#include <stdlib.h>
#include <string.h>

#include "Url.h"


/* Function prototypes for internal functions */
static BOOL Url_ReturnMessage_h(event_pollblock *event, void *param);

/* Internal data structure */
typedef struct
{
  char *url;
  urllaunch_handler *success;
  urllaunch_handler *failure;
  char *rma_url;
} url_info_blk;

/* ---------------------------------------------------------------------
 * The URL launching has succeeded.
 * --------------------------------------------------------------------- */
static BOOL Url_Succeeded(event_pollblock *event, void *param)
{
  url_info_blk *block = (url_info_blk *) param;
  UNUSED(event);
      
  /* Release the various events we claimed to measuere success */
  EventMsg_Release(message_URI_MRETURNRESULT, event_ANY, Url_ReturnMessage_h);
  EventMsg_Release(message_ANTOPENURL, event_ANY, Url_ReturnMessage_h);
  Event_Release(event_NULL, event_ANY, event_ANY, Url_Succeeded, param);
         
  /* If we have a copy of the URL in RMA, free it */
  if (block->rma_url)
  { 
    Module_Free(block->rma_url);
    block->rma_url = NULL;
  }

  if (block->success != NULL)
  {
    block->success(block->url);
  }

  if (block->url != NULL) free(block->url);
  free(block);

  return (TRUE);
}

/* ---------------------------------------------------------------------
 * The URL launching has failed.
 * --------------------------------------------------------------------- */
static void Url_Failed(url_info_blk *block)
{
  /* Release the various events we claimed to measuere success */
  EventMsg_Release(message_URI_MRETURNRESULT, event_ANY, Url_ReturnMessage_h);
  EventMsg_Release(message_ANTOPENURL, event_ANY, Url_ReturnMessage_h);
  Event_Release(event_NULL, event_ANY, event_ANY, Url_Succeeded, block);

  /* If there was a "failure" function given, call it now. */
  if (block->failure != NULL) block->failure(block->url);

  /* Free our copy of the URL we were calling */
  if (block->url != NULL) free(block->url);

  /* And the data block containing it and the callbacks */
  free(block);

  return;
}

/* ---------------------------------------------------------------------
 * Broadcast an URL using the ANT protocol.
 *
 * Format of the message is simple - the URL makes up the "data" section
 * of the message.  This is only suitable for shortish URLs, less than
 * 235 (?) characters.
 * --------------------------------------------------------------------- */
static int Url_ANTBroadcast(url_info_blk *block, const char *url)
{
  message_block msg;
  message_anturl *antmsg = (message_anturl *) &msg.data;

  /* Prepare the wimp message header */
  msg.header.yourref = 0;
  msg.header.action = message_ANTOPENURL;

  /* Long URL requires the indirected form to be used */
  if (strlen(url) > 235)
  {
    msg.header.size = sizeof(msg.header) + 24;

    antmsg->data.indirect.tag = 0;

    /* Claim some RMA memory for the url */
    if (Module_Claim(strlen(url) + 1, (void *) &block->rma_url) != NULL)
    {
      /* If it failes, return that we cannot broadcast the URL */
      return -1;
    }
                     
    strcpy(block->rma_url, url);

    antmsg->data.indirect.url.ptr = block->rma_url;
    antmsg->data.indirect.flags = 0;
    antmsg->data.indirect.body_file.offset = 0;
    antmsg->data.indirect.target.offset = 0;
    antmsg->data.indirect.body_mimetype.offset = 0;

    /* Attempt to broadcast the message, return an error if we fail */
    if (Wimp_SendMessage(event_SENDWANTACK, &msg, 0 , 0))
    {
      Module_Free(block->rma_url);
      block->rma_url = NULL;
      return -1;
    }
  }
  else
  {
    msg.header.size = WORDALIGN(sizeof(msg.header) + strlen(url));

    /* Write the URL string into the "data" part of the message */
    strcpy(antmsg->data.url, url);

    /* Attempt to broadcast the message, return an error if we fail */
    if (Wimp_SendMessage(event_SENDWANTACK, &msg, 0 , 0)) return -1;
  }

  return 0;
}

/* ---------------------------------------------------------------------
 * Uses the ANT load thingy, where we try to launch a given application
 * for this type of URL.
 * --------------------------------------------------------------------- */
static BOOL Url_ANTLoad(const char *url)
{
  char *command_line = NULL;
  char *colon;

  /* Find the protocol (method) delimiter */
  colon = strchr(url, ':');

  /* We've not found a ':', so we can't identify the protocol */
  if (colon == NULL) return (FALSE);

  /* Work out how long the command line should be, allocate space */
  command_line = malloc(14 + (colon - url) + 1 + strlen(url) + 1);

  /* We couldn't allocate memory, so return failure */
  if (command_line == NULL) return (FALSE);

  /* Build a system variable to read the contents of */
  strcpy(command_line, "Alias$URLOpen_");

  /* Append the "method" */
  strncat(command_line, url, colon - url);

  /* Terminate the string */
  command_line[14 + (colon - url)] = 0;

  /* If we can't read the contents of the variable we can't launch any application */
  if (getenv(command_line) == NULL) return (FALSE);

  /* Add a space to our string */
  strcat(command_line, " ");

  /* Write the rest of the URL into the buffer */
  strcat(command_line, url);

  /* Try and Launch the task, passing the URL as parameter */
  if (Wimp_StartTask(command_line + 6) != NULL)
  {
    /* Failure to start the task */
    return (FALSE);
  }

  return (TRUE);
}

/* ---------------------------------------------------------------------
 * Launch an URL using the Acorn URI protocol.
 * --------------------------------------------------------------------- */
static void Url_URIDispatch(url_info_blk *block)
{
  int flags;

  /* Dispatch the SWI, asking to be notified of the result with a message */
  if (SWI(3, 1, SWI_URI_Dispatch, 1, block->url, event_taskhandle, &flags) != NULL || flags & 1)
  {
    /* If we have failed, move straight onto the "antload" technique */
    if (Url_ANTLoad(block->url) == FALSE)
    {
      /* If that failed, report error and clean up */
      Url_Failed(block);
    }
  }

  return;
}

/* ---------------------------------------------------------------------
 * This is called when we get a return from our ANT broadcast message,
 * or URI message
 * --------------------------------------------------------------------- */
static BOOL Url_ReturnMessage_h(event_pollblock *event, void *param)
{
  url_info_blk *block = (url_info_blk *) param;

  /* This has come back from the Acorn system */
  if (event->data.message.header.action == message_URI_MRETURNRESULT)
  {
    /* Check the flags to see if the URL was claimed */
    if ((event->data.message.data.words[0] & 0x01) == 0)
    {
      /* The URL was claimed - we can stop now */
      Url_Succeeded(NULL, param);
      return (TRUE);
    }

    /* failed AcornURI. Try ANT launch */
    if (Url_ANTLoad(block->url) == FALSE)
    {
      Url_Failed(block);
    }
    else
    {
      Url_Succeeded(NULL, block);
    }

    /*
     * We haven't acknowledged the returnresult message, so the URI
     * handler task will automatically free the URI
     */
  }
  /* If this was a response to the ant broadcast */
  else if (event->data.message.header.action == message_ANTOPENURL)
  {
    if (event->type == event_USERMESSAGEACK)
    {
      /* If it was indirected, we must free our RMA space holding the URL */
      if (block->rma_url)
      { 
        Module_Free(block->rma_url);
        block->rma_url = NULL;
      }

      /* Try the acorn method instead */
      Url_URIDispatch(block);

      /* True because we handled the message, not because it succeeded */
      return (TRUE);
    }
  }

  return (FALSE);
}

/* ---------------------------------------------------------------------
 * Our "entry point" for the URL launch routines.
 * You can (optionally) supply a function to be called if an URL couldn't
 * be fetched.
 * --------------------------------------------------------------------- */
extern void Url_Launch(const char *url, urllaunch_handler *failure, urllaunch_handler *success)
{
  url_info_blk *block = NULL;

  block = malloc(sizeof(url_info_blk));
  if (block == NULL)
  {
    failure(url);
    return;
  }

  block->failure = failure;
  block->success = success;
  block->url = malloc(strlen(url) + 1);
  block->rma_url = NULL;

  /* Take a copy of the URL */
  if (block->url == NULL)
  {
    Url_Failed(block);
    return;
  }
  else
  {
    strcpy(block->url, url);
  }

  /* Claim these message before we've sent anything out */
  EventMsg_Claim(message_URI_MRETURNRESULT, event_ANY, Url_ReturnMessage_h, block);
  EventMsg_Claim(message_ANTOPENURL, event_ANY, Url_ReturnMessage_h, block);

  /* This is in case the original ANT broadcast succeeds! */
  Event_Claim(event_NULL, event_ANY, event_ANY, Url_Succeeded, block);

  /* If this fails, we give an immediate report of problems */
  if (Url_ANTBroadcast(block, url) == -1) Url_Failed(block);

  return;
}
