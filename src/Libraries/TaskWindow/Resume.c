
#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:TaskWindow.h"

/*
  This sends a message telling a previously suspended taskwindow
  task to resume.
*/
void TaskWindow_Resume(task_handle handle)
{
  message_header mess;

  mess.size = sizeof(message_header);
  mess.yourref = 0;
  mess.action = message_TASKWINDOW_RESUME;

  Wimp_SendMessage(event_SEND, (message_block *) &mess, handle, 0);
}

