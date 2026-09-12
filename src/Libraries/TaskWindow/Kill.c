
#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:TaskWindow.h"

/*
  This sends a message to the specified taskwindow task in
  order to kill it.
*/
void TaskWindow_Kill(task_handle handle)
{
  message_header mess;

  mess.size = sizeof(message_header);
  mess.yourref = 0;
  mess.action = message_TASKWINDOW_MORITE;

  Wimp_SendMessage(event_SEND, (message_block *) &mess, handle, 0);
}

