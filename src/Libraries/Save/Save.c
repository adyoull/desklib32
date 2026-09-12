/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Save.Save.c
    Author:  Copyright © 1994 Julian Smith and Jason Howat
    Version: 1.03 (03 Aug 2003)
    Purpose: Automated handling of save-as window
    Mods:    13-Jun-1994 - JPS - Added support for filetypes
             18-Jun-1994 - JDH - Changed sense of value returned by
                  save_filesaver to be in line with rest of DeskLib.
                  save_filsaver uses filename instead of stream.  Tidied code
                  to use core routines and proper DeskLib conventions.  Added
                  Save__KeyHandler to check for RETURN key in text icon.  For
                  a RETURN key or OK click, added check that filename isn't
                  just a leafname.  Changed Save__DragIconClickHandler to use
                  Icon_StartSolidDrag.  Changed several routines to make
                  messages in their own blocks -- they used to corrupt
                  event_lastevent.  In Save__MessageHandler the initial
                  reference check excludes unacknowledged messages that were
                  returned.
             03-Aug-2003 - Wimp_AddMessages which the Save__MessageHandler
                           listens for to be sure it'll always work
*/

#include "DeskLib:Event.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"
#include "DeskLib:KeyCodes.h"
#include "DeskLib:Error.h"
#include "DeskLib:Save.h"
#include "DeskLib:Str.h"
#include "Desklib:File.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void Save_SetFiletype(save_saveblock *saveblock, int filetype)
{
  char *spritename;

  saveblock->filetype = filetype;

  /* Only update the icon if it was specified to begin with */
  if (saveblock->dragsprite >= 0)
  {
    spritename = Icon_GetTextPtr(saveblock->window, saveblock->dragsprite);

    /* We can't set it - it is not indirected */
    if (spritename == NULL) return;

    sprintf(spritename, "file_%03x", filetype);

    Icon_ForceRedraw(saveblock->window, saveblock->dragsprite);
  }
}


static void Save__CleanIconText(window_handle window, icon_handle icon)
/* used to clean up writable-icon text - these seem to be
 * terminated by '\n', which is no use to <string.h> functions.
 */
{
  icon_block iconblock;

  Wimp_GetIconState(window, icon, &iconblock);
  Str_MakeASCIIZ(iconblock.data.indirecttext.buffer,
                 iconblock.data.indirecttext.bufflen-1);
}


static void Save__ResetSaveBlock(save_saveblock *saveblock)
{
  saveblock->ram_progress               = 0;
  saveblock->last_message_ref           = -1;
  saveblock->flags.data.we_are_dragging = FALSE;
}


static BOOL Save__SaveDataToFilename(char *filename, save_saveblock *saveblock)
{
  BOOL save_ok;

  save_ok = saveblock->filesaver(filename, saveblock->ref);

  if(save_ok)
    File_SetType(filename, saveblock->filetype);
  else
    saveblock->resulthandler(save_FILESAVERFAILED, saveblock);

  return save_ok;
}


static void Save__CloseMenuOrSaveWindow(save_saveblock *saveblock)
  /* A save has been completed with 'Select'.
   * If we were part of a menu, close menu.
   * If we were a save window, close this window.
   * If none of the above, our window contains other things, (it might be the
   *   main document window with a 'Quick save' icon), so do nothing.
   * The Event_ handlers will be released by Save_CloseHandler and
   * Save_MenusDeletedHandler, so we don't release them here.
   */
{
  if(saveblock->flags.data.is_menu)
    Wimp_CreateMenu((menu_block *)-1, 0, 0);  /* close menu */

  if(saveblock->flags.data.is_save_window)
    Wimp_CloseWindow(saveblock->window);      /* close window */
}


static BOOL Save__OKIconHandler(event_pollblock *event, void *ref)
{
  icon_block     iconblock;
  save_saveblock *saveblock = (save_saveblock *)ref;
  char           *filename;

  Wimp_GetIconState(saveblock->window, saveblock->filenameicon, &iconblock);
  filename = iconblock.data.indirecttext.buffer;

  if(LeafName(filename) != filename)    /* not just a leafname in icon */
  {
    Save__SaveDataToFilename(iconblock.data.indirecttext.buffer, saveblock);
    Save__ResetSaveBlock(saveblock);

    if((event->type == event_CLICK && event->data.mouse.button.data.select) ||
        event->type == event_KEY)
      Save__CloseMenuOrSaveWindow(saveblock);
  }
  else
    Error_Report(0, "To save, drag the icon to a directory display");

  return TRUE;
}


