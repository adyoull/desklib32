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

#include "DeskLib:SWI.h"
#include "DeskLib:Wimp.h"

#include "Desklib:WimpMsg.h"

#ifndef SWI_Wimp_SendMessage
#define SWI_Wimp_SendMessage ((message_action) 0x400E7)
#endif


task_handle WimpMsg_GetTaskHandle(window_handle window, icon_handle icon)
{
  message_header msg;
  task_handle handle;

  msg.size = 20;
  msg.yourref = 0;
  msg.action = (message_action)0;

  SWI(4, 3, SWI_Wimp_SendMessage, event_ACK, &msg, window, icon,
                                  NULL, NULL, &handle);

  return handle;
}
