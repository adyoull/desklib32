/*
 * File: Url.Receive.c
 * Author: Copyright © Antony Sidwell, 2003
 *
 * Abstract: Receive URL requests from other apps, pass them to handlers.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

#include <stdlib.h>

#include "DeskLib:Event.h"
#include "DeskLib:EventMsg.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Url.h"
    
#include "Url.h"

/*
 * These are used to keep a simple linked list of
 * currently registered handler functions.
 */
struct handler_item
{
  struct handler_item *next;
  urlreceive_handler *handler;
};

static struct handler_item *handler_list = NULL;



static BOOL Url_Received(event_pollblock *event, void *ref)
{
  struct handler_item *handler = ref;

  if (event->data.message.header.action == message_URI_MPROCESS)
  {
    message_uriprocess *urimess = (message_uriprocess *) &event->data.message.data;
    BOOL check_only = urimess->flags & 0x01;

    if (handler->handler(check_only, urimess->uri) == TRUE)
    {
      message_block mess = event->data.message;

      mess.header.yourref = mess.header.myref;
      mess.header.action = message_URI_MPROCESSACK;
      Wimp_SendMessage(event_ACK, &mess, mess.header.sender, NULL);

      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else if (event->data.message.header.action == message_ANTOPENURL)
  {
    message_anturl *urlmess = (message_anturl *) &event->data.message.data;

    /* We'll assume that it is the simple form, and change later as needed */
    const char *url = urlmess->data.url;

    if (urlmess->data.indirect.tag == 0)
    {
      if (urlmess->data.indirect.url.offset < 236 &&
          urlmess->data.indirect.url.offset > 0)
      {
        url = urlmess->data.url + urlmess->data.indirect.url.offset;
      }
      else if (urlmess->data.indirect.url.offset >= 0x01800000)
      {
        url = urlmess->data.indirect.url.ptr;
      }
      else
      {
        return FALSE;
      }
    }

    if (handler->handler(FALSE, url))
    {
      message_block mess = event->data.message;
      mess.header.yourref = mess.header.myref;
      Wimp_SendMessage(event_ACK, &mess, mess.header.sender, NULL);

      return TRUE;
    }
  }

  return FALSE;
}



extern BOOL Url_RemoveHandler(urlreceive_handler *handler)
{
  struct handler_item *item = handler_list;
  struct handler_item *prev = NULL;

  /* Find the item in the list */
  while (item != NULL && item->handler != handler)
  {
    prev = item;
    item = item->next;
  }

  if (item)
  {
    /* Release just the even claim for these messages & this item */
    EventMsg_ReleaseSpecific(message_URI_MPROCESS, event_ANY, Url_Received, item);
    EventMsg_ReleaseSpecific(message_ANTOPENURL, event_ANY, Url_Received, item);

    /* Remove the item from the list */
    if (prev) prev->next = item->next;
    else handler_list = item->next;

    free(item);
                
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}



extern BOOL Url_AddHandler(urlreceive_handler *handler)
{
  struct handler_item *item;

  item = malloc(sizeof(*item));

  if (item == NULL) return FALSE;

  item->handler = handler;
  item->next = handler_list;

  /* Place the item at the beginning of the list */
  if (handler_list == NULL) handler_list = item;

  /* Add new event claims for the new handler */
  EventMsg_Claim(message_URI_MPROCESS, event_ANY, Url_Received, item);
  EventMsg_Claim(message_ANTOPENURL, event_ANY, Url_Received, item);

  return TRUE;
}