static BOOL Save__KeyHandler(event_pollblock *event, void *ref)
{
  if(event->data.key.code == keycode_RETURN)
    return Save__OKIconHandler(event, ref);

  return FALSE;
}


static BOOL Save__CancelIconHandler(event_pollblock *event, void *ref)
{
  UNUSED_ARG(event);

  Save__CloseMenuOrSaveWindow((save_saveblock *)ref);
  return TRUE;
}


static BOOL Save__DragIconClickHandler(event_pollblock *event, void *ref)
{
  save_saveblock *saveblock = (save_saveblock *)ref;

  if(!event->data.mouse.button.data.dragselect &&
     !event->data.mouse.button.data.dragadjust)
    return FALSE;

  saveblock->flags.data.quit_after_save =
                                      event->data.mouse.button.data.dragselect;

  saveblock->flags.data.we_are_dragging = TRUE;

  Icon_StartSolidDrag(saveblock->window, saveblock->dragsprite);

  return TRUE;
}


static BOOL Save__UserDragHandler(event_pollblock *event, void *ref)
{
  save_saveblock *saveblock = (save_saveblock *)ref;
  mouse_block    ptr;
  message_block  msg;

  UNUSED_ARG(event);

  /* Is the drag nothing to do with the save drag-icon? */
  if(!saveblock->flags.data.we_are_dragging)
    return FALSE;

  saveblock->flags.data.we_are_dragging = FALSE;

  Wimp_GetPointerInfo(&ptr);

  msg.header.size            = 80;
  msg.header.yourref         = 0;
  msg.header.action          = message_DATASAVE;
  msg.data.datasave.window   = ptr.window;
  msg.data.datasave.icon     = ptr.icon;
  msg.data.datasave.pos      = ptr.pos;
  msg.data.datasave.estsize  = saveblock->estimatedsize;
  msg.data.datasave.filetype = saveblock->filetype;

  strcpy(msg.data.datasave.leafname,
         LeafName(Icon_GetTextPtr(saveblock->window, saveblock->filenameicon)));

  Wimp_SendMessage(event_SENDWANTACK, &msg, ptr.window, ptr.icon);

  saveblock->last_message_ref = msg.header.myref;

  return TRUE;
}


static BOOL Save__MenusDeletedOrWindowClosedHandler(event_pollblock *event,
                                                    void *ref)
{
  save_saveblock *saveblock = (save_saveblock *)ref;

  /* Don't release handlers when menu/window is closed */
  if(!saveblock->flags.data.release_after)
    return FALSE;

  if((event->type == event_SEND && saveblock->flags.data.is_menu &&
      event->data.message.header.action == message_MENUSDELETED) ||
     (event->type == event_CLOSE  && saveblock->flags.data.is_save_window))
  {
    Save_ReleaseSaveHandlers(ref);
    free(saveblock);
  }

  return FALSE;
}


