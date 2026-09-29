
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "DeskLib:Event.h"
#include "DeskLib:EventMsg.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Error.h"
#include "DeskLib:Time.h"

#include "DeskLib:TaskWindow.h"

#define SWI_Wimp_StartTask 0x400DE

/* These error numbers are not significant */
error_STATIC(err_BUFFSIZE, 0, "Buffer too small to hold TaskWindow command.");
error_STATIC(err_NOMEM, 1, "Not enough memory to store taskwindow references.");

struct tw_info
{
  task_handle handle;
  int txt_handle;
  tw_output_fn *output;
  tw_morio_fn *died;
  void *ref;
};


static BOOL TaskWindow_EgoMessage(event_pollblock *event, void *ref)
{
  struct tw_info *info = ref;

  if (event->data.message.data.words[0] == info->txt_handle)
  {
    info->handle = event->data.message.header.sender;

    EventMsg_ReleaseSpecific(message_TASKWINDOW_EGO, event_ANY,
                             TaskWindow_EgoMessage, info);

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

static BOOL TaskWindow_Messages(event_pollblock *event, void *ref)
{
  struct tw_info *info = ref;

  /* If it doesn't concern this task, ignore it. */
  if (event->data.message.header.sender != info->handle) return FALSE;

  if (event->data.message.header.action == message_TASKWINDOW_OUTPUT)
  {
    /* We ack the message */
    event->data.message.header.yourref = event->data.message.header.myref;
    Wimp_SendMessage(event_ACK, &event->data.message,
                     event->data.message.header.sender, 0);

    /* And send output to the handler (as appropriate) */
    if (info->output != NULL)
    {
      info->output(event->data.message.data.bytes + 4,
                   event->data.message.data.words[0],
                   info->ref);
    }
  }
  else if (event->data.message.header.action == message_TASKWINDOW_MORIO)
  {
    if (info->died != NULL) info->died(info->ref);

    EventMsg_ReleaseSpecific(message_TASKWINDOW_OUTPUT, event_ANY,
                             TaskWindow_Messages, info);
    EventMsg_ReleaseSpecific(message_TASKWINDOW_MORIO, event_ANY,
                            TaskWindow_Messages, info);

    free(info);
  }

  return TRUE;
}


/*
  Takes a command to start in a taskwindow, a name for that task and a
  wimpslot (or zero to not specify a wimpslot).  Wimpslot sizes are in
  kilobytes, not bytes.  If the task is started successfully (and 'handle'
  is not NULL), then the task handle of the started task is returned in
  handle.  If there is a problem, an error block is returned, otherwise
  it returns NULL.

  If there is any output from the task, it is passed to the "output"
  function for the parent task to process or display.  When the task
  exits, the "died" function is called, and all the handlers attached
  to events by TaskWindow_Start are released.

  "ref" is a value of your choosing, passed to both the output and the
  died function so that you can identify the task being executed.
*/
os_error *TaskWindow_Start(const char *command, const char *taskname,
                           size_t wimpslot_k, tw_output_fn output,
                           tw_morio_fn died, void *ref,
                           task_handle *handle)
{
  char starttask[1024] = "";
  int n, end;
  int command_len = strlen(command);
  int name_len = strlen(taskname);
  task_handle newtask;
  struct tw_info *newinfo;
  os_error *err;

  /* First check we have space in the buffer for the complete command */
  end = sizeof(starttask);

  /* Static string size */
  end -= 65 + command_len + name_len;

  if (wimpslot_k > 0)
  {
    /* Static string size (and an assumed 1 certain character for the number) */
    end -= 13;

    /* the number size */
    for (n = wimpslot_k; n > 10; n /= 10) end--;
  }

  /* Extra characters needed to escape characters in command and name */
  for (n = 0; n < command_len; n++) if (command[n] == '\"') end--;
  for (n = 0; n < name_len; n++) if (taskname[n] == '\"') end--;

  /* Can't fit it - can't start it. */
  if (end <= 0) return err_BUFFSIZE;

  /*
   * If we can't store info about the task, there's no point
   * starting it like this.
   */
  newinfo = malloc(sizeof(*newinfo));
  if (newinfo == NULL) return err_NOMEM;

  /* Compile the taskwindow command, escaping "s in the command and taskname */
  strcpy(starttask, "Taskwindow \"");
  end = 12;

  for (n = 0; n < command_len; n++)
  {
    switch (command[n])
    {
      case '\"': starttask[end++] = '\"';  starttask[end++] = '\"';  break;
      default: starttask[end++] = command[n];
    }
  }

  end += sprintf(starttask + end, "\" -name \"");

  for (n = 0; n < name_len; n++)
  {
    switch (taskname[n])
    {
      case '\"': starttask[end++] = '\"';  starttask[end++] = '\"';  break;
      default: starttask[end++] = taskname[n];
    }
  }

  newinfo->txt_handle = Time_Monotonic();
  end += sprintf(starttask + end, "\" -quit -ctrl -task &%x -txt &%x", event_taskhandle, newinfo->txt_handle);

  if (wimpslot_k > 0) sprintf(starttask + end, " -wimpslot %zdk", wimpslot_k);

  /* Start the task, return any error */
  err = SWI(1, 1, SWI_Wimp_StartTask, starttask, &newtask);
  if (err) return err;

  newinfo->handle = newtask;
  newinfo->output = output;
  newinfo->died = died;
  newinfo->ref = ref;

  EventMsg_Claim(message_TASKWINDOW_OUTPUT, event_ANY,
                 TaskWindow_Messages, newinfo);
  EventMsg_Claim(message_TASKWINDOW_MORIO, event_ANY,
                 TaskWindow_Messages, newinfo);

  EventMsg_Claim(message_TASKWINDOW_EGO, event_ANY,
                 TaskWindow_EgoMessage, newinfo);

  if (handle != NULL) *handle = newtask;

  return NULL;
}


