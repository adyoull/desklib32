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

#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Wimp.h"

#include "Desklib:WimpMsg.h"

void WimpMsg_Ack(message_block *previous)
{
  message_block msg = *previous;

  msg.header.yourref = msg.header.myref;

  Wimp_SendMessage(event_ACK, &msg, previous->header.sender, 0);
}