static BOOL Save__MessageHandler(event_pollblock *event, void *ref)
{
  save_saveblock *saveblock = (save_saveblock *)ref;
  message_block  *msg = &event->data.message;

  if(event->type != event_ACK &&
     msg->header.yourref != saveblock->last_message_ref)
    return FALSE;
  /* we are only interested in replies to our previous messages...
     or unacknowledged messages */

  if(msg->header.action == message_DATASAVE)
  {
    /* User has dragged onto useless part of the desktop...
     * This could be our original message_SENDWANTACK being sent to us
     * because the sprite was dragged onto one of *our task's* windows,
     * or a message_ACK sent to us by the WIMP because nobody responded
     * to our original message.
     */

    return FALSE;  /*  the app might want to deal with this
                    *  message to save into itself.
                    */
  }

  if(msg->header.action == message_DATASAVEACK)
  {
    BOOL save_ok;

    save_ok = Save__SaveDataToFilename(msg->data.datasaveack.filename,
                                       saveblock);
    if(save_ok)
    {
      message_block reply;

      memcpy(&reply, msg, sizeof(message_block));

      reply.header.action = message_DATALOAD;
      reply.data.dataload.size = 49;
      reply.header.yourref = reply.header.myref;

      Wimp_SendMessage(event_SENDWANTACK, &reply, reply.header.sender, 0);

      saveblock->last_message_ref = reply.header.myref;
    }
    else
      Save__ResetSaveBlock(saveblock);

    return TRUE;
  }

  if(msg->header.action == message_RAMFETCH)
  {
    /* If we ignore message_RAMFETCH, the receiving task should
     * try again using a transfer through <Wimp$Scrap>.
     */
    int           byteswritten;
    BOOL          last_ramtransfer;
    message_block reply;

    if(saveblock->ramsaver == NULL)
      return TRUE;

    byteswritten = (saveblock->ramsaver)(event_taskhandle, saveblock->ref,
                                         msg->header.sender,
                                         msg->data.ramfetch.buffer,
                                         msg->data.ramfetch.buffsize,
                                         saveblock->ram_progress);

    last_ramtransfer = byteswritten < msg->data.ramfetch.buffsize;
    saveblock->ram_progress += byteswritten;

    if(byteswritten < 0)
    {
      (saveblock->resulthandler)(save_RAMSAVERFAILED, saveblock);
      Save__ResetSaveBlock(saveblock);
      return TRUE;
    }

    memcpy(&reply, msg, sizeof(message_block));
    reply.header.yourref = reply.header.myref;
    reply.header.action  = message_RAMTRANSMIT;
    reply.data.ramtransmit.byteswritten = byteswritten;

    Wimp_SendMessage((last_ramtransfer) ? event_SEND : event_SENDWANTACK,
                     &reply, reply.header.sender, 0);

    saveblock->last_message_ref = reply.header.myref;

    if(last_ramtransfer)
    {
      Save__ResetSaveBlock(saveblock);
      if(saveblock->flags.data.quit_after_save)
        Save__CloseMenuOrSaveWindow(saveblock);

      (saveblock->resulthandler)(save_SAVEOK, saveblock->ref);
    }

    return TRUE;
  }

  if(msg->header.action == message_RAMTRANSMIT)
  {
    /* This is our message being returned to us by the Wimp  */
    Save__ResetSaveBlock(saveblock);
    (saveblock->resulthandler)(save_RECEIVERFAILED, saveblock->ref);
    return TRUE;
  }

  if(msg->header.action == message_DATALOADACK)
  {
    /* everything OK  */
    Save__ResetSaveBlock(saveblock);
    if(saveblock->flags.data.quit_after_save)
      Save__CloseMenuOrSaveWindow(saveblock);

    (saveblock->resulthandler)(save_SAVEOK, saveblock->ref);
    return TRUE;
  }

  if(msg->header.action == message_DATALOAD)
  {
    /* This is our message being returned to us by the Wimp  */
    Error_Report(0, "Received DATALOAD back - scrap file not loaded...");
    Save__ResetSaveBlock(saveblock);

    (saveblock->resulthandler)(save_RECEIVERFAILED, saveblock->ref);
    return TRUE;
  }

  return FALSE;
}


static void Save__DefaultResultHandler(save_result result, void *ref)
{
  UNUSED_ARG(ref);

  switch(result)
  {
    case save_RECEIVERFAILED:
      Error_Report(0, "Receiver failed to load scrap file");
      break;

    case save_FILESAVERFAILED:
      Error_Report(0, "Filer-saver failed");
      break;

    case save_RAMSAVERFAILED:
      Error_Report(0, "RAM-saver failed");
    default:
      break;
  }
}


typedef BOOL (*save_eventclaimorreleasefn)(event_type    eventtype,
                                           window_handle window,
                                           icon_handle   icon,
                                           event_handler handler,
                                           void          *reference);
  /* This just defines a function pointer which can point
   * to the two functions Event_Claim and Event_Release.
   */


