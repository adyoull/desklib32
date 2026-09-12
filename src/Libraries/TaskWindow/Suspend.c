
#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:TaskWindow.h"

/*
  This sends a message to suspend the specified taskwindow task.
  It can be started up again using TaskWindow_Resume.
*/
void TaskWindow_Suspend(task_handle handle)
{
  message_header mess;

  mess.size = sizeof(message_header);
  mess.yourref = 0;
  mess.action = message_TASKWINDOW_SUSPEND;

  Wimp_SendMessage(event_SEND, (message_block *) &mess, handle, 0);
}

