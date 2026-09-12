
#include <string.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:TaskWindow.h"

/*
  This sends "input" (i.e. keypresses) to the specified taskwindow task.
  The data need not be a C string.

  You can also send data using the Data Transfer Protocol - just send a
  DataSave message to the task, and then handle the resulting RAM transfer.
*/
void TaskWindow_SendInput(task_handle handle, const char *data, size_t datalen)
{
  message_block mess;
  size_t sent = 0;

  mess.header.yourref = 0;
  mess.header.action = message_TASKWINDOW_INPUT;

  /* Send messages until there is no more to send. */
  while (sent < datalen)
  {
    if ((datalen - sent) < 232)
    {
      mess.header.size = WORDALIGN(24 + (datalen - sent));
      mess.data.words[0] = 24 + (datalen - sent);
      memcpy(mess.data.bytes + 4, data + sent, (datalen - sent));
      sent = datalen;
    }
    else
    {
      mess.header.size = 256;
      mess.data.words[0] = 232;
      memcpy(mess.data.bytes + 4, data + sent, 232);
      sent += 232;
    }

    Wimp_SendMessage(event_SEND, &mess, handle, 0);
  }
}

