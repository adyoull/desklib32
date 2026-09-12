/*
 * File: WimpMsg
 * Author: Antony Sidwell, 2003
 *
 * Abstract: A simpler interface to sending Wimp messages.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

#include <string.h>

#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Wimp.h"

#include "Desklib:WimpMsg.h"


int WimpMsg_Reply(message_block *previous, event_type type, message_action action, void *message_body, size_t datalen)
{
  message_block msg;

  /* Make sure that the data will fit in a message block */
  if (datalen > 236) return 0;

  msg.header.size = 20 + WORDALIGN(datalen);
  msg.header.action = action;
  msg.header.yourref = previous->header.myref;

  memcpy(msg.data.bytes, message_body, datalen);

  if (Wimp_SendMessage(type, &msg, previous->header.sender, 0) == NULL)
  {
    return msg.header.myref;
  }
  else
  {
    return 0;
  }
}