static void Save__EventClaimOrRelease(save_saveblock *saveblock,
                                      save_eventclaimorreleasefn fn)
{
  if(saveblock->dragsprite >= 0)
  {
    (fn)(event_CLICK, saveblock->window, saveblock->dragsprite,
         Save__DragIconClickHandler, saveblock);

    (fn)(event_USERDRAG, event_ANY, event_ANY,
         Save__UserDragHandler, saveblock);
  }

  if(saveblock->okbutton >= 0)
    (fn)(event_CLICK, saveblock->window, saveblock->okbutton,
         Save__OKIconHandler, saveblock);

  if(saveblock->cancelbutton >= 0)
    (fn)(event_CLICK, saveblock->window, saveblock->cancelbutton,
         Save__CancelIconHandler, saveblock);

  (fn)(event_KEY, saveblock->window, saveblock->filenameicon,
       Save__KeyHandler, saveblock);

  (fn)(event_CLOSE, saveblock->window, event_ANY,
       Save__MenusDeletedOrWindowClosedHandler, saveblock);

  (fn)(event_SEND, event_ANY, event_ANY,
       Save__MenusDeletedOrWindowClosedHandler, saveblock);

  /* we always want to check whether to release the handlers
   * when a menu has been closed because flags.data.is_menu
   * may be changed after Save_InitSaveWindowHandler
   */

  (fn)(event_SEND, event_ANY, event_ANY, Save__MessageHandler, saveblock);
  (fn)(event_SENDWANTACK, event_ANY, event_ANY,
       Save__MessageHandler, saveblock);
  (fn)(event_ACK, event_ANY, event_ANY, Save__MessageHandler, saveblock);
}



save_saveblock *Save_InitSaveWindowHandler(window_handle      window,
                                           BOOL               is_menu,
                                           BOOL               is_save_window,
                                           BOOL               release_after,
                                           icon_handle        dragsprite,
                                           icon_handle        okbutton,
                                           icon_handle        cancelbutton,
                                           icon_handle        filenameicon,
                                           save_filesaver     filesaver,
                                           save_ramsaver      ramsaver,
                                           save_resulthandler resulthandler,
                                           size_t             estimatedsize,
                                           int                filetype,
                                           void               *ref)
{
  save_saveblock *saveblock;
  int savemessages[7] = { message_DATASAVE,
                          message_DATASAVEACK,
                          message_RAMFETCH,
                          message_RAMTRANSMIT,
                          message_DATALOADACK,
                          message_DATALOAD,
                          0
                        };

  if(!filesaver)
  {
    /* Must have a file-saver  */
    Error_Report(0, "Save_InitSaveWindowHandler called with NULL filesaver");
    return NULL;
  }

  saveblock = malloc(sizeof(save_saveblock));
  if(!saveblock)
  {
    Error_OutOfMemory(FALSE, "save window");
    return NULL;
  }

  saveblock->window                    = window;
  saveblock->flags.data.is_menu        = is_menu;
  saveblock->flags.data.is_save_window = is_save_window;
  saveblock->flags.data.release_after  = release_after;
  saveblock->dragsprite                = dragsprite;
  saveblock->okbutton                  = okbutton;
  saveblock->cancelbutton              = cancelbutton;
  saveblock->filenameicon              = filenameicon;
  saveblock->filesaver                 = filesaver;
  saveblock->ramsaver                  = ramsaver;
  saveblock->resulthandler             = resulthandler;
  saveblock->estimatedsize             = estimatedsize;
  saveblock->ref                       = ref;
  saveblock->filetype                  = filetype;

  /* Recreate the filetype icon as an indirected sprite-only icon with
   * the appropriate filetype sprite in place - only if the icon is "real"
   */
  if (saveblock->dragsprite >= 0)
  {
    saveblock->dragsprite = Icon_FileIcon(saveblock->window,
                                          saveblock->dragsprite, filetype);
  }

  Save__ResetSaveBlock(saveblock);

  if(saveblock->resulthandler == NULL)
    saveblock->resulthandler = Save__DefaultResultHandler;

  /*  Make sure the terminator of the filename is '\0' - templates
   *  seem to come with a '\n' as terminator.
   */
  Save__CleanIconText(saveblock->window, saveblock->filenameicon);


  /*  Claim the relevant poll events and ensure the user messages pertaining to
   *  saving are being passed in.
   *  Since DeskLib only supports Wimp 3.10 and later this is unconditional
   */
  Save__EventClaimOrRelease(saveblock, Event_Claim);
  Wimp_AddMessages(savemessages);

  return saveblock;
}


void Save_ReleaseSaveHandlers(save_saveblock *saveblock)
{
  Save__EventClaimOrRelease(saveblock, Event_Release);
}

